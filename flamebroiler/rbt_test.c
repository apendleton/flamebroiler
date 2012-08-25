#include <locale.h>
#include <stdio.h>

#include "rbt.h"

void print_dot_node(rb_tree_ptr tree, rb_node_ptr node) {
    wprintf(L"  %lc [label=\"%lc\", color=\"", node->key, node->key);
    if (node->is_red) {
        wprintf(L"red");
    } else {
        wprintf(L"black");
    }
    wprintf(L"\"];\n");
    if (node->left != tree->nil) {
        print_dot_node(tree, node->left);
        wprintf(L"  %lc -> %lc ;\n", node->key, node->left->key);
    }
    if (node->right != tree->nil) {
        print_dot_node(tree, node->right);
        wprintf(L"  %lc -> %lc ;\n", node->key, node->right->key);
    }
}
void print_dot(rb_tree_ptr tree) {
    wprintf(L"digraph red_black_tree {\n");

    if (tree->root != tree->nil) {
        print_dot_node(tree, tree->root);
    }

    wprintf(L"}\n");
}

int main() {
    int i;
    wchar_t n;
    rb_tree_ptr tree = create_rb_tree();
    
    n = L'a';
    for (i = 0; i < 20; i++) {
        rb_tree_insert(tree, n + i, NULL);
    }
    for (i = 10; i < 26; i++) {
        rb_tree_insert(tree, n + i, NULL);
    }

    print_dot(tree);

    free_rb_tree(tree);

    return 0;
}