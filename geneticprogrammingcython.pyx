import numpy as np
cimport numpy as np
import random
import time
import gp
cimport cython
import gpcython as gpc1
import geneticprogramming as gp2


# How do turn on c-division and turn off bound checking for whole file (if at top of file)
#!python
#cython: boundscheck=False
#cython: cdivision=True
# How to do it locally using decorators and with statement: https://github.com/cython/cython/wiki/enhancements-compilerdirectives
#TODO: cimport cython

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
cdef void* func_array[5]
cdef int param_array[5]
cdef char* name_array[5]
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

func_list = np.array(func_list)
# TODO: How to create a memory view to speed up numpy array: cdef int[:] func_list_cview = func_list. See NumPy Tutorial above. Except this won't work for the function array. I need a better way.
for i in range(5):
    param_array[i] = func_list[i,PARAM_COUNT]
    # name_array[i] = func_list[i,NAME]
    # func_array[i] = func_list[i,FUNCTION]


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

@cython.boundscheck(False)
@cython.wraparound(False)
cdef list createtree(Py_ssize_t node_type, int funcnum, int val_or_param, bint lock, list children):
    # NumPy to C Array: https://github.com/cython/cython/wiki/tutorials-NumpyPointerToC
    # Numpy arrays as parameters: https://stackoverflow.com/questions/4641200/cython-inline-function-with-numpy-array-as-parameter
    # Get node id
    global nodes
    cdef int id = nodes
    cdef int index, param_count
    cdef Py_ssize_t position, size, i, j # cdef Py_ssize_t see http://docs.cython.org/en/latest/src/userguide/numpy_tutorial.html 
    cdef long[11] node = [node_type, funcnum, val_or_param, lock, id, -1, -1, -1, -1, -1, -1]
    cdef list tree = []

    nodes += 1
    # If this is a function, get number of parameters expected vs of children given
    if node_type == FUNC_NODE:
        param_count = param_array[funcnum]
        # parameter count and number of children in the list must match or else we have an error
        # assert param_count == len(children)
        index = 1
        position = 5
        for i in range(param_count):
            if i < len(children):
                child = children[i]
                node[position] = index
                position += 1
                size = len(child)
                node[position] = size
                position += 1
                index += size
                for j in range(size):
                    row = child[j]
                    tree.append(row)

    if len(tree) == 0:
        return [node]
    else:
        tree.insert(0, node)
        return tree

@cython.boundscheck(False)
@cython.wraparound(False)
cpdef list makerandomtree(int param_count, int maxdepth=4, float func_prob=0.5, float param_prob=0.6):
    cdef list children
    if random.random() < func_prob and maxdepth > 0:
        func_num = random.randint(0, len(func_list)-1)
        children = [makerandomtree(param_count, maxdepth - 1, func_prob, param_prob)
                    for i in range(func_list[func_num][PARAM_COUNT])]
        return createtree(FUNC_NODE, func_num, -1, False, children)
    elif random.random() < param_prob:
        return createtree(PARAM_NODE, -1, random.randint(0, param_count - 1), False, None)
    else:
        return createtree(CONST_NODE, -1, random.randint(0, 10), False, None)


@cython.boundscheck(False)
@cython.wraparound(False)
cpdef long evaluate(list treearray, list input):
    cdef list values
    cdef long node[11]
    cdef Py_ssize_t node_type
    cdef int func_num, param_count, i
    cdef Py_ssize_t col, start, length, param
    cdef long val

    node = treearray[0]
    node_type = node[TYPE_COL]
    if node_type == FUNC_NODE:
        func_num = node[FUNC_NUM]
        function = func_list[func_num, FUNCTION]
        param_count = param_array[func_num]
        values = []
        col = CHILDOFFSETS
        for i in range(param_count):
            start = node[col]
            length = node[col + 1]
            col += 2
            val = evaluate(treearray[start:start+length], input)
            values.append(val)
        return function(*values)
    elif node_type == PARAM_NODE:
        param = node[VALUE_OR_PARAM]
        return input[param]
    elif node_type == CONST_NODE:
        return node[VALUE_OR_PARAM]



def runexperiment():
    treearray = makerandomtree(2)
    print(treearray)
    print(evaluate(treearray, [5,2]))

cdef timeit():
    runs = 250000

    start = time.time()
    population = []
    input = [10, 42]
    for i in range(runs):
        population.append(makerandomtree(2))
    mid = time.time()
    for tree in population:
        evaluate(tree, input)

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
    treearray = np.array(tree)
    input = [5,2]
    assert evaluate(treearray, input) == 28
    input = [2,5]
    assert evaluate(treearray, input) == 70

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
    treearray = np.array(tree)
    input = [5,2]
    assert evaluate(treearray, input) == 2
    input = [2,5]
    assert evaluate(treearray, input) == 5
    print("Pass test_evaluate")


def test_makerandomtree():
    random.seed(10)
    treearray = np.asarray(makerandomtree(2))
    assert treearray.shape == (1,11)
    assert  evaluate(treearray, [5, 2]) == 5

    treearray = np.asarray(makerandomtree(2))
    assert treearray.shape == (9,11)
    assert evaluate(treearray, [100, 200]) == 1

    treearray = np.asarray(makerandomtree(3))
    assert treearray.shape == (3,11)
    assert evaluate(treearray, [1, 2, 3]) == -1

    treearray = np.asarray(makerandomtree(4))
    assert treearray.shape == (1,11)
    assert evaluate(treearray, [4, 3, 2, 1]) == 2

    treearray = np.asarray(makerandomtree(5))
    assert treearray.shape == (1,11)
    assert evaluate(treearray, [2, 4, 6, 8, 10]) == 10
    print("Pass test_makerandomtree")



def main():
    # runexperiment()
    test_evaluate()
    test_makerandomtree()
    timeit()

if __name__ == "__main__":
    main()