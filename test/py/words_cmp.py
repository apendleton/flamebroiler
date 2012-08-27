from flamebroiler import Trie
import resource
import timeit

def cmp_words():
    letters = [unichr(i) for i in range(97,123)]
    for first in letters:
        for second in letters:
            print first + second
            d_match = set([words_d[k] for k in words_d.keys() if k.startswith(first + second)])
            t_match = set(words_t.suffixes(first + second))
            if t_match == d_match:
                print 'yes'
            else:
                print "only in trie (of %s)" % len(t_match)
                print t_match.difference(d_match)
                
                print "only in dict (of %s)" % len(d_match)
                print d_match.difference(t_match)

print resource.getrusage(resource.RUSAGE_SELF).ru_maxrss
words_t = Trie()
words_d = {}
for w in open("/usr/share/dict/words"):
    word = unicode(w.strip().lower())
    print word
    words_t[word] = word
    words_d[word] = word

cmp_words()