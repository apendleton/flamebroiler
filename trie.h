#include "rbt.h"

typedef struct trie_node {
    uchr key;
    uchr *value;

    rb_tree_ptr children;
} trie_node_t, *trie_node_ptr;

typedef struct trie {
    trie_node_ptr root;
} trie_t, *trie_ptr;