#include <stdlib.h>
#include <locale.h>
#include <wchar.h>
#include <stdbool.h>

#include "rbt.h"
#include "trie.h"

trie_ptr create_trie() {
    trie_ptr out = (trie_ptr)(malloc(sizeof(trie_t)));
    out->value = NULL;
    out->compact = true;
    out->child_data = NULL;
    return out;
}

void free_trie(void *trie) {
    trie_ptr _trie = (trie_ptr)(trie);
    if (_trie->compact) {
        if (_trie->child_data != NULL) free(_trie->child_data);
    } else {
        free_rb_tree((rb_tree_ptr)(_trie->child_data), &free_trie);
    }
    free(_trie->value);
    free(_trie);
}

void expand_trie_node(trie_ptr trie, int length) {
    wchar_t *old_child_text, *new_child_text;
    trie_ptr child_trie;
    rb_tree_ptr rbt;
    rb_node_ptr next_node;

    old_child_text = (wchar_t*)(trie->child_data);
    
    rbt = create_rb_tree();
    trie->child_data = (void*)(rbt);

    if (wcslen(old_child_text) > 0) {
        next_node = rb_tree_insert(rbt, old_child_text[0], NULL);
        child_trie = create_trie();
        next_node->value = (void*)(child_trie);

        new_child_text = (wchar_t*)(malloc(length * sizeof(wchar_t)));
        wcscpy(new_child_text, old_child_text + 1);
        child_trie->child_data = (void*)(new_child_text);
        child_trie->value = trie->value;
        trie->value = NULL;
    }

    free(old_child_text);

    trie->compact = false;
}

void trie_insert(trie_ptr trie, wchar_t *key, wchar_t *value) {
    wchar_t next_letter;
    rb_node_ptr next_node;
    int key_length;
    wchar_t* child_text;

    key_length = wcslen(key);

    while (key_length > 0) {
        /* are we on a compact node? */
        if (trie->compact) {
            if (trie->child_data == NULL) {
                /* if we're on a compact trie node without key text, we can stop */
                child_text = (wchar_t*)(malloc((key_length + 1) * sizeof(wchar_t)));
                wcscpy(child_text, key);
                trie->child_data = (void*)(child_text);
                break;
            } else {
                /* we need to expand this node and proceed */
                expand_trie_node(trie, key_length);
            }
        }

        /* we're not at the leaf, so proceed down the tree */
        next_letter = *key;
        next_node = rb_tree_insert((rb_tree_ptr)(trie->child_data), next_letter, NULL);

        if (next_node->value == NULL) {
            next_node->value = (void *)(create_trie());
        }

        key += 1;
        key_length -= 1;
        trie = (trie_ptr)(next_node->value);
    }

    /* we're at the leaf, so copy the value into the structure */
    if (trie->value != NULL) free(trie->value);
    trie->value = (wchar_t*)(malloc((wcslen(value) + 1) * sizeof(wchar_t)));
    wcscpy(trie->value, value);
    return;
}

trie_search_result trie_node_search(trie_ptr trie, wchar_t *key) {
    wchar_t next_letter;
    rb_node_ptr next_node;
    int key_length;
    trie_search_result result;
    
    result.is_exact = true;
    result.trie = NULL;

    key_length = wcslen(key);

    while (key_length >= 0) {
        /* are we on a compact node? */
        if (trie->compact) {
            if (trie->child_data == NULL) {
                /* we're not going to find it */
                return result;
            } else {
                /* if the key matches or begins the compact leaf, we're good */
                if (wcsncmp(trie->child_data, key, key_length) == 0) {
                    if (wcslen(trie->child_data) != key_length) {
                        result.is_exact = false;
                    }
                    break;
                } else {
                    return result;
                }
            }
        }
        if (key_length == 0) break;

        /* we're not at the leaf, so proceed down the tree */
        next_letter = *key;
        next_node = rb_tree_search((rb_tree_ptr)(trie->child_data), next_letter);

        if (next_node == ((rb_tree_ptr)(trie->child_data))->nil || next_node->value == NULL) {
            return result;
        }

        key += 1;
        key_length -= 1;
        trie = (trie_ptr)(next_node->value);
    }

    /* we're at the leaf, so return it */
    result.trie = trie;
    return result;
}

wchar_t* trie_search(trie_ptr trie, wchar_t* key) {
    trie_search_result match;

    match = trie_node_search(trie, key);
    if (match.trie == NULL || match.is_exact == false) {
        return NULL;
    } else {
        return match.trie->value;
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

    if (!(trie->compact)) {
        return trie_suffixes_rb_node_visit(((rb_tree_ptr)(trie->child_data))->root, ((rb_tree_ptr)(trie->child_data))->nil, max_matches, out);
    } else {
        return true;
    }
}

/* creates a structure that must be freed by caller! */
dyn_array *trie_suffixes(trie_ptr trie, wchar_t *key, bool strict, int max_matches) {
    dyn_array *out;
    trie_search_result match;

    out = dyn_array_new();

    /* first do a standard search to find the key */
    match = trie_node_search(trie, key);
    if (match.trie == NULL) {
        return out;
    }

    if (match.trie->compact) {
        if (strict && match.is_exact) {
            return out;
        } else {
            trie_suffixes_visit(match.trie, max_matches, out);
        }
    } else {
        if (match.trie->value != NULL && !strict) {
            trie_suffixes_visit(match.trie, max_matches, out);
        } else {
            trie_suffixes_rb_node_visit(((rb_tree_ptr)(match.trie->child_data))->root, ((rb_tree_ptr)(match.trie->child_data))->nil, max_matches, out);
        }
    }
    
    return out;
}