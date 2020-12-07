CC=g++
CFLAGS=-Wall -g

SRC=$(shell find src -name '*.cc')
OBJ=$(SRC:.cc=.o)

OUTDIR=bin
LIB=$(OUTDIR)/algo.so
TEST=$(OUTDIR)/test

all: $(LIB) $(TEST)

$(LIB): $(OBJ)
	mkdir -p $(OUTDIR)
	$(CC) $(CFLAGS) $^ -o $@ -shared

%.o: %.cc
	$(CC) $(CFLAGS) -c $< -o $@ -shared -fpic

$(TEST): $(OBJ)
	$(CC) $(CFLAGS) $^ test/test.cc -o $@

.PHONY: clean
clean:
	rm -f $(OBJ) $(LIB) $(TEST)
