#pragma once
#include "malloc.h"
void *ref_malloc(size_t nbytes);
void ref_free(void *ptr);
void *ref_sbrk(size_t nbytes);
void ref_malloc_report(void);
