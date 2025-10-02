CC := clang
AR := llvm-ar

CFLAGS := -std=c99
CFLAGS += -march=native -O2 -pipe
CFLAGS += -fPIC
# `CFLAGS += -DNO_DEBUG
# CFLAGS += -march=native -O3 -pipe
# CFLAGS += -funroll-loops -ffast-math
CFLAGS += -Wall -Wextra -pedantic
CFLAGS += -g -MD
ARFLAGS = rcs

SRCDIR = src
LIBDIR = lib
EXAMPLES = examples

SRCS = \
	   	$(SRCDIR)/vector.c \
	   	$(SRCDIR)/map.c \
		$(SRCDIR)/list.c
OBJS = $(SRCS:.c=.o)
HEADERS = $(SRCS:.c=.h)

# DEMOS = \
# 		$(EXAMPLES)/demo_vector.c \
# 		$(EXAMPLES)/demo_list.c
# DEMO_OBJS = $(DEMOS:.c=.o)
# EXES = $(DEMO_OBJS:.o=)

LIBNAME = dsa
PKGCONF = dsa.pc

PREFIX ?= /usr/local
INSTALL_INC  := $(PREFIX)/include/$(LIBNAME)
INSTALL_LIB  := $(PREFIX)/lib
INSTALL_PC   := $(PREFIX)/share/pkgconfig

all: clean build

always:
	mkdir -p $(SRCDIR) $(LIBDIR) $(EXAMPLES)

$(SRCDIR)/%.o: $(SRCDIR)/%.c
	@echo "Building $@ without debug logs"
	$(CC) $(CFLAGS) -DNO_DEBUG=1 -o $@ -c $<

$(LIBDIR)/$(LIBNAME).a: $(OBJS)
	@echo "Creating static library"
	$(AR) $(ARFLAGS) $@ $^

$(LIBDIR)/$(LIBNAME).so: $(OBJS)
	@echo "Creating shared library"
	$(CC) -shared -o $@ $(OBJS)

build: always $(OBJS) $(LIBDIR)/$(LIBNAME).a $(LIBDIR)/$(LIBNAME).so

# DEMO_CFLAGS := $(CFLAGS) -DNO_DEBUG=0 $(shell pkg-config --cflags dsa)
# DEMO_LDFLAGS := $(LDFLAGS) $(shell pkg-config --libs dsa)
#
# $(EXAMPLES)/%.o: $(EXAMPLES)/%.c
# 	$(CC) $(DEMO_CFLAGS) -c -o $@ $<
#
# $(EXAMPLES)/%: $(EXAMPLES)/%.o
# 	@echo "CFLAGS = $(DEMO_CFLAGS)"
# 	@echo "LDFLAGS = $(DEMO_LDFLAGS)"
# 	$(CC) $(DEMO_CFLAGS) -o $@ $< $(DEMO_LDFLAGS)
#
# build-examples: always $(EXES)

install: all
	mkdir -p $(INSTALL_INC)
	cp $(HEADERS) $(INSTALL_INC)/
	cp $(LIBDIR)/$(LIBNAME).a $(INSTALL_LIB)/lib$(LIBNAME).a
	cp $(LIBDIR)/$(LIBNAME).so $(INSTALL_LIB)/lib$(LIBNAME).so
	cp $(PKGCONF) $(INSTALL_PC)

deinstall: always
	rm -rf $(INSTALL_INC)
	rm -f $(INSTALL_LIB)/$(LIBNAME).a $(INSTALL_LIB)/$(LIBNAME).so
	rm -f $(INSTALL_PC)/$(PKGCONF)

clean:
	rm -f *.o */*.o */**/*.o
	rm -f *.d */*.d */**/*.d
	rm -f *.i */*.i */**/*.i
	rm -f $(LIBDIR)/*

.PHONY: all clean build buid-examples install deinstall

# vim: ft=make ts=4 sts=4 sw=2 ai
