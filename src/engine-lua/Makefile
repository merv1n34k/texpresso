include ../../Makefile.config

## Target name
LUATEX_BIN?=texpresso-luatex

## Build tools
_CC?=gcc
_LD?=gcc
_CFLAGS?=-O2

## Build configuration
# For now, only build the main shim (stub).
# Phase 2.2 will add LuaTeX source compilation.
CFLAGS = $(_CFLAGS) $(_FLAGS) -I../include
LDFLAGS = $(_LDFLAGS)
DIRS = main

## Internal variables
BUILD = ../../build
BUILD_DIRS = $(BUILD) $(patsubst %,$(BUILD)/engine-lua/%, $(DIRS))

OBJECTS = $(BUILD)/engine-lua/main/main.o \
          $(BUILD)/engine-lua/main/texpresso_protocol.o \
          $(BUILD)/engine-lua/main/fork.o

## Main targets

all: $(BUILD)/$(LUATEX_BIN)

clean:
	rm -f $(OBJECTS) $(BUILD)/$(LUATEX_BIN)

## Internal targets

$(BUILD)/$(LUATEX_BIN): $(OBJECTS) $(BUILD)/common/libcommon.a
	$(_LD) -o $@ $^ $(LDFLAGS)

$(BUILD)/engine-lua/%.o: %.c | $(BUILD_DIRS)
	$(_CC) $(CFLAGS) -c -o $@ $^

$(BUILD)/common/libcommon.a:
	$(MAKE) -C ../common

$(BUILD_DIRS):
	mkdir -p $@

.PHONY: all clean
