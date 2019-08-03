import numpy as np
cimport numpy as np
import random
import time
import gp
cimport cython
import gpcython as gpc1
import geneticprogramming as gp2
from libc.stdlib cimport rand, RAND_MAX, srand
import geneticprogrammingcython as gpc2
cimport geneticprogrammingcython as gpc2




# How do turn on c-division and turn off bound checking for whole file (if at top of file)
#!python
#cython: boundscheck=False
#cython: cdivision=True
# How to do it locally using decorators and with statement: https://github.com/cython/cython/wiki/enhancements-compilerdirectives

# Enum for TYPE#
# ctypedef enum NodeType:
#     FUNC_NODE = 1
#     PARAM_NODE = 2
#     CONST_NODE = 3

cdef Py_ssize_t FUNC_NODE = 1, PARAM_NODE = 2, CONST_NODE = 3


# Node format
# [TYPE #, Function #, value or param indx, child1, child2, child3, lock, node id]
# Structured Arrays: https://docs.scipy.org/doc/numpy/user/basics.rec.html
# Create dtype array: https://docs.scipy.org/doc/numpy/reference/generated/numpy.dtype.html#numpy.dtype

# Using NumPy and Cython: http://docs.cython.org/en/latest/src/userguide/numpy_tutorial.html
# Memoryviews: http://docs.cython.org/en/latest/src/userguide/memoryviews.html#memoryviews
# C-contiguous arrays: https://stackoverflow.com/questions/26998223/what-is-the-difference-between-contiguous-and-non-contiguous-arrays
# timeit: https://docs.python.org/3/library/timeit.html

# Functions

#Fake Enum for Function List Columns
# ctypedef enum FunctionListCols:
#     FUNCTION = 0
#     PARAM_COUNT = 1
#     NAME = 2

cdef Py_ssize_t FUNCTION = 0, PARAM_COUNT = 1, NAME = 2


# CONSTANTS
cdef int MAX_PARAMS = 3

# Function Arrays
ctypedef long (*func2param)(long param1, long param2)
ctypedef long (*funct3param)(long param1, long param2, long param3)
cdef int param_array[5]
cdef list name_array = []
cdef list func_array = []
func_list = []

cdef list definefunction(function, param_count, name):    
    func_list.append((function, param_count, name))


# Functions with 2 parameters
cdef long add(long param1, long param2):
    return param1 + param2

definefunction(add, 2, 'add')

cdef long subtract(long param1, long param2):
    return param1 - param2

definefunction(subtract, 2, 'subtract')

cdef long multiply(long param1, long param2):
    return param1 * param2

definefunction(multiply, 2, 'multiply')

# > function

cdef long isgreater(long param1, long param2):
    if param1 > param2:
        return 1
    else:
        return 0

definefunction(isgreater, 2, 'isgreater')

# If Function (3 parameters)
cdef long iffunc(long param1, long param2, long param3):
    if param1 > 0:
        return param2
    else:
        return param3

definefunction(iffunc, 3, 'if')

# func_list = np.array(func_list)
cdef Py_ssize_t numfuncs = len(func_list)

# TODO: How to create a memory view to speed up numpy array: cdef int[:] func_list_cview = func_list. See NumPy Tutorial above. Except this won't work for the function array. I need a better way.
for i in range(5):
    param_array[i] = func_list[i][PARAM_COUNT]
    name_array.append(func_list[i][NAME])
    func_array.append(func_list[i][FUNCTION])


# Program Trees: Lists or Dynamic arrays that follow this format:
# [TYPE #, Function #, value or param indx, lock, node id, child1offset, child1length, child2offset, child2length, child3offset, child3length]
# TODO: Dynamic 2d arrays in Cython: https://stackoverflow.com/questions/25917593/how-to-declare-2d-c-arrays-dynamically-in-cython
# TODO: Or use memoryviews (see above)

