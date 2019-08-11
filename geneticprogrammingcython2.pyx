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
cdef long crandint(int lower, int upper) except -1:
    return (rand() % (upper - lower + 1)) + lower


# node parenet
cdef class node:
    cdef Py_ssize_t node_type
    def __cinit__(self):
        pass

    # TODO: Can python call a super class method?
    cdef long evaluate(self, list inp):
        super.evaluate(inp)


    def display(self, indent=0):
        super.display(indent)




# node objects
cdef class functnode(node):
    cdef str name
    cdef object function
    cdef list children
    cdef bint lock
    cdef long value
    cdef Py_ssize_t param_num
    cdef Py_ssize_t size

    def __cinit__(self, long value, list children=None):
        self.setnode(value, children)

    @cython.boundscheck(False)
    @cython.wraparound(False)
    @cython.nonecheck(False)
    cdef void setnode(self, long value, list children=None):
        self.node_type = FUNC_NODE
        self.value = value
        self.lock = False
        self.function = func_array[self.value]
        self.name = name_array[self.value]
        self.param_num = param_array[self.value]
        self.size = len(children)
        self.children = children
        # for i in range(self.size):
        #     self.children[i] = <node>children[i]


    @cython.boundscheck(False)
    @cython.wraparound(False)
    @cython.nonecheck(False)
    cdef long evaluate(self, list inp):
        cdef int size
        cdef long results[3]
        cdef object t
        for i in range(self.size):
            results[i] = (<node>(self.children[i])).evaluate(inp)

        if self.size == 1:
            return self.function(results[0])
        elif self.size == 2:
            return self.function(results[0], results[1])
        elif self.size == 3:
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


cdef class paramnode(node):
    cdef Py_ssize_t idx

    def __cinit__(self, int idx):
        self.idx = idx
        self.node_type = PARAM_NODE

    @cython.boundscheck(False)
    @cython.wraparound(False)
    @cython.nonecheck(False)
    cdef long evaluate(self, list inp):
        return inp[self.idx]

    def display(self, indent=0):
            print('%sp%d' % (' ' * indent, self.value))


cdef class constnode(node):
    cdef long value

    def __cinit__(self, long value):
        self.value = value
        self.node_type = CONST_NODE


    @cython.boundscheck(False)
    @cython.wraparound(False)
    @cython.nonecheck(False)
    cdef long evaluate(self, list inp):
        return self.value

    def display(self, indent=0):
            print('%s%d' % (' ' * indent, self.value))  






# @cython.boundscheck(False)
# @cython.wraparound(False)
cdef node makerandomtree(int param_count, int maxdepth=4, float func_prob=0.5, float param_prob=0.6):
    cdef list children = []
    cdef Py_ssize_t i
    if crandom() < func_prob and maxdepth > 0:
        func_num = crandint(0, numfuncs-1)
        for i in range(param_array[func_num]):
            tree = makerandomtree(param_count, maxdepth - 1, func_prob, param_prob)
            children.append(tree)            
        return functnode.__new__(functnode, func_num, children) #functnode(func_num, children)
    elif crandom() < param_prob:
        # newnode.setnode(PARAM_NODE, crandint(0, param_count - 1))
        # return newnode
        return paramnode.__new__(paramnode, crandint(0, param_count - 1)) #paramnode(crandint(0, param_count - 1))
    else:
        return constnode.__new__(constnode, crandint(0, 10)) #constnode(crandint(0, 10))


def runexperiment():
    srand(10)
    tree = makerandomtree(2)
    tree.display()
    print(tree.evaluate([5,2]))


cdef timeit():
    runs = 5000000

    # 20.821658849716187, 2.0441694259643555; 16.92554783821106, 2.061018705368042; 17.349987030029297, 2.0460333824157715
    start = time.time()
    population = []
    input = [10, 42]
    for i in range(runs):
        population.append(makerandomtree(2))
    mid = time.time()
    for tree in population:
        (<node>tree).evaluate(input)
    end = time.time()
    print("Benchmark (geneticprogrammingcython2.py):  ", mid-start, end-mid)


    # # 32.76694297790527, 5.336438894271851; 34.19974899291992, 5.901979446411133; 31.23992419242859, 5.255947113037109
    # start = time.time()
    # population = []
    # input = [10, 42]
    # for i in range(runs):
    #     population.append(gpc2.makerandomtree(2))
    # mid = time.time()
    # for tree in population:
    #     gpc2.evaluate(tree, input)

    # end = time.time()
    # print("Benchmark (geneticprogrammingcython.py):  ", mid-start, end-mid)



    # start = time.time()
    # population = []
    # input = [10, 42]
    # for i in range(runs):
    #     population.append(gpc1.makerandomtree(2))
    # mid = time.time()
    # for tree in population:
    #     tree.evaluate(input)
    # end = time.time()
    # print("Benchmark (gpcython.py): ", mid-start, end-mid)



    # start = time.time()
    # population = []
    # input = [10, 42]
    # for i in range(runs):
    #     population.append(gp.makerandomtree(2))
    # mid = time.time()
    # for tree in population:
    #     tree.evaluate(input)

    # end = time.time()
    # print("Benchmark (gp.py): ", mid-start, end-mid)


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
    # print(tree.evaluate([5, 2]))
    # assert evaluate(tree, [5, 2]) == 36
    assert tree.evaluate([5,2]) == 36

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