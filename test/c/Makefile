UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
LIBTYPE=so
endif
ifeq ($(UNAME), Darwin)
LIBTYPE=dylib
endif

all: clean test

clean:
	rm -f *.dylib *.gch trie_test

test:
	gcc -I../../flamebroiler -L../../flamebroiler -ltrie -ltrieprint -o trie_test trie_test.c