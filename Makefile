.POSIX:
MAKEFLAGS += --no-builtin-rules
MAKEFLAGS += --no-print-directory

BIN := scn

CC ?= cc

CFLAGS := -std=c99
CFLAGS += -D_POSIX_C_SOURCE=200809L
CFLAGS += -O2

CFLAGS += -Wall -Wextra
CFLAGS += -Wpedantic
CFLAGS += -Wshadow
CFLAGS += -Wconversion
CFLAGS += -Wmissing-prototypes
CFLAGS += -Wstrict-prototypes
CFLAGS += -Wno-unused-parameter
CFLAGS += -Wstrict-aliasing=2

LDFLAGS ?=
LDLIBS += -lcurl

INCLUDES := -I include -I src -I pkgs
INCLUDES += $(shell pkg-config --cflags libcurl)

PROFILE ?= release
ifeq ($(PROFILE),release)
    CFLAGS += -DNEBUG -fomit-frame-pointer
else
    CFLAGS += -g3
    CFLAGS += -DDEBUG_MODE
endif

ifdef SANITIZE
    PROFILE := sanitize
    CFLAGS  += -fsanitize=address,undefined -fno-omit-frame-pointer
    LDFLAGS += -fsanitize=address,undefined
endif

ifdef USE_LOCAL_DIRS
CFLAGS += -DUSE_LOCAL_DIRS
endif

BUILD_DIR := .build/$(PROFILE)

SRC_FILES := $(shell find src -name '*.c')

PKG_FILES := $(shell find pkgs -name '*.c')
CONFIG_FILE ?= etc-example/config.c

ALL_SRCS  = $(SRC_FILES) $(PKG_FILES) $(CONFIG_FILE)
OBJ_FILES = $(patsubst %.c,$(BUILD_DIR)/%.o,$(ALL_SRCS))
DEP_FILES = $(OBJ_FILES:.o=.d)


RM ?= rm --force

.PHONY: all clean switch

.NOTPARALLEL: all
all: $(REGISTRY) $(BIN)

$(BIN): $(OBJ_FILES)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

$(BUILD_DIR)/%.o: %.c
	@ mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) -MMD -MP -c -o $@ $<

-include $(DEP_FILES)

.PHONY: clean
clean:
	rm -rf .build

.PHONY: fclean
fclean: clean
	$(RM) scn

.NOTPARALLEL: re
.PHONY: re
re: fclean all

.PHONY: switch
switch: scn
	./scn switch

.PHONY: debug
debug:
	$(MAKE) all SANITIZE=1 PROFILE=1
