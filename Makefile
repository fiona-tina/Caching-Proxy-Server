TARGETS=proxy_daemon
CC=g++
CCFLAGS=-ggdb3 -Wall -Werror -pedantic -std=c++11

.PHONY: all clean clobber

all: $(TARGETS)

proxy_daemon: proxy_daemon.cpp
	$(CC) $(CCFLAGS) -o $@ $<

cache: cache.cpp cache.h
	$(CC) $(CCFLAGS) -o $@ $<

clean:
	rm -f *~ $(TARGETS)

clobber:
	rm -f *~
