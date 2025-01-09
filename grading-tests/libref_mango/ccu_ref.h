#pragma once
#include "ccu.h"
long ref_ccu_config_pll_rate(pll_id_t id, long rate);
long ref_ccu_config_module_clock_rate(module_clk_id_t id, parent_id_t parent, long rate);
long ref_ccu_ungate_bus_clock(bgr_id_t reg_id);
long ref_ccu_ungate_bus_clock_bits(bgr_id_t reg_id, uint32_t gating_bits, uint32_t reset_bits);
void ref_ccu_debug_show_clocks(const char *label);
