#include <locale.h>
#include <stdio.h>
#include <wchar.h>

#include "rbt.h"
#include "trie.h"
#include "trieprint.h"

void print_rbt_node(rb_tree_ptr tree, rb_node_ptr node) {
    wprintf(L"  %lu [label=\"%lc\", color=\"", node, node->key);
    if (node->is_red) {
        wprintf(L"red");
    } else {
        wprintf(L"black");
    }
    wprintf(L"\"];\n");
    if (node->left != tree->nil) {
        print_rbt_node(tree, node->left);
        wprintf(L"  %lu -> %lu ;\n", node, node->left);
    }
    if (node->right != tree->nil) {
        print_rbt_node(tree, node->right);
        wprintf(L"  %lu -> %lu ;\n", node, node->right);
    }
    if (node->value != NULL) {
        print_trie_node((trie_ptr)(node->value));
        wprintf(L"  %lu -> %lu ;\n", node, node->value);
    }
}

void print_trie_node(trie_ptr trie) {
    rb_tree_ptr children;
    if (trie->compact) {
        wprintf(L"  %lu [label=\"%ls : %ls\", color=\"blue\"];\n", trie, trie->child_data == NULL ? L"[NONE]" : trie->child_data, trie->value == NULL ? L"[NONE]" : trie->value);
    } else {
        children = (rb_tree_ptr)(trie->child_data);
        wprintf(L"  %lu [label=\"%ls\", color=\"green\"];\n", trie, trie->value == NULL ? L"[NONE]" : trie->value);
        if (children->root != children->nil) {
            print_rbt_node(children, children->root);
            wprintf(L"  %lu -> %lu ;\n", trie, children->root);
        }
    }
}

void print_dot(trie_ptr trie) {
    wprintf(L"digraph trie {\n");

    print_trie_node(trie);

    wprintf(L"}\n");
}