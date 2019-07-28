import numpy as np
cimport numpy as np
from random import random, randint
import time
import gp
import gpcython as gpc

# How do turn on c-division and turn off bound checking for whole file (if at top of file)
#!python
#cython: boundscheck=False
#cython: cdivision=True
# How to do it locally using decorators and with statement: https://github.com/cython/cython/wiki/enhancements-compilerdirectives
#TODO: cimport cython

# Fake enum for TYPE#
FUNC_NODE = 1
PARAM_NODE = 2
CONST_NODE = 3

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
cdef enum NodeType:
    FUNCTION = 0
    PARAM_COUNT = 1
    NAME = 2

# CONSTANTS
cdef int MAX_PARAMS = 3

# Function List
func_list = []

cdef definefunction(function, param_count, name):
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



# Program Trees: Lists or Dynamic arrays that follow this format:
# [TYPE #, Function #, value or param indx, lock, node id, child1offset, child1length, child2offset, child2length, child3offset, child3length]
# TODO: Dynamic 2d arrays in Cython: https://stackoverflow.com/questions/25917593/how-to-declare-2d-c-arrays-dynamically-in-cython
# TODO: Or use memoryviews (see above)

#Fake Enum for Column Names
cdef enum ColumnNames:
    TYPE_COL = 0
    FUNC_NUM = 1
    VALUE_OR_PARAM = 2
    LOCK = 3
    ID = 4
    CHILD1OFFSET = 5
    CHILDOFFSETS = CHILD1OFFSET
    CHILD1LENGTH = 6
    CHILD2OFFSET = 7
    CHILD2LENGTH = 8
    CHILD3OFFSET = 9
    CHILD3LENGTH = 10
    NUM_COLS = CHILD3LENGTH + 1

cdef long treecounter = 0
cdef long nodes = 0


cdef object createtree(NodeType node_type, int funcnum, int val_or_param, bint lock, object child1, object child2, object child3):
    # NumPy to C Array: https://github.com/cython/cython/wiki/tutorials-NumpyPointerToC
    # Numpy arrays as parameters: https://stackoverflow.com/questions/4641200/cython-inline-function-with-numpy-array-as-parameter
    # Get node id
    global nodes
    cdef int id = nodes
    cdef index, param_count
    cdef Py_ssize_t i
    nodes += 1
    # Create base node
    node = [node_type, funcnum, val_or_param, lock, id]
    # If this is a function, get number of parameters expected vs of children given
    if node_type == FUNC_NODE:
        param_count = func_list[funcnum][PARAM_COUNT]
        # parameter count and number of children in the list must match or else we have an error
        # assert param_count == len(children)
        index = 1
        children_array = None
        if child1 is not None:
            assert type(child1) == np.ndarray
            size = len(child1)
            node.append(index)
            index += size
            node.append(size)
            children_array = np.array(child1, dtype='int32').reshape(-1, NUM_COLS)
        else:
            node = node + [-1,-1]

        if child2 is not None:
            assert type(child2) == np.ndarray
            size = len(child2)
            node.append(index)
            index += size
            node.append(size)
            children_array = np.concatenate([children_array, child2]).reshape(-1, NUM_COLS)
        else:
            node = node + [-1,-1]

        if child3 is not None:
            assert type(child3) == np.ndarray
            size = len(child3)
            node.append(index)
            index += size
            node.append(size)
            children_array = np.concatenate([children_array, child3]).reshape(-1, NUM_COLS)
        else:
            node = node + [-1,-1]            
        
        # for i in range(MAX_PARAMS):
        #     if i < len(children):
        #         child = children[i].reshape(-1,NUM_COLS)
        #         assert type(child) == np.ndarray
        #         node.append(index)
        #         # TODO: cdef Py_ssize_t see http://docs.cython.org/en/latest/src/userguide/numpy_tutorial.html
        #         size = len(child)
        #         node.append(size)
        #         index += size
        #         if children_array is None:
        #             children_array = np.array(child, dtype='int32').reshape(-1, NUM_COLS)
        #         else:
        #             children_array = np.concatenate([children_array, child]).reshape(-1, NUM_COLS)
        #     else:
        #         node = node + [-1,-1]
    else:
        node = node + [-1,-1,-1,-1,-1,-1]


    node = np.array(node, dtype='int32')
    if node_type == FUNC_NODE:
        tree = np.concatenate([node.reshape(-1,NUM_COLS),children_array]).reshape(-1,NUM_COLS)
    else:
        tree = np.array(node, dtype='int32').reshape(-1,NUM_COLS)

    return tree


def makerandomtree(param_count, maxdepth=4, func_prob=0.5, param_prob=0.6):
    if random() < func_prob and maxdepth > 0:
        func_num = randint(0, len(func_list)-1)
        children = [makerandomtree(param_count, maxdepth - 1, func_prob, param_prob)
                    for i in range(func_list[func_num][PARAM_COUNT])]

        while len(children) < 3:
            children.append(None)
        return createtree(FUNC_NODE, func_num, -1, False, children[0], children[1], children[2])
    elif random() < param_prob:
        return createtree(PARAM_NODE, -1, randint(0, param_count - 1), False, None, None, None)
    else:
        return createtree(CONST_NODE, -1, randint(0, 10), False, None, None, None)



cpdef evaluate(treearray, input):
    node = treearray[0]
    node_type = node[TYPE_COL]
    if node_type == FUNC_NODE:
        func_num = node[FUNC_NUM]
        function = func_list[func_num, FUNCTION]
        param_count = func_list[func_num, PARAM_COUNT]
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

def timeit():
    runs = 250000

    start = time.time()
    population = []
    input = [10, 42]
    for i in range(runs):
        population.append(gpc.makerandomtree(2))
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


    start = time.time()
    population = []
    input = [10, 42]
    for i in range(runs):
        population.append(makerandomtree(2))
    mid = time.time()
    for tree in population:
        evaluate(tree, input)

    end = time.time()
    print("Benchmark (geneticprogramming.py):  ", mid-start, end-mid)



def main():
    # runexperiment()
    timeit()

if __name__ == "__main__":
    main()