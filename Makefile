NAME = json-c-parser

SRCDIR = src
INCDIR = include

OBJDIR = obj
OUTDIR = bin
DIRS = $(OBJDIR) $(OUTDIR)

SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES))

BIN = $(OUTDIR)/lib$(NAME).a

CFLAGS += -std=gnu99 -pedantic -Wall -I$(INCDIR)

ifeq (1,$(DEBUG))
CFLAGS += -g -Wextra
else
CFLAGS += -O2
endif

CFLAGS += -fPIC
CFLAGS += $(DEFINES)

.PHONY: clean default debug example

default: $(BIN)

debug:
	export DEBUG=1; "$(MAKE)"

$(BIN): $(OBJECTS) | $(OUTDIR)
	ar rcs $@ $^

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) -c $< -o $@ $(CFLAGS)

$(DIRS):
	mkdir -p $@

clean::
	rm -rf $(DIRS)

EXAMPLEBIN=bin/example
example: $(EXAMPLEBIN)

$(EXAMPLEBIN): $(BIN) | $(DIR)
	gcc -o $@ $(CFLAGS) example.c bin/libjson-c-parser.a -ljson-c
