SRCDIR=src
CC ?= cc
USERFLAGS+=
override CFLAGS+=-I$(SRCDIR) -g -Wall -Wpedantic $(USERFLAGS) -std=c11 -Wno-format-extra-args
PEDANTIC_CFLAGS=-std=c11 -Werror -Wpedantic -Wall -Wextra -Wformat=2 -O -Wuninitialized -Winit-self -Wswitch-enum -Wdeclaration-after-statement -Wshadow -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion -Waggregate-return -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wredundant-decls -Wnested-externs -Wno-long-long

ODIR=obj
LDIR =lib
LIBS=-lm

DEPS = $(wildcard $(SRCDIR)/*.h)
SRCS = $(wildcard $(SRCDIR)/*.c)
_OBJ = $(patsubst $(SRCDIR)/%,$(ODIR)/%,$(SRCS:.c=.o))
OBJ = $(filter-out $(ODIR)/main.o $(ODIR)/gui.o,$(_OBJ))

DEPS2 := $(OBJ:.o=.d)
-include $(DEPS2)


$(ODIR)/%.o: $(SRCDIR)/%.c
	+@[ -d $(ODIR) ] || mkdir -p $(ODIR)
	$(CC) -MMD $(CFLAGS) -c -o $@ $<


# GUI
ifeq ($(OS),Windows_NT)
ijvm-gui: $(OBJ) $(SRCDIR)/gui.c
		$(CC) $(CFLAGS) $(LIBS) -mwindows -o $@ $^ `pkg-config --cflags --libs gtk+-3.0 gmodule-2.0` -Wl,-export-all-symbols
else
ijvm-gui: $(OBJ) $(SRCDIR)/gui.c
	$(CC) $(CFLAGS) $(LIBS) -o $@ $^ `pkg-config --cflags --libs gtk+-3.0 gmodule-2.0` -rdynamic
endif

clean:
	-rm -f $(ODIR)/*.o *~ core.* $(INCDIR)/*~
	-rm -f $(ODIR)/*.d
	-rm -f ijvm-gui
	-rm -rf *.dSYM
	-rm -rf profdata/

tools:
	make -C tools/
