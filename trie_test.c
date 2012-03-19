#include <locale.h>
#include <stdio.h>
#include <wchar.h>

#include "rbt.h"
#include "trie.h"

void print_trie_node(trie_ptr trie);

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
    wprintf(L"  %lu [label=\"%ls\", color=\"green\"]\n", trie, trie->value == NULL ? L"[NONE]" : trie->value);
    if (trie->children->root != trie->children->nil) {
        print_rbt_node(trie->children, trie->children->root);
        wprintf(L"  %lu -> %lu ;\n", trie, trie->children->root);
    }
}

void print_dot(trie_ptr trie) {
    wprintf(L"digraph trie {\n");

    print_trie_node(trie);

    wprintf(L"}\n");
}

int main() {
    trie_ptr trie = create_trie();
    int i;

    uchr *fruit[10];
    fruit[0] = L"apple";
    fruit[1] = L"peach";
    fruit[2] = L"pear";
    fruit[3] = L"mango";
    fruit[4] = L"lemon";
    fruit[5] = L"lime";
    fruit[6] = L"applesauce";
    fruit[7] = L"pineapple";
    fruit[8] = L"kiwi";
    fruit[9] = L"passionfruit";
    for (i = 0; i < 10; i++) {
        trie_insert(trie, fruit[i], wcslen(fruit[i]), fruit[i]);
    }

    print_dot(trie);

    free_trie(trie);

    return 0;
}