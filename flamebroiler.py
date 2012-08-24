import cffi, os, platform
libtype = "dylib" if platform.uname()[0] == "Darwin" else "so"

trie_ffi = cffi.FFI()
trie_ffi.cdef("void* create_trie();")
trie_ffi.cdef("void free_trie(void *trie);")
trie_ffi.cdef("void trie_insert(void* trie, wchar_t *key, wchar_t *value);")
trie_ffi.cdef("void print_dot(void* trie);")
libtrie = trie_ffi.dlopen(os.path.join(os.path.dirname(os.path.abspath(__file__)), "libtrie.%s" % libtype))
libtrieprint = trie_ffi.dlopen(os.path.join(os.path.dirname(os.path.abspath(__file__)), "libtrieprint.%s" % libtype))

class Trie(object):
    def __init__(self):
        self._trie = libtrie.create_trie()

    def __setitem__(self, key, value):
        ukey = unicode(key)
        uval = unicode(value)
        libtrie.trie_insert(self._trie, ukey, uval)

    def print_dot(self):
        libtrieprint.print_dot(self._trie)