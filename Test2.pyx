cimport Test1

cdef test2():
    print("Do Something In Test 2")

cdef runstuff():
    Test1.test1()
    test2()


def main():
    runstuff()    