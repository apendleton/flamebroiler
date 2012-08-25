#include <stdlib.h>
#include <locale.h>
#include <wchar.h>
#include <stdbool.h>

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

void trie_insert(trie_ptr trie, wchar_t *key, wchar_t *value) {
    wchar_t next_letter;
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
    trie->value = (wchar_t*)(malloc((wcslen(value) + 1) * sizeof(wchar_t)));
    wcscpy(trie->value, value);
    return;
}

trie_ptr trie_node_search(trie_ptr trie, wchar_t *key) {
    wchar_t next_letter;
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

    /* we're at the leaf, so return it */
    return trie;
}

wchar_t* trie_search(trie_ptr trie, wchar_t* key) {
    trie_ptr match;

    match = trie_node_search(trie, key);
    if (match == NULL) {
        return NULL;
    } else {
        return trie->value;
    }
}

/* hacky dynamic array structure */
dyn_array* dyn_array_new() {
    dyn_array* out = (dyn_array*)(malloc(sizeof(dyn_array)));
    out->arr = (wchar_t**)(malloc(10 * sizeof(wchar_t*)));
    out->slots = 10;
    out->len = 0;
    return out;
}
dyn_array* dyn_array_add(dyn_array* darr, wchar_t* item) {
    if (darr->len == darr->slots) {
        darr->slots += 10;
        darr->arr = realloc(darr->arr, darr->slots * sizeof(wchar_t*));
    }
    darr->arr[(darr->len)++] = item;
}
void dyn_array_free(dyn_array* darr) {
    free(darr->arr);
    free(darr);
}


bool trie_suffixes_visit(trie_ptr trie, int max_matches, dyn_array *out);

bool trie_suffixes_rb_node_visit(rb_node_ptr node, rb_node_ptr nil, int max_matches, dyn_array *out) {
    int i;
    bool cont;
    if (node != nil) {
        cont = trie_suffixes_rb_node_visit(node->left, nil, max_matches, out);
        if (!cont) return cont;

        if (node->value != NULL) {
            cont = trie_suffixes_visit(node->value, max_matches, out);
            if (!cont) return cont;
        }

        cont = trie_suffixes_rb_node_visit(node->right, nil, max_matches, out);
        if (!cont) return cont;
    }
    return true;
}

bool trie_suffixes_visit(trie_ptr trie, int max_matches, dyn_array *out) {
    if (trie->value != NULL) {
        dyn_array_add(out, trie->value);
    }
    if (max_matches > 0 && out->len >= max_matches) {
        return false;
    }
    return trie_suffixes_rb_node_visit(trie->children->root, trie->children->nil, max_matches, out);
}

/* creates a structure that must be freed by caller! */
dyn_array *trie_suffixes(trie_ptr trie, wchar_t *key, bool strict, int max_matches) {
    dyn_array *out;
    trie_ptr match;

    out = dyn_array_new();

    /* first do a standard search to find the key */
    match = trie_node_search(trie, key);
    if (match == NULL) {
        return out;
    }

    if (match->value != NULL && !strict) {
        trie_suffixes_visit(match, max_matches, out);
    } else {
        trie_suffixes_rb_node_visit(match->children->root, match->children->nil, max_matches, out);
    }
    
    return out;
}