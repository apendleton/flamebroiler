#include "rbt.h"

#ifndef TRIE_H
#define TRIE_H

typedef struct trie {
    uchr *value;

    rb_tree_ptr children;
} trie_t, *trie_ptr;

trie_ptr create_trie();
void free_trie(void *trie);
void trie_insert(trie_ptr trie, uchr *key, uchr *value);
uchr* trie_search(trie_ptr trie, uchr *key);

#endif