#include <wchar.h>

#ifndef RBT_H
#define RBT_H

typedef struct rb_node {
    wchar_t key;
    void *value;

    struct rb_node *parent;
    struct rb_node *left;
    struct rb_node *right;
    unsigned char is_red;
} rb_node_t, *rb_node_ptr;

typedef struct rb_tree {
    rb_node_ptr nil;
    rb_node_ptr root;
} rb_tree_t, *rb_tree_ptr;

rb_tree_ptr create_rb_tree();
void free_rb_tree(rb_tree_ptr tree, void(*free_value)(void *));
void free_rb_node(rb_tree_ptr tree, rb_node_ptr node, void(*free_value)(void *));

rb_node_ptr rb_tree_insert(rb_tree_ptr tree, wchar_t key, void* value);
void rb_tree_insert_fixup(rb_tree_ptr tree, rb_node_ptr node);
void rb_tree_left_rotate(rb_tree_ptr tree, rb_node_ptr x);
void rb_tree_right_rotate(rb_tree_ptr tree, rb_node_ptr y);
rb_node_ptr rb_tree_search(rb_tree_ptr tree, wchar_t key);

#endif