#Fake Enum for Column Names
# ctypedef enum ColumnNames:
#     TYPE_COL = 0
#     FUNC_NUM = 1
#     VALUE_OR_PARAM = 2
#     LOCK = 3
#     ID = 4
#     CHILD1OFFSET = 5
#     CHILDOFFSETS = CHILD1OFFSET
#     CHILD1LENGTH = 6
#     CHILD2OFFSET = 7
#     CHILD2LENGTH = 8
#     CHILD3OFFSET = 9
#     CHILD3LENGTH = 10
#     NUM_COLS = CHILD3LENGTH + 1

cdef Py_ssize_t TYPE_COL = 0,FUNC_NUM = 1,VALUE_OR_PARAM = 2,LOCK = 3,ID = 4,CHILD1OFFSET = 5,CHILDOFFSETS = CHILD1OFFSET,CHILD1LENGTH = 6,CHILD2OFFSET = 7,CHILD2LENGTH = 8,CHILD3OFFSET = 9,CHILD3LENGTH = 10,NUM_COLS = CHILD3LENGTH + 1

cdef long treecounter = 0
cdef long nodes = 0

@cython.cdivision(True)
cdef float crandom() except -1: 
    return  <float>rand() / <float>RAND_MAX

@cython.cdivision(True)
cdef int crandint(int lower, int upper) except -1:
    return (rand() % (upper - lower + 1)) + lower



# node objects
cdef class node:
    cdef public object function
    cdef public object name
    cdef public object children
    cdef int func_num
    cdef int lock
    cdef int param_num

    def __init__(self, int func_num, object children):
        self.func_num = func_num
        self.function = func_array[func_num]
        self.name = name_array[func_num]
        self.param_num = param_array[func_num]
        self.children = children
        self.lock = False


    cpdef double evaluate(self, list inp):
        cdef int size 
        cdef double results[3]
        size = len(self.children)
        for i in range(size):
            results[i] = self.children[i].evaluate(inp)

        if size == 1:
            return self.function(results[0])
        elif size == 2:
            return self.function(results[0], results[1])
        elif size == 3:
            return self.function(results[0], results[1], results[2])
        # TODO: fix to use return self.function(*results)


    def display(self, indent=0):
        if self.lock:
            add = "*"
        else:
            add = ""
        name = str(self.name)
        print( (' ' * indent) + name + add)
        for c in self.children:
            c.display(indent + 1)


cdef class paramnode:
    cdef int idx

    def __init__(self, int idx):
        self.idx = idx

    cpdef double evaluate(self, list inp):
        return inp[self.idx]

    def display(self, indent=0):
        print('%sp%d' % (' ' * indent, self.idx))


cdef class constnode:
    cdef double value

    def __init__(self, double value):
        self.value = value

    cpdef double evaluate(self, list inp):
        return self.value

    def display(self, indent=0):
        print('%s%d' % (' ' * indent, self.value))







# @cython.boundscheck(False)
# @cython.wraparound(False)
cdef object makerandomtree(int param_count, int maxdepth=4, float func_prob=0.5, float param_prob=0.6):
    cdef list children = []
    cdef Py_ssize_t i
    if crandom() < func_prob and maxdepth > 0:
        func_num = crandint(0, numfuncs-1)
        for i in range(param_array[func_num]):
            tree = makerandomtree(param_count, maxdepth - 1, func_prob, param_prob)
            children.append(tree)
        return node(func_num, children)
    elif crandom() < param_prob:
        return paramnode(crandint(0, param_count - 1))
    else:
        return constnode(crandint(0, 10))



def runexperiment():
    tree = makerandomtree(2)
    tree.display()
    print(tree.evaluate([5,2]))


