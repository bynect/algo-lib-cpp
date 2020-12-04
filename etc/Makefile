SRC=$(shell find src -name '*.cc')
OBJ=$(SRC:.cc=.o)

OUTDIR=bin
LIB=$(OUTDIR)/algo.so
TEST=$(OUTDIR)/test

all: $(LIB) $(TEST)

$(LIB): $(OBJ)
	mkdir -p $(OUTDIR)
	g++ $^ -o $@ -shared

%.o: %.cc
	g++ -c $< -o $@ -shared -fpic

$(TEST): $(OBJ)
	g++ $^ test/test.cc -o $@

.PHONY: clean
clean:
	rm -f $(OBJ) $(LIB) $(TEST)
