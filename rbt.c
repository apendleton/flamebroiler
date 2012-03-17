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

void free_rb_tree(rb_tree_ptr tree) {
    if (tree->root != tree->nil) {
        free_rb_node(tree, tree->root);
    }
    free(tree->nil);
    free(tree);
}

void free_rb_node(rb_tree_ptr tree, rb_node_ptr node) {
    if (node->left != tree->nil) {
        free_rb_node(tree, node->left);
    }

    if (node->right != tree->nil) {
        free_rb_node(tree, node->right);
    }

    free(node);
}

void rb_tree_insert(rb_tree_ptr tree, uchr key, void* value) {
    rb_node_ptr node = (rb_node_ptr)(malloc(sizeof(rb_node_t)));
    node->key = key;
    node->value = value;

    rb_tree_insert_node(tree, node);
}

void rb_tree_insert_node(rb_tree_ptr tree, rb_node_ptr node) {
    rb_node_ptr x, y;
    y = tree->nil;
    x = tree->root;
    while (x != tree->nil) {
        y = x;
        if (node->key < x->key) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    node->parent = y;
    if (y == tree->nil) {
        tree->root = node;
    } else if (node->key < y->key) {
        y->left = node;
    } else {
        y->right = node;
    }
    node->left = tree->nil;
    node->right = tree->nil;
    node->is_red = 1;

    rb_tree_insert_fixup(tree, node);
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