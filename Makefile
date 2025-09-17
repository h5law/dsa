CFLAGS = -O2 -pipe
CFLAGS += -Wall -Wpedantic -Werror
# CFLAGS += -g -MD -DDEBUG_TESTS

SRCS = \
	   	src/vector.c \
	   	src/map.c
OBJS = $(SRCS:.c=.o)

all: clean build

always:
	mkdir -p src test

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

build: always $(OBJS)

clean:
	rm -f *.o */*.o **/*.o
	rm -f *.d */*.d **/*.d

.PHONY: all clean build