cdef timeit():
    runs = 250000

    start = time.time()
    population = []
    input = [10, 42]
    for i in range(runs):
        population.append(makerandomtree(2))
    mid = time.time()
    for tree in population:
        tree.evaluate(input)
    end = time.time()
    print("Benchmark (geneticprogrammingcython2.py):  ", mid-start, end-mid)


    start = time.time()
    population = []
    input = [10, 42]
    for i in range(runs):
        population.append(gpc2.makerandomtree(2))
    mid = time.time()
    for tree in population:
        gpc2.evaluate(tree, input)

    end = time.time()
    print("Benchmark (geneticprogrammingcython.py):  ", mid-start, end-mid)



    start = time.time()
    population = []
    input = [10, 42]
    for i in range(runs):
        population.append(gpc1.makerandomtree(2))
    mid = time.time()
    for tree in population:
        tree.evaluate(input)
    end = time.time()
    print("Benchmark (gpcython.py): ", mid-start, end-mid)



    start = time.time()
    population = []
    input = [10, 42]
    for i in range(runs):
        population.append(gp.makerandomtree(2))
    mid = time.time()
    for tree in population:
        tree.evaluate(input)

    end = time.time()
    print("Benchmark (gp.py): ", mid-start, end-mid)


    # start = time.time()
    # population = []
    # input = [10, 42]
    # for i in range(runs):
    #     population.append(gp2.makerandomtree(2))
    # mid = time.time()
    # for tree in population:
    #     gp2.evaluate(tree, input)

    # end = time.time()
    # print("Benchmark (geneticprogramming.py):  ", mid-start, end-mid)


def test_evaluate():
    tree = [[1, 2, -1,  0,  8,  1,  1,  2,  7, -1, -1], # Multiply
            [3, -1,  7,  0,  0, -1, -1, -1, -1, -1, -1],
            [1,  1, -1,  0,  7,  1,  5,  6,  1, -1, -1], # Subtract
            [1,  2, -1,  0,  5,  1,  3,  4,  1, -1, -1], # Multiply
            [1,  0, -1,  0,  3,  1,  1,  2,  1, -1, -1], # Add
            [3, -1,  2,  0,  1, -1, -1, -1, -1, -1, -1],
            [2, -1,  0,  0,  2, -1, -1, -1, -1, -1, -1],
            [2, -1,  1,  0,  4, -1, -1, -1, -1, -1, -1],
            [3, -1, 10,  0,  6, -1, -1, -1, -1, -1, -1]]


    input = [5,2]
    assert tree.evaluate(input) == 28
    input = [2,5]
    assert tree.evaluate(input) == 70

    tree = [[1,  4, -1, 0, 20,  1,  7,  8,  1,  9,  1], # IF
            [1,  0, -1, 0, 17,  1,  5,  6,  1, -1, -1], # Add
            [1,  3, -1, 0, 15,  1,  1,  2,  3, -1, -1], # >
            [2, -1,  0, 0, 11, -1, -1, -1, -1, -1, -1],
            [1,  2, -1, 0, 14,  1,  1,  2,  1, -1, -1], # Multiply
            [3, -1,  7, 0, 12, -1, -1, -1, -1, -1, -1],
            [2, -1,  0, 0, 13, -1, -1, -1, -1, -1, -1],
            [2, -1,  1, 0, 16, -1, -1, -1, -1, -1, -1],
            [2, -1,  1, 0, 18, -1, -1, -1, -1, -1, -1],
            [2, -1,  0, 0, 19, -1, -1, -1, -1, -1, -1]]

    input = [5,2]
    assert tree.evaluate(input) == 2
    input = [2,5]
    assert tree.evaluate(input) == 5
    print("Pass test_evaluate")


cdef test_makerandomtree():
    srand(10)
    tree = makerandomtree(2)
    # print(treearray.evaluate([5, 2]))
    assert tree.evaluate([5, 2]) == 36

    tree = makerandomtree(2)
    # print(treearray.evaluate([100, 200]))
    assert tree.evaluate([100, 200]) == 5

    tree = makerandomtree(3)
    # print(treearray.evaluate([1, 2, 3]))
    assert tree.evaluate([1, 2, 3]) == 2

    tree = makerandomtree(4)
    # print(treearray.evaluate([4, 3, 2, 1]))
    assert tree.evaluate([4, 3, 2, 1]) == 0

    tree = makerandomtree(5)
    # print(treearray.evaluate([2, 4, 6, 8, 10]))   
    assert tree.evaluate([2, 4, 6, 8, 10]) == 8
    
    print("Pass test_makerandomtree")



def main():
    # runexperiment()
    # test_evaluate()
    test_makerandomtree()
    timeit()

if __name__ == "__main__":
    main()