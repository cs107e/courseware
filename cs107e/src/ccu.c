/*
 * Functions to manage D1 clocks
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 * Sat Sep  7 11:08:33 PDT 2024
 */

#include "ccu.h"
#include "assert.h"
#include <stddef.h>
#include "printf.h"
#include "timer.h"

/*
 * Models the D-1 clock tree as diagrammed on p.39 of D-1 manual
 * (with some simplifications to support clocks we use)
 * See debug info below for more info on what is modeled.
 */

static long debug_rate_pll(pll_id_t id);
static long debug_rate_clk(module_clk_id_t id);
static long debug_rate_bgr(bgr_id_t id);
static long debug_rate_parent(parent_id_t id);
static int get_parent_src_index(module_clk_id_t id, parent_id_t parent);

typedef union {
    struct {
        uint32_t factor_m0  : 1;
        uint32_t factor_m1  : 1;
        uint32_t            : 6;
        uint32_t factor_n   : 8;
        uint32_t factor_p   : 6;
        uint32_t            : 5;
        uint32_t output_ena : 1;
        uint32_t locked     : 1;
        uint32_t lock_ena   : 1;
        uint32_t ldo_ena    : 1;
        uint32_t ena        : 1;
    };
    uint32_t bits;
} pll_reg_t;

typedef union {
    struct {
        uint32_t factor_m   : 5;
        uint32_t            : 3;
        uint32_t factor_n   : 2;
        uint32_t            : 14;
        uint32_t src        : 3;
        uint32_t            : 4;
        uint32_t ena        : 1;
    };
    uint32_t bits;
} module_clk_reg_t;

typedef struct {
    union {
        uint8_t raw[0xD10];
        uint32_t regs[1];
    } ;
} ccu_t;

#define CCU_BASE ((ccu_t *)0x02001000)
_Static_assert(&CCU_BASE->raw[0]                ==  (uint8_t *)0x02001000, "CCU pll cpu reg must be at address 0x02001000");
_Static_assert(&CCU_BASE->regs[0]               == (uint32_t *)0x02001000, "CCU pll cpu reg must be at address 0x02001000");
_Static_assert(&CCU_BASE->raw[CCU_APB0_CLK_REG] ==  (uint8_t *)0x02001520, "CCU apb0 reg must be at address 0x02001520");

static volatile ccu_t *const module = CCU_BASE;

static volatile uint32_t *reg_for_id(uint32_t raw_offset) {
    assert(raw_offset % 4 == 0);                // reject if not on 4-byte boundary
    assert(raw_offset < sizeof(module->raw));   // reject if not in range
    return (uint32_t *)(module->raw + raw_offset);
}

// list of recommended dividers for commonly used pll rates
// if you need an additional rate config, simply add to this table
static const struct pll_config_t {
    pll_id_t pll_id;
    uint32_t rate;
    struct { uint8_t P; uint8_t N; uint8_t M1; uint8_t M0; };
} pll_table[] = {
    {CCU_PLL_VIDEO0_CTRL_REG,  297000000, {        .N= 99,  .M1= 2 }},          // recommended dividers 99,2 p.43 D-1 manual
    {CCU_PLL_VIDEO0_CTRL_REG,  120000000, {        .N= 20,  .M1= 1 }},
    {CCU_PLL_AUDIO0_CTRL_REG,   22545454, {.P= 33, .N= 124, .M1= 1, .M0= 1 }},  // base rate for audio 22.1Khz
    {CCU_PLL_AUDIO1_CTRL_REG, 3072000000, {        .N= 128, .M1= 1 }},          // base rate for audio 24Khz
    {0}
};

static const struct pll_config_t *get_pll_config_for_rate(pll_id_t id, uint32_t rate) {
    for (const struct pll_config_t *cfg = pll_table; cfg->rate != 0; cfg++) {
        if (cfg->pll_id == id && cfg->rate == rate) return cfg;
    }
    error("No matching pll config found in rate table\n");
    return NULL;
}

