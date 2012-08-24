import cffi, os, platform
libtype = "dylib" if platform.uname()[0] == "Darwin" else "so"

ffi = cffi.FFI()
ffi.cdef("void* create_trie();")
ffi.cdef("void free_trie(void *trie);")
ffi.cdef("void trie_insert(void* trie, wchar_t *key, int key_length, wchar_t *value);")
ffi.cdef("void print_dot(void* trie);")
libtrie = ffi.dlopen(os.path.join(os.path.dirname(os.path.abspath(__file__)), "libtrie.%s" % libtype))
libtrieprint = ffi.dlopen(os.path.join(os.path.dirname(os.path.abspath(__file__)), "libtrieprint.%s" % libtype))

trie = libtrie.create_trie()
for fruit in ["apple", "peach", "pear", "mango", "lemon", "lime", "applesauce", "pineapple", "kiwi", "passionfruit"]:
    uf = unicode(fruit)
    libtrie.trie_insert(trie, uf, len(uf), uf)

libtrieprint.print_dot(trie)
libtrie.free_trie(trie)