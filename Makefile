
TOPTARGETS=all clean

SUBDIRS=src

MKFILE_PATH=$(abspath $(lastword $(MAKEFILE_LIST)))
MKFILE_DIR=$(dir $(MKFILE_PATH))
export ROOT_PROJECT_DIRECTORY=$(MKFILE_DIR)
export BIN_DIRECTORY=$(ROOT_PROJECT_DIRECTORY)bin/
export SCRIPT_DIRECTORY=$(ROOT_PROJECT_DIRECTORY)scripts/
export BUILD_SCRIPT_DIRECTORY=$(ROOT_PROJECT_DIRECTORY)build-scripts/

export CC=clang
export AS=nasm
export LD=/usr/local/opt/llvm/bin/ld.lld
export OBJCOPY=/usr/local/opt/llvm/bin/llvm-objcopy

export EXTENSION=c

ifeq ($(DEBUG),1)
override CFLAGS+= -DDEBUG=1 -g -O0
else
override CFLAGS+= -O3
endif

override CFLAGS+= -Wall -Wextra
override CFLAGS+= -masm=intel
override CFLAGS+= -std=c11
override ASFLAGS+=

override LDFLAGS+=
override LDFLAGS+=
override INCLUDE+=


export CFLAGS
export ASFLAGS
export LDFLAGS
export INCLUDE

export DEPENFLAGS= -MMD -MP

.PHONY: $(TOPTARGETS)

all: $(BIN_DIRECTORY)
	@:

$(BIN_DIRECTORY):
	@mkdir -p $@

clean:
	$(RM) -r $(BIN_DIRECTORY)

$(TOPTARGETS): $(SUBDIRS)

.PHONY: $(SUBDIRS)
$(SUBDIRS):
	@echo "make --directory=$@ $(MAKECMDGOALS)"
	@$(MAKE) --directory=$@ $(MAKECMDGOALS)

