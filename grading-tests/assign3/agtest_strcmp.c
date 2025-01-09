// Test strcmp

#include "grade_strings.h"

void trace_strcmp(const char *a, const char *b);

void run_test(void) {
    static const char *inputs[][2] = {
        {"apples", "apples"},
        {"oranges", "apples"},
        {"apples and oranges", "apples"},
        {"apples and oranges", "oranges"},
        {"2022", "33"},
        {"orange", "ORANGE"},
        {"", "orange"},
        {"a", "A"},
        {"", ""},
    };
    for (int i = 0; i < COUNT(inputs); ++i) {
        trace_strcmp(inputs[i][0], inputs[i][1]);
    }
}

static char sign(int result) {
    if (result == 0) return '0';
    return (result < 0) ? '-' : '+';
}

void trace_strcmp(const char *a, const char *b) {
    char result = sign(strcmp(a, b));
    char expected = sign(ref_strcmp(a, b));
    char reverse = sign(strcmp(b, a));

    trace("call strcmp(\"%s\", \"%s\") returned %c, expected %c, with args reversed strcmp returned %c\n",
        a, b, result, expected, reverse);
}
