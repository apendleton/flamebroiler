#include <locale.h>
#include <stdio.h>
#include <wchar.h>

#include <rbt.h>
#include <trie.h>
#include <trieprint.h>

int main() {
    trie_ptr trie = create_trie();
    int i;
    dyn_array *matches;

    wchar_t *fruit[10];
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
        trie_insert(trie, fruit[i], fruit[i]);
    }

    print_dot(trie);
}