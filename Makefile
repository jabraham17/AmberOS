
TOPTARGETS=all clean

SUBDIRS=src

MKFILE_PATH=$(abspath $(lastword $(MAKEFILE_LIST)))
MKFILE_DIR=$(dir $(MKFILE_PATH))
export ROOT_PROJECT_DIRECTORY=$(MKFILE_DIR)
export LIB_DIRECTORY=$(ROOT_PROJECT_DIRECTORY)lib/
export BIN_DIRECTORY=$(ROOT_PROJECT_DIRECTORY)lib/
export INCLUDE_DIRECTORY=$(ROOT_PROJECT_DIRECTORY)include/

export CC=clang
export AS=nasm
export LD=/usr/local/opt/llvm/bin/ld.lld

export EXTENSION=c

ifeq ($(DEBUG),1)
override CFLAGS+= -DDEBUG=1 -g
else
override CFLAGS+= -O3
endif

override CFLAGS+= -Wall -Wextra
override CFLAGS+= -masm=intel
override CFLAGS+= -std=c11
override ASFLAGS+=

override LDFLAGS+=
override LDFLAGS+= -L$(LIB_DIRECTORY)
override INCLUDE+= -I$(INCLUDE_DIRECTORY)


export CFLAGS
export ASFLAGS
export LDFLAGS
export INCLUDE

export DEPENFLAGS= -MMD -MP

.PHONY: $(TOPTARGETS)

all: $(LIB_DIRECTORY)
	@:

$(LIB_DIRECTORY): 
	@mkdir -p $@

$(BIN_DIRECTORY):
	@mkdir -p $@

clean:
	$(RM) -r $(LIB_DIRECTORY) $(BIN_DIRECTORY)

$(TOPTARGETS): $(SUBDIRS)

.PHONY: $(SUBDIRS)
$(SUBDIRS):
	@echo "make --directory=$@ $(MAKECMDGOALS)"
	@$(MAKE) --directory=$@ $(MAKECMDGOALS)

