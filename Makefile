CFLAGS=-std=c17 -O2 -Wall -Wextra -fstack-protector -fstack-protector-all -pipe
RM=rm -vfr

CSRC=pin.c
OBJS=$(CSRC:.c=.o)

OS=$(shell uname -s)

ifeq ($(OS), Linux)
	CFLAGS+=-mtune=native
	LIBS+=-lbsd
endif

ifeq ($(OS), Darwin)
	CFLAGS+=-arch x86_64 -mmacosx-version-min=10.15
endif

.PHONY: all clean

all: zoom-pin

zoom-pin: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

clean:
	@rm -rvf zoom-pin $(OBJS)


pin.o: pin.c
