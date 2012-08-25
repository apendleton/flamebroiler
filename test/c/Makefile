UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
LIBTYPE=so
endif
ifeq ($(UNAME), Darwin)
LIBTYPE=dylib
endif

all: clean test

clean:
	rm -f *.dylib *.gch *_test

test:
	gcc -I../../flamebroiler -L../../flamebroiler -ltrie -ltrieprint -o trieprint_test trieprint_test.c
	gcc -I../../flamebroiler -L../../flamebroiler -ltrie -ltrieprint -o suffix_test suffix_test.c