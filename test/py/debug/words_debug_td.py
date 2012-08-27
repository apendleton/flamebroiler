from flamebroiler import Trie
import resource
import timeit
import sys
import os
import time

words_t = Trie()
words_d = {}
for w in open("/usr/share/dict/words"):
    word = unicode(w.strip().lower())
    words_t[word] = word
    words_d[word] = word

for pair in ["ab", "td", "ym", "yr"]:
    print pair
    d_match = set([words_d[k] for k in words_d.keys() if k.startswith(pair)])
    t_match = set(words_t.suffixes(pair))
    if t_match == d_match:
        print 'yes'
    else:
        print "only in trie (of %s)" % len(t_match)
        print t_match.difference(d_match)
        
        print "only in dict (of %s)" % len(d_match)
        print d_match.difference(t_match)