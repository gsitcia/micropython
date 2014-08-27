include ../py/mkenv.mk
-include mpconfigport.mk

CC := nspire-gcc
LD := arm-none-eabi-ld.gold

ZEHNFLAGS = --name "micropython" --author "micropython team, ported by Fabian Vogt" --notice "Interactive python console" --version 09

# define main target
PROG = micropython

# qstr definitions (must come before including py.mk)
QSTR_DEFS = qstrdefsport.h

# OS name, for simple autoconfig
UNAME_S := $(shell uname -s)

# Disable stripping of the main executable, relocation info is needed by genzehn
# DEBUG := 1
NOSTRIP := 1

# include py core make definitions
include ../py/py.mk

INC =  -I.
INC += -I$(PY_SRC)
INC += -I$(BUILD)

# compiler settings
CWARN = -Wall -Werror -Wno-error=cpp
CFLAGS = $(INC) $(CWARN) -ansi -std=gnu99 -DUNIX $(CFLAGS_MOD) $(COPT) $(CFLAGS_EXTRA)

# Debugging/Optimization
ifdef DEBUG
CFLAGS += -g
COPT = -O0
else
COPT = -Os -DNDEBUG
endif

LDFLAGS = $(LDFLAGS_MOD) -lm $(LDFLAGS_EXTRA) -Wl,--nspireio

# source files
SRC_C = \
	main.c \
	gccollect.c \
	input.c \
	file.c \
	modos.c \
	$(SRC_MOD)

OBJ = $(PY_O) $(addprefix $(BUILD)/, $(SRC_C:.c=.o))

include ../py/mkrules.mk

all: $(PROG).tns

$(PROG).tns: $(PROG)
	+genzehn --input $^ --output $@.zehn $(ZEHNFLAGS)
	+make-prg $@.zehn $@
	+rm $@.zehn
