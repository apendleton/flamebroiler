UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
LIBTYPE=so
endif
ifeq ($(UNAME), Darwin)
LIBTYPE=dylib
endif

all: clean build

clean:
	rm -f *.dylib *.gch trie_test

build:
	gcc -shared -fPIC -o libtrie.$(LIBTYPE) rbt.h trie.h rbt.c trie.c
	gcc -shared -fPIC -o libtrieprint.$(LIBTYPE) rbt.h trie.h trieprint.h trieprint.c