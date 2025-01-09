// snprintf diassemble format extension

#include "grade_printf.h"

static void test_one_instruction(const unsigned int *pInsn);

// This function just here as code to disassemble for extension
int sum(int n) {
    int result = 6;
    for (int i = 0; i < n; i++) {
        result += i * 3;
    }
    return result + 729;
}

static void disassemble_instructions(const char *description, void *addr, int n) {
    trace("    %s instructions\n", description);
    trace("\n");
    unsigned int *cur = addr;
    while (n-- != 0 && *cur != 0) { // zero word at end of insns used as terminator
        test_one_instruction(cur);
        cur++;
    }
    trace(VISUAL_BREAK);
}

void run_test(void) {
    extern unsigned int _sample, _rtype, _itype, _stype, _utype, _pseudo;

    disassemble_instructions("sample", &_sample, -1);
    disassemble_instructions("rtype", &_rtype, -1);
    disassemble_instructions("itype", &_itype, -1);
    disassemble_instructions("stype", &_stype, -1);
    disassemble_instructions("utype", &_utype, -1);
//    disassemble_instructions("pseudo", &_pseudo, -1);
}

static bool isspace(char ch) { return ch == ' ' || ch == '\t' || ch == '\n'; }

static int tokenize(const char *orig, char *buf, const char *tokens[], int max) {
    ref_memcpy(buf, orig, ref_strlen(orig) + 1);
    ref_memset(tokens, 0, sizeof(tokens[0])*max);
    char *cur = buf;
    int n = 0;
    while (*cur) {
        while (*cur && (isspace(*cur) || *cur == ',')) {
            cur++;    // skip space,comma
        }
        char* start = cur;
        // token extends until first space/comma/paren
        while (*cur && !isspace(*cur) && *cur != ',' && *cur != '(') {
            cur++;
        }
        if (start == cur) {
            break;    // discard trailing space
        }
        tokens[n++] = start;
        if (!*cur || n == max) {
            break;
        }
        *cur++ = '\0';
    }
    return n;
}

#define NAN -99999999
// accept either decimal or hex for immediate value
// our dissasembly no prefix 0x to match objdump, must add prefix before convert
static long try_convert(const char *str, bool force_hex) {
    char copy[64] = {'0', 'x', 0};
    if (force_hex && str[0] != '0' && str[1] != 'x') {
        ref_memcpy(copy + 2, str, ref_strlen(str) + 1);
    } else {
        ref_memcpy(copy, str, ref_strlen(str) + 1);
    }
    const char *end;
    long val = ref_strtonum(copy, &end);
    return (*end == '\0') ? val : NAN;
}

static int tolower(char ch) {
    if (ch >= 'A' && ch <= 'Z') {
        return ch - 'A' + 'a';
    }
    return ch;
}

static int strcasecmp(const char *s1, const char *s2) {
    while (*s1 && tolower(*s1) == tolower(*s2)) {
        s1++;
        s2++;
    }
    return *s1 - *s2;
}

static bool fuzzy_match(const char *soln_orig, const char *stud_orig, uintptr_t iptr) {
    char soln_buf[128], stud_buf[128];
    const char *soln_tokens[10], *stud_tokens[10];
    tokenize(soln_orig, soln_buf, soln_tokens, COUNT(soln_tokens));
    tokenize(stud_orig, stud_buf, stud_tokens, COUNT(soln_tokens));

    bool imm_is_target = (soln_tokens[0][0] == 'b' || strcasecmp(soln_tokens[0],"jal") == 0 || strcasecmp(soln_tokens[0],"j") == 0);
    for (int i = 0; soln_tokens[i] || stud_tokens[i]; i++) {
        const char *a = soln_tokens[i], *b = stud_tokens[i];
        if (!b) return (*a == '<'); // if no more stud token, ok to discard soln label
        bool force_hex = imm_is_target && i > 0;
        long a_imm = try_convert(a, force_hex);
        long b_imm = try_convert(b, force_hex);
        if (a_imm != NAN && b_imm != NAN) {
            if (a_imm == b_imm) continue;
            if (imm_is_target && a_imm == iptr + b_imm) continue; // accept relative target
            return false;
        } else if (*a == '<' || *b == '<') {
            return true; // ignore rest of line if hit label
        } else if (strcasecmp(a, b) != 0) {
           return false;
        }
    }
    return true;
}

static const char *without_lead_space(const char *s) {
    while (isspace(*s)) s++;
    return s;
}

static bool not_decoded(const char *s) {
    if (!*s) return true;
    const char *end;
    long val = ref_strtonum(s, &end);
    return (*end == '\0' || *end == 'I');
}

void ref_disassemble_set_pseudo_enabled(bool b);

static void test_one_instruction(const unsigned int *pInsn) {
    # define BUFSIZE (1024)
    const char *format = "%pI";
    char dst_buf[BUFSIZE*10]; // oversize

    // Initialize contents of dst buffer
    ref_memset(dst_buf, 'a', BUFSIZE - 1);
    dst_buf[BUFSIZE - 1] = '\0';

    // Setup expected result
    char expected[BUFSIZE];
    char expected_alt[BUFSIZE];
    ref_disassemble_set_pseudo_enabled(true);
    ref_snprintf(expected, BUFSIZE, format, pInsn);
    ref_disassemble_set_pseudo_enabled(false);
    ref_snprintf(expected_alt, BUFSIZE, format, pInsn);
    bool has_alt = ref_strcmp(expected, expected_alt) != 0;

    // Run student code
    snprintf(dst_buf, BUFSIZE, format, pInsn);

    if (not_decoded(dst_buf)) {
        trace("[%08x] *** NOT DECODED ***\n", *pInsn);
    } else if (fuzzy_match(expected, dst_buf, (uintptr_t)pInsn) || (has_alt && fuzzy_match(expected_alt, dst_buf, (uintptr_t)pInsn))) {
        trace("[%08x] = %s\n", *pInsn, dst_buf);
    } else { // Show student output if mismatch
        trace("[%08x] *** MISMATCH ***      expect  =  %s\n", *pInsn, without_lead_space(expected));
        trace("                                 student =  %s\n", dst_buf);
    }
}