#define ASSERT_IN_RANGE(val, lo, hi) assert((val) >= lo && (val) <= hi)
#define BITS_N_M1(n, m1) ((pll_reg_t){.factor_n=n, .factor_m1=m1}).bits
#define BITS_P_N_M1_M0(p, n, m1, m0) ((pll_reg_t){ .factor_p=p, .factor_n=n, .factor_m1=m1, .factor_m0=m0 }).bits;

static void get_pll_bits(pll_id_t id, uint32_t rate, uint32_t *factor_mask, uint32_t *new_factors) {
    const struct pll_config_t *cfg = get_pll_config_for_rate(id, rate);
    uint32_t out_mhz;

    switch(cfg->pll_id) {
    case CCU_PLL_VIDEO0_CTRL_REG: // N, M1
    case CCU_PLL_VIDEO1_CTRL_REG:
        ASSERT_IN_RANGE(cfg->N, 13, 255);
        ASSERT_IN_RANGE(cfg->M1, 1, 2);
        *factor_mask = BITS_N_M1(-1,-1);
        *new_factors = BITS_N_M1(cfg->N-1,cfg->M1-1);
        return;
    case CCU_PLL_AUDIO0_CTRL_REG:   // P, N, M1, M0
        ASSERT_IN_RANGE(cfg->P, 1, 64);
        ASSERT_IN_RANGE(cfg->N, 13, 255);
        ASSERT_IN_RANGE(cfg->M1, 1, 2);
        ASSERT_IN_RANGE(cfg->M0, 1, 2);
        out_mhz = (cfg->N/cfg->M0/cfg->M1)*24;
        ASSERT_IN_RANGE(out_mhz, 180, 3000); // valid output freq range is 180M-3G
        *factor_mask = BITS_P_N_M1_M0(-1,-1,-1,-1);
        *new_factors = BITS_P_N_M1_M0(cfg->P-1,cfg->N-1,cfg->M1-1,cfg->M0-1);
        return;
    case CCU_PLL_AUDIO1_CTRL_REG: // N, M1
        ASSERT_IN_RANGE(cfg->N, 13, 255);
        ASSERT_IN_RANGE(cfg->M1, 1, 2);
        out_mhz = (cfg->N/cfg->M1)*24;
        ASSERT_IN_RANGE(out_mhz, 180, 3500); // valid output freq range is 180M-3.5G
        *factor_mask = BITS_N_M1(-1,-1);
        *new_factors = BITS_N_M1(cfg->N-1,cfg->M1-1);
        return;

    case CCU_PLL_PERI_CTRL_REG:

    case CCU_PLL_CPU_CTRL_REG:
    case CCU_PLL_DDR_CTRL_REG:
    case CCU_PLL_VE_CTRL_REG:
        assert(!"Attempt to change PLL that should not be modified");
    }
}

// Procedure to update PLL from p46 of D-1 user manual
// NOTE: code explicitly sets and clears bits using bitwise ops!
// This ensures it acts exactly as required in spec
// (not using bitfields, gcc-generated code can vary)
static void update_pll_bits(volatile uint32_t *reg, uint32_t factor_mask, uint32_t factor_bits) {
    const uint32_t PLL_ENA =  (1 << 31);
    const uint32_t LOCK_ENA = (1 << 29);
    const uint32_t LOCKED =   (1 << 28);
    const uint32_t OUT_ENA =  (1 << 27);

    *reg |= PLL_ENA;            // enable PLL
    *reg &= ~OUT_ENA;           // disable output while changing
    *reg = (*reg & ~factor_mask) | factor_bits;   // clear previous factors & apply new
    *reg |= LOCK_ENA;           // lock enable
    while (! (*reg & LOCKED))   // wait until lock
       ;
    timer_delay_us(50);         // short delay to stabilize
    *reg |= OUT_ENA;            // re-enable output
}

long ccu_config_pll_dividers(pll_id_t id, uint32_t n, uint32_t m) {
    const pll_reg_t factor_mask = { .factor_m1 = -1,   .factor_n = -1 };
    pll_reg_t new_factors =       { .factor_m1 = m-1, .factor_n = n-1 };
    assert(n >= 13 && n <= 255); // confirm dividers within spec
    assert(m >= 1 && m <= 2);
    update_pll_bits(reg_for_id(id), factor_mask.bits, new_factors.bits);
    return debug_rate_pll(id);
}

