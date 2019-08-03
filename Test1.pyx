# https://cython.readthedocs.io/en/latest/src/userguide/sharing_declarations.html
cdef test1():
    print("Do Something In Test 1")

def main():
    test1()
    