#include <stdlib.h>

#include "rbt.h"

rb_tree_ptr create_rb_tree() {
    rb_tree_ptr out = (rb_tree_ptr)(malloc(sizeof(rb_tree_t)));
    rb_node_ptr nil = (rb_node_ptr)(malloc(sizeof(rb_node_t)));
    nil->is_red = 0;
    nil->key = 0;
    out->nil = nil;
    out->root = nil;
    return out;
}

void free_rb_tree(rb_tree_ptr tree, void(*free_value)(void *)) {
    if (tree->root != tree->nil) {
        free_rb_node(tree, tree->root, free_value);
    }
    free(tree->nil);
    free(tree);
}

void free_rb_node(rb_tree_ptr tree, rb_node_ptr node, void(*free_value)(void *)) {
    if (free_value != NULL & node->value != NULL) {
        free_value(node->value);
    }

    if (node->left != tree->nil) {
        free_rb_node(tree, node->left, free_value);
    }

    if (node->right != tree->nil) {
        free_rb_node(tree, node->right, free_value);
    }

    free(node);
}

rb_node_ptr rb_tree_insert(rb_tree_ptr tree, wchar_t key, void* value) {
    rb_node_ptr x, y, z;
    y = tree->nil;
    x = tree->root;
    while (x != tree->nil) {
        y = x;
        if (key < x->key) {
            x = x->left;
        } else if (key > x->key) {
            x = x->right;
        } else {
            /* key == x->key, so we don't need to create a new node */
            return x;
        }
    }

    /* we do need to create a new node */
    z = (rb_node_ptr)(malloc(sizeof(rb_node_t)));
    z->key = key;
    z->value = value;
    z->parent = y;
    if (y == tree->nil) {
        tree->root = z;
    } else if (key < y->key) {
        y->left = z;
    } else {
        y->right = z;
    }
    z->left = tree->nil;
    z->right = tree->nil;
    z->is_red = 1;

    rb_tree_insert_fixup(tree, z);

    return z;
}

void rb_tree_insert_fixup(rb_tree_ptr tree, rb_node_ptr node) {
    rb_node_ptr y;
    while (node->parent->is_red == 1) {
        if (node->parent == node->parent->parent->left) {
            y = node->parent->parent->right;
            if (y->is_red == 1) {
                node->parent->is_red = 0;
                y->is_red = 0;
                node->parent->parent->is_red = 1;
                node = node->parent->parent;
            } else {
                if (node == node->parent->right) {
                    node = node->parent;
                    rb_tree_left_rotate(tree, node);
                }
                node->parent->is_red = 0;
                node->parent->parent->is_red = 1;
                rb_tree_right_rotate(tree, node->parent->parent);
            }
        } else {
            y = node->parent->parent->left;
            if (y->is_red == 1) {
                node->parent->is_red = 0;
                y->is_red = 0;
                node->parent->parent->is_red = 1;
                node = node->parent->parent;
            } else {
                if (node == node->parent->left) {
                    node = node->parent;
                    rb_tree_right_rotate(tree, node);
                }
                node->parent->is_red = 0;
                node->parent->parent->is_red = 1;
                rb_tree_left_rotate(tree, node->parent->parent);
            }
        }
    }
    tree->root->is_red = 0;
}

void rb_tree_left_rotate(rb_tree_ptr tree, rb_node_ptr x) {
    rb_node_ptr y;
    y = x->right;
    x->right = y->left;
    if (y->left != tree->nil) {
        y->left->parent = x;
    }
    y->parent = x->parent;

    if (x->parent == tree->nil) {
        tree->root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }

    y->left = x;
    x->parent = y;
}

void rb_tree_right_rotate(rb_tree_ptr tree, rb_node_ptr y) {
    rb_node_ptr x;
    x = y->left;
    y->left = x->right;
    if (x->right != tree->nil) {
        x->right->parent = y;
    }
    x->parent = y->parent;

    if (y->parent == tree->nil) {
        tree->root = x;
    } else if (y == y->parent->right) {
        y->parent->right = x;
    } else {
        y->parent->left = x;
    }

    x->right = y;
    y->parent = x;
}

rb_node_ptr rb_tree_search(rb_tree_ptr tree, wchar_t key) {
    rb_node_ptr x;
    x = tree->root;
    while (x != tree->nil && x->key != key) {
        if (key < x->key) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    return x;
}