CFLAGS = -O2 -pipe
CFLAGS += -Wall -Wpedantic -Werror
# CFLAGS += -g -MD -DDEBUG_TESTS

SRCS = \
	   src/map.c
OBJS = $(SRCS:.c=.o)

all: clean build

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

build: $(OBJS)

test:
	$(CC) $(CFLAGS) -o test map.c && ./test

clean:
	rm -f *.o */*.o **/*.o
	rm -f *.d */*.d **/*.d

.PHONY: all clean test
