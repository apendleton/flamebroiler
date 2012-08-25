from flamebroiler import Trie

trie = Trie()
words = set((w.strip().lower() for w in open("/usr/share/dict/words")))
#for word in words:
#    trie[word] = word
