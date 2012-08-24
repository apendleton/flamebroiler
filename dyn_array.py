import cffi, os, platform
from fb_ffi import ffi

ffi.cdef("""
    typedef struct {
    unsigned len, slots;
    wchar_t** arr;
} dyn_array;
""")

def dyn_array_as_list(arr):
    out = []
    for i in range(arr.len):
        out.append(ffi.string(arr.arr[i]))
    return out