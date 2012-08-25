import cffi, os, platform
from fb_ffi import ffi as trie_ffi
from dyn_array import dyn_array_as_list
from UserDict import DictMixin

libtype = "dylib" if platform.uname()[0] == "Darwin" else "so"

trie_ffi.cdef("void* create_trie();")
trie_ffi.cdef("void free_trie(void *trie);")
trie_ffi.cdef("void trie_insert(void* trie, wchar_t *key, wchar_t *value);")
trie_ffi.cdef("wchar_t* trie_search(void* trie, wchar_t *key);")
trie_ffi.cdef("void print_dot(void* trie);")
trie_ffi.cdef("dyn_array *trie_suffixes(void* trie, wchar_t *key, int strict, int max_matches);")
trie_ffi.cdef("void dyn_array_free(dyn_array* darr);")
libtrie = trie_ffi.dlopen(os.path.join(os.path.dirname(os.path.abspath(__file__)), "libtrie.%s" % libtype))
libtrieprint = trie_ffi.dlopen(os.path.join(os.path.dirname(os.path.abspath(__file__)), "libtrieprint.%s" % libtype))

class Trie(DictMixin):
    def __init__(self):
        self._trie = libtrie.create_trie()

    def __setitem__(self, key, value):
        ukey = unicode(key)
        uval = unicode(value)
        libtrie.trie_insert(self._trie, ukey, uval)

    def __getptr__(self, key):
        ukey = unicode(key)
        return libtrie.trie_search(self._trie, ukey)

    def __contains__(self, key):
        return self.__getptr__(key) != trie_ffi.NULL

    def __getitem__(self, key):
        value = self.__getptr__(key)
        if value == trie_ffi.NULL:
            raise KeyError(key)
        else:
            return trie_ffi.string(value)

    def print_dot(self):
        libtrieprint.print_dot(self._trie)

    def suffixes(self, key, strict=False, max_matches=0):
        ukey = unicode(key)
        matches_uta = libtrie.trie_suffixes(self._trie, ukey, int(strict), max_matches)
        out = dyn_array_as_list(matches_uta)
        libtrie.dyn_array_free(matches_uta)

        return out