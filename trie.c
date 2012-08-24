#include <stdlib.h>
#include <locale.h>
#include <wchar.h>

#include "rbt.h"
#include "trie.h"

trie_ptr create_trie() {
    trie_ptr out = (trie_ptr)(malloc(sizeof(trie_t)));
    out->value = NULL;
    out->children = create_rb_tree();
    return out;
}

void free_trie(void *trie) {
    trie_ptr _trie = (trie_ptr)(trie);
    free_rb_tree(_trie->children, &free_trie);
    free(_trie->value);
    free(_trie);
}

void trie_insert(trie_ptr trie, uchr *key, uchr *value) {
    uchr next_letter;
    rb_node_ptr next_node;
    int key_length;

    key_length = wcslen(key);

    while (key_length > 0) {
        /* we're not at the leaf, so proceed down the tree */
        next_letter = *key;
        next_node = rb_tree_insert(trie->children, next_letter, NULL);

        if (next_node->value == NULL) {
            next_node->value = (void *)(create_trie());
        }

        key += 1;
        key_length -= 1;
        trie = (trie_ptr)(next_node->value);
    }

    /* we're at the leaf, so copy the value into the structure */
    trie->value = (uchr*)(malloc((wcslen(value) + 1) * sizeof(uchr)));
    wcscpy(trie->value, value);
    return;
}

uchr* trie_search(trie_ptr trie, uchr *key) {
    uchr next_letter;
    rb_node_ptr next_node;
    int key_length;

    key_length = wcslen(key);

    while (key_length > 0) {
        /* we're not at the leaf, so proceed down the tree */
        next_letter = *key;
        next_node = rb_tree_search(trie->children, next_letter);

        if (next_node == trie->children->nil || next_node->value == NULL) {
            return NULL;
        }

        key += 1;
        key_length -= 1;
        trie = (trie_ptr)(next_node->value);
    }

    /* we're at the leaf, so return the value (null or otherwise) */
    return trie->value;
}