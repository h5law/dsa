CFLAGS = -xc -std=c99
CFLAGS += -march=native -O2 -pipe
# CFLAGS += -march=native -O3 -pipe
# CFLAGS += -funroll-loops -ffast-math
CFLAGS += -Wall -Wextra -pedantic
# CFLAGS += -g -MD -DDEBUG_TESTS

SRCS = \
	   	src/vector.c \
	   	src/map.c \
		src/list.c
OBJS = $(SRCS:.c=.o)

all: clean build

always:
	mkdir -p src test

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

build: always $(OBJS)

tests: always
	$(foreach src,$(SRCS), \
		$(CC) $(CFLAGS) -o $(src:.c=.o) $(src) $(LDFLAGS) \
	 )
	mv src/*.o test/

clean:
	rm -f *.o */*.o **/*.o
	rm -f *.d */*.d **/*.d

.PHONY: all clean build
