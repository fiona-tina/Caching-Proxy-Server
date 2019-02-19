TARGETS=cache_proxy_d
CC=gcc
CCFLAGS=-ggdb3 -Wall -Werror -pedantic

.PHONY: all clean clobber

all: $(TARGETS)

cache_proxy_d:
	$(CC) $(CCFLAGS) -o $@ $<

clean:
	rm -f *~ $(TARGETS)

clobber:
	rm -f *~
