from flamebroiler import Trie
import resource
import timeit
import sys
import os
import time

words = Trie()
s = set()
for w in open("/usr/share/dict/words"):
    word = w.strip().lower()
    if word.startswith('ae'):
        s.add(word)

        if word == "aeolist":
            words.print_dot()
        
        words[word] = word
        
        if word == "aeolist":
            words.print_dot()
        if word == "aeolistic":
            words.print_dot()

"""
if len(sys.argv) > 1:
    words.print_dot()
else:
    suffixes = words.suffixes("ae")
    print len(suffixes), len(s)
    print s.difference(set(suffixes))
"""