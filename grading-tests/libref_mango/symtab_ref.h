#pragma once
#include "symtab.h"
void ref_symtab_label_for_addr(char *buf, size_t bufsize, uintptr_t addr);
bool ref_symtab_symbol_for_name(const char *name, symbol_t *p_symbol);
bool ref_symtab_symbol_for_addr(uintptr_t addr, symbol_t *p_symbol);
