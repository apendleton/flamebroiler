from flamebroiler import Trie

trie = Trie()
for fruit in ["apple", "peach", "pear", "mango", "lemon", "lime", "applesauce", "pineapple", "kiwi", "passionfruit"]:
    trie[fruit] = fruit

trie.print_dot()