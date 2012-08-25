#include <locale.h>
#include <stdio.h>
#include <wchar.h>

#include "rbt.h"
#include "trie.h"

#ifndef TRIEPRINT_H
#define TRIEPRINT_H

void print_trie_node(trie_ptr trie);
void print_rbt_node(rb_tree_ptr tree, rb_node_ptr node);
void print_dot(trie_ptr trie);

#endif