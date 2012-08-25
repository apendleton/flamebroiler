#include <stdbool.h>
#include "rbt.h"

#ifndef TRIE_H
#define TRIE_H

typedef struct trie {
    wchar_t *value;

    rb_tree_ptr children;
} trie_t, *trie_ptr;

/* hacky dynamic array structure */
typedef struct {
    unsigned len, slots;
    wchar_t** arr;
} dyn_array;

trie_ptr create_trie();
void free_trie(void *trie);
void trie_insert(trie_ptr trie, wchar_t *key, wchar_t *value);
wchar_t* trie_search(trie_ptr trie, wchar_t *key);
dyn_array *trie_suffixes(trie_ptr trie, wchar_t *key, bool strict, int max_matches);

#endif