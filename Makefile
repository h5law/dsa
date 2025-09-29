CC := clang
AR := llvm-ar

CFLAGS := -xc -std=c99
CFLAGS += -march=native -O2 -pipe
CFLAGS += -fPIC -DNO_DEBUG
# CFLAGS += -march=native -O3 -pipe
# CFLAGS += -funroll-loops -ffast-math
CFLAGS += -Wall -Wextra -pedantic
# CFLAGS += -g -MD
ARFLAGS = rcs

SRCDIR = src
LIBDIR = lib
TESTDIR = test

SRCS = \
	   	$(SRCDIR)/vector.c \
	   	$(SRCDIR)/map.c \
		$(SRCDIR)/list.c
OBJS = $(SRCS:.c=.o)
HEADERS = $(SRCS:.c=.h)

LIBNAME = dsa
PKGCONF = dsa.pc

.PHONY: all clean build install deinstall

all: clean build

always:
	mkdir -p $(SRCDIR) $(LIBDIR) $(TESTDIR)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(LIBDIR)/$(LIBNAME).a: $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

$(LIBDIR)/$(LIBNAME).so: $(OBJS)
	$(CC) -shared -o $@ $(OBJS)

build: always $(OBJS) $(LIBDIR)/$(LIBNAME).a $(LIBDIR)/$(LIBNAME).so

PREFIX ?= /usr/local
INSTALL_INC  := $(PREFIX)/include/$(LIBNAME)
INSTALL_LIB  := $(PREFIX)/lib
INSTALL_PC   := $(PREFIX)/share/pkgconfig

install: all
	mkdir -p $(INSTALL_INC)
	cp $(HEADERS) $(INSTALL_INC)/
	cp $(LIBDIR)/$(LIBNAME).a $(INSTALL_LIB)/lib$(LIBNAME).a
	cp $(LIBDIR)/$(LIBNAME).so $(INSTALL_LIB)/lib$(LIBNAME).so
	cp $(PKGCONF) $(INSTALL_PC)

deinstall:
	rm -rf $(INSTALL_INC)
	rm -f $(INSTALL_LIB)/$(LIBNAME).a $(INSTALL_LIB)/$(LIBNAME).so
	rm -f $(INSTALL_PC)/$(PKGCONF)

clean:
	rm -f *.o */*.o */**/*.o
	rm -f *.d */*.d */**/*.d
	rm -f $(LIBDIR)/*

PHONY: all clean build
