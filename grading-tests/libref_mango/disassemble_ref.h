#pragma once
#include "disassemble.h"
int ref_disassemble_is_valid_insn(uintptr_t addr);
const char *ref_disassemble_decode_address(char format, void *address);
