#include <wchar.h>

typedef wchar_t uchr;

typedef struct rb_node {
    uchr key;
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
void free_rb_tree(rb_tree_ptr tree);
void free_rb_node(rb_tree_ptr tree, rb_node_ptr node);

void rb_tree_insert(rb_tree_ptr tree, uchr key, void* value);
void rb_tree_insert_node(rb_tree_ptr tree, rb_node_ptr node);
void rb_tree_insert_fixup(rb_tree_ptr tree, rb_node_ptr node);
void rb_tree_left_rotate(rb_tree_ptr tree, rb_node_ptr x);
void rb_tree_right_rotate(rb_tree_ptr tree, rb_node_ptr y);