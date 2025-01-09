REF_LIB = libref_mango.a

DST_PATH ?= .
MANGO_INCLUDE_DIR = $(CS107E)/include
MANGO_LIB_DIR = $(CS107E)/lib
REF_HEADERS :=  $(subst .h,_ref.h,$(notdir $(wildcard $(MANGO_INCLUDE_DIR)/[^_]*.h)))

all: $(REF_LIB) $(REF_HEADERS)

# This is the magic -- get all global symbol names, objcopy and rename each, make new lib
# all cross-module references within ref library use other ref modules so you can link
# with orig lib also, there will be no conflicts
# Note: we need to include all external undefined symbols so that we pull in the
# undefined symbols associated with floating point arithmetic that are defined
# in libgcc.
$(REF_LIB): $(MANGO_LIB_DIR)/libmango.a
	riscv64-unknown-elf-objcopy 																										   \
		$(foreach sym,$(shell riscv64-unknown-elf-nm --extern-only --defined-only $< | grep -v " W " | cut -d' ' -f3-),--redefine-sym $(sym)=ref_$(sym)) \
		$(foreach sym,$(shell riscv64-unknown-elf-nm --extern-only --undefined-only $< | cut -d' ' -f3-), --keep-symbol=$(sym)) 		   \
		$< $@

# Create ref header from original by extracting
# all function declarations and prepend `ref_` to each function name
# Wild sed line courtesy of the fabulous Peter McEvoy
%_ref.h : $(MANGO_INCLUDE_DIR)/%.h
	@echo "#pragma once" > $@
	@echo "#include \"$*.h\"" >> $@
	cat $< | grep '^[^/*#} ].*[(].*[)].*;' | grep -v 'typedef' |sed 's/\([a-z0-9_]*\)(/ref_\1(/' >> $@

clean:
	rm -f $(REF_LIB) $(REF_HEADERS)

.PHONY: all clean