long ccu_config_pll_rate(pll_id_t id, uint32_t rate) {
    uint32_t factor_mask, new_factors;
    get_pll_bits(id, rate, &factor_mask, &new_factors);
    update_pll_bits(reg_for_id(id), factor_mask, new_factors);
    uint32_t set_rate = debug_rate_pll(id);
    assert(rate == set_rate);
    return set_rate;
}

static uint32_t get_module_clk_bits(module_clk_id_t id, parent_id_t parent, uint32_t rate) {
    uint32_t parent_rate = debug_rate_parent(parent);
    int src = get_parent_src_index(id, parent);
    ASSERT_IN_RANGE(src, 0, 3);
    module_clk_reg_t new_settings = { .src= src, .factor_n= 0, .factor_m= 0 };

    if (parent_rate == rate) { // no dividers needed, src parent at desired rate
        return new_settings.bits;
    }
    int factor_goal = parent_rate/rate;
    assert(parent_rate % rate == 0);
    assert(factor_goal <= 32*8);
    if (factor_goal <= 32) {
        new_settings.factor_m = factor_goal-1;
    } else if (factor_goal % 8 == 0) {
        new_settings.factor_n = 3;
        new_settings.factor_m = factor_goal/8 - 1;
    } else if (factor_goal % 4 == 0) {
        new_settings.factor_n = 2;
        new_settings.factor_m = factor_goal/4 - 1;
    } else if (factor_goal % 2 == 0) {
        new_settings.factor_n = 1;
        new_settings.factor_m = factor_goal/2 - 1;
    } else {
        error("No compatible factors between parent and module rate\n");
    }
    return new_settings.bits;
}

/* From p47 D-1 user manual:
 * Configure the clock source and frequency division factor first,
 * and then release the clock gating (that is, set enable to 1)
 */
static void update_clock_bits(volatile uint32_t *reg, uint32_t bits) {
    const uint32_t ENA = (1 << 31);
    *reg &= ~ENA;   // disable clock during change
    *reg = bits;
    *reg |= ENA;    // re-enable
}

long ccu_config_module_clock(module_clk_id_t id, uint32_t src, uint32_t factor_n, uint32_t factor_m) {
    module_clk_reg_t new_settings = { .src= src, .factor_n= factor_n, .factor_m= factor_m };
    assert(src <= 7);
    assert(factor_n <= 3 && factor_m <= 31);
    update_clock_bits(reg_for_id(id), new_settings.bits);
    return debug_rate_clk(id);
}

long ccu_config_module_clock_rate(module_clk_id_t id, parent_id_t parent, uint32_t rate) {
    uint32_t new_bits = get_module_clk_bits(id, parent, rate);
    update_clock_bits(reg_for_id(id), new_bits);
    return debug_rate_clk(id);
}


/*
 *  From p47 D-1 user manual:
 *  For the Bus Gating Reset register of a module, the reset bit is de-asserted first,
 *  and then the clock gating bit is enabled to avoid potential problems
 *  caused by the asynchronous release of the reset signal.
 */
long ccu_ungate_bus_clock_bits(bgr_id_t id, uint32_t gating_bits, uint32_t reset_bits) {
    volatile uint32_t *reg = reg_for_id(id);
    *reg |= reset_bits;      // de-assert reset
    *reg |= gating_bits;     // enable
    return debug_rate_bgr(id);
}

// most bus clocks use standard bits for reset/gate
// general function above allow other use cases
long ccu_ungate_bus_clock(bgr_id_t id) {
    const uint32_t standard_gating_bits = 1 << 0;
    const uint32_t standard_reset_bits = 1 << 16;
    return ccu_ungate_bus_clock_bits(id, standard_gating_bits, standard_reset_bits);
}

/****  DEBUG INFO from here down ***/

#define NOT_IN_MODEL PARENT_NONE

struct debug_info {
    long (*fn)(uint32_t);
    uint32_t reg_id;
    const char *name;
    parent_id_t parents[4];
};

