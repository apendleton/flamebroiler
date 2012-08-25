from flamebroiler import Trie
import resource
import timeit

def find_words():
    letters = [chr(i) for i in range(97,123)]
    for first in letters:
        for second in letters:
            print first + second
            matches = words.suffixes(first + second)

print resource.getrusage(resource.RUSAGE_SELF).ru_maxrss
words = Trie()
for w in open("/usr/share/dict/words"):
    word = w.strip().lower()
    words[word] = word

print resource.getrusage(resource.RUSAGE_SELF).ru_maxrss

print timeit.Timer("find_words()", "from __main__ import find_words; gc.enable()").repeat(10, 1)

print resource.getrusage(resource.RUSAGE_SELF).ru_maxrss
