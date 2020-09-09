CFLAGS=-O2 -DNDEBUG -std=c17 -Wall -Wextra -fstack-protector -fstack-protector-all -pipe
#CFLAGS=-g -std=c17 -Wall -Wextra -fstack-protector -fstack-protector-all -pipe
RM=rm -vfr
YELLOW='\033[33m'
NORMAL='\033[0m'
INSTALL=ginstall

BINNAME=mkpin

CSRC=pin.c
OBJS=$(CSRC:.c=.o)

OS=$(shell uname -s)

ifeq ($(OS), Linux)
	CFLAGS+=-mtune=native
	LIBS+=-lbsd
	SHA256=sha256sum
endif

ifeq ($(OS), Darwin)
	CFLAGS+=-arch x86_64 -mmacosx-version-min=10.15
	SHA256=shasum -a 256
endif

.PHONY: all clean install

all: $(BINNAME)

mkpin: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

clean:
	@rm -rvf $(BINNAME) $(OBJS)

install: $(BINNAME)
	@printf "\033[33mInstalling $(BINNAME) to $(HOME)/bin, press CTRL-C to cancel... "
	@read  -n 1 -s
	@echo
	@test -d $(HOME)/bin || mkdir -vp $(HOME)/bin
	$(INSTALL) -S -sv -m 0755 $(BINNAME) $(HOME)/bin
	@ls -laF $(HOME)/bin/$(BINNAME)
	@$(SHA256) $(HOME)/bin/$(BINNAME)


pin.o: pin.c