#define STRINGIFY(x) #x
#define INFO_PLL(x) debug_rate_pll, x, STRINGIFY(x)
#define INFO_CLK(x) debug_rate_clk, x, STRINGIFY(x)
#define INFO_BGR(x) debug_rate_bgr, x, STRINGIFY(x)

static struct debug_info info_table[] = {
    { .name= "PLL" },
    { INFO_PLL(CCU_PLL_CPU_CTRL_REG)    },
    { INFO_PLL(CCU_PLL_DDR_CTRL_REG)    },
    { INFO_PLL(CCU_PLL_PERI_CTRL_REG)   },
    { INFO_PLL(CCU_PLL_VIDEO0_CTRL_REG) },
    { INFO_PLL(CCU_PLL_VIDEO1_CTRL_REG) },
    { INFO_PLL(CCU_PLL_VE_CTRL_REG)     },
    { INFO_PLL(CCU_PLL_AUDIO0_CTRL_REG) },
    { INFO_PLL(CCU_PLL_AUDIO1_CTRL_REG) },
    { .name= "Module Clock" },       // parent not listed defaults to NOT_IN_MODEL
    { INFO_CLK(CCU_PSI_CLK_REG),      {PARENT_HOSC, PARENT_32K, NOT_IN_MODEL, PARENT_PERI} },
    { INFO_CLK(CCU_APB0_CLK_REG),     {PARENT_HOSC, PARENT_32K, PARENT_PSI, PARENT_PERI} },
    { INFO_CLK(CCU_APB1_CLK_REG),     {PARENT_HOSC, PARENT_32K, PARENT_PSI, PARENT_PERI} },
    { INFO_CLK(CCU_DRAM_CLK_REG),     {PARENT_DDR, NOT_IN_MODEL, PARENT_PERI_2X} },
    { INFO_CLK(CCU_DE_CLK_REG),       {PARENT_PERI_2X, PARENT_VIDEO0_4X, PARENT_VIDEO1_4X} },
    { INFO_CLK(CCU_TCONTV_CLK_REG),   {PARENT_VIDEO0, PARENT_VIDEO0_4X } },
    { INFO_CLK(CCU_HDMI_24M_CLK_REG), {PARENT_HOSC} },
    { INFO_CLK(CCU_SPI0_CLK_REG),     {PARENT_HOSC, PARENT_PERI, PARENT_PERI_2X } },
    { INFO_CLK(CCU_I2S2_CLK_REG),     {PARENT_AUDIO0, NOT_IN_MODEL, NOT_IN_MODEL, PARENT_AUDIO1_DIV5} },
    { .name= "Bus Clock" },
    { INFO_BGR(CCU_DE_BGR_REG),       {PARENT_AHB0} },
    { INFO_BGR(CCU_DPSS_TOP_BGR_REG), {PARENT_AHB0} },
    { INFO_BGR(CCU_HDMI_BGR_REG),     {PARENT_AHB0} },
    { INFO_BGR(CCU_TCONTV_BGR_REG),   {PARENT_AHB0} },
    { INFO_BGR(CCU_DMA_BGR_REG),      {PARENT_AHB0} },
    { INFO_BGR(CCU_HSTIMER_BGR_REG),  {PARENT_AHB0} },
    { INFO_BGR(CCU_PWM_BGR_REG),      {PARENT_APB0} },
    { INFO_BGR(CCU_UART_BGR_REG),     {PARENT_APB1} },
    { INFO_BGR(CCU_I2S_BGR_REG),      {PARENT_APB0} },
    { INFO_BGR(CCU_TWI_BGR_REG),      {PARENT_APB1} },
    { INFO_BGR(CCU_SPI_BGR_REG),      {PARENT_APB1} },
    {0},
  };

static int get_parent_src_index(module_clk_id_t id, parent_id_t parent) {
    for (struct debug_info *info = info_table; info->name; info++) {
        if (info->reg_id == id) {
            for (int i = 0; i < sizeof(info->parents)/sizeof(*info->parents); i++) {
                if (info->parents[i] == parent) return i;
            }
        }
    }
    return -1;
}
static struct debug_info *info_for_id(uint32_t id) {
    for (struct debug_info *info = info_table; info->name; info++) {
        if (!info->fn) continue;
        if (info->reg_id == id) {
            return info;
        }
    }
    error("No such clock reg id\n");
    return NULL;
}

