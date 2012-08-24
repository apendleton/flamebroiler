all: clean build

clean:
	rm -f *.dylib *.gch trie_test

build:
	gcc -shared -fPIC -o libtrie.dylib rbt.h trie.h rbt.c trie.c
	gcc -shared -fPIC -o libtrieprint.dylib rbt.h trie.h trieprint.h trieprint.c

test:
	gcc -L. -ltrie -ltrieprint -o trie_test rbt.h trie.h trieprint.h trie_test.c