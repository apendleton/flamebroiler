import cffi, os, platform
from UserDict import DictMixin

libtype = "dylib" if platform.uname()[0] == "Darwin" else "so"

trie_ffi = cffi.FFI()
trie_ffi.cdef("void* create_trie();")
trie_ffi.cdef("void free_trie(void *trie);")
trie_ffi.cdef("void trie_insert(void* trie, wchar_t *key, wchar_t *value);")
trie_ffi.cdef("wchar_t* trie_search(void* trie, wchar_t *key);")
trie_ffi.cdef("void print_dot(void* trie);")
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