void ccu_debug_show_clocks(const char *label) {
    printf("\n++++++++ CCU clock debug (%s) ++++++++\n", label);
    for (struct debug_info *i = info_table; i->name; i++) {
        if (!i->fn) {
            printf("\n        Rate  %s\n", i->name);
            continue;
        }
        long rate = i->fn(i->reg_id);
        if (rate != 0) printf("%12ld  %s\t raw=[%08x]\n", rate, i->name, *reg_for_id(i->reg_id));
    }
}

static long debug_rate_parent(parent_id_t id) {
    int mult = 1, div = 1;
    switch (id) {
        case NOT_IN_MODEL:      return -1;
        case PARENT_HOSC:       return 24*1000*1000;
        case PARENT_32K:        return 32768;
        case PARENT_DDR:        return debug_rate_pll(CCU_PLL_DDR_CTRL_REG);
        case PARENT_PERI_2X:           mult = 2; // *** fallthrough
        case PARENT_PERI:       return mult*debug_rate_pll(CCU_PLL_PERI_CTRL_REG);
        case PARENT_VIDEO0_4X:         mult = 4; // *** fallthrough
        case PARENT_VIDEO0:     return mult*debug_rate_pll(CCU_PLL_VIDEO0_CTRL_REG);
        case PARENT_VIDEO1_4X:         mult = 4; // *** fallthrough
        case PARENT_VIDEO1:     return mult*debug_rate_pll(CCU_PLL_VIDEO1_CTRL_REG);
        case PARENT_AUDIO0:     return debug_rate_pll(CCU_PLL_AUDIO0_CTRL_REG);
        case PARENT_AUDIO1_DIV5:       div = 5; // *** fallthrough
        case PARENT_AUDIO1:     return debug_rate_pll(CCU_PLL_AUDIO1_CTRL_REG) / div;
        case PARENT_APB0:       return debug_rate_clk(CCU_APB0_CLK_REG);
        case PARENT_APB1:       return debug_rate_clk(CCU_APB1_CLK_REG);
        case PARENT_AHB0:       return debug_rate_clk(CCU_PSI_CLK_REG);
        case PARENT_PSI:        return debug_rate_clk(CCU_PSI_CLK_REG);
    }
    return -1;
}

static long debug_rate_pll(pll_id_t id) {
    pll_reg_t pll;
    pll.bits = *reg_for_id(id);
    if (!pll.ena || !pll.output_ena) return 0;
    int p = pll.factor_p+1, n = pll.factor_n+1, m1 = pll.factor_m1+1, m0 = pll.factor_m0+1;
    long parent_rate = debug_rate_parent(PARENT_HOSC);

    switch (id) {
        case CCU_PLL_PERI_CTRL_REG:
        case CCU_PLL_VIDEO0_CTRL_REG:
        case CCU_PLL_VIDEO1_CTRL_REG:
            return parent_rate*n/m1/4;
        case CCU_PLL_AUDIO0_CTRL_REG:
            return parent_rate*n/m1/m0/p/4;
        default:
            return parent_rate*n/m1/m0;
    }
}

static long debug_rate_clk(module_clk_id_t id) {
    module_clk_reg_t clk;
    clk.bits = *reg_for_id(id);
    if (id > CCU_APB1_CLK_REG && !clk.ena) return 0; // cheezy (ena bits not applicable for psi/apb?)
    int n = 1 << clk.factor_n;
    int m = clk.factor_m + 1;
    struct debug_info *i = info_for_id(id);
    parent_id_t parent = i->parents[clk.src];
    return debug_rate_parent(parent)/n/m;
}

static long debug_rate_bgr(bgr_id_t id) {
    uint32_t val = *reg_for_id(id);
    struct debug_info *i = info_for_id(id);
    return (val & 0xff) ? debug_rate_parent(i->parents[0]) : 0;
}
