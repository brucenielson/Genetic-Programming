import numpy as np
from random import random, randint
# How do turn on c-division and turn off bound checking for whole file (if at top of file)
#!python
#cython: boundscheck=False
#cython: cdivision=True
# How to do it locally using decorators and with statement: https://github.com/cython/cython/wiki/enhancements-compilerdirectives
#TODO: cimport cython

# Fake enum for TYPE#
NODE = 1
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
FUNCTION = 0
PARAM_COUNT = 1
NAME = 2

# CONSTANTS
MAX_PARAMS = 3

# Function List
func_list = []

def definefunction(function, param_count, name):
    func_list.append((function, param_count, name))

# Functions with 2 parameters
definefunction(lambda p: p[0] + p[1], 2, 'add')
definefunction(lambda p: p[0] - p[1], 2, 'subtract')
definefunction(lambda p: p[0] * p[1], 2, 'multiply')
# > function
def isgreater(l):
    if l[0] > l[1]:
        return 1
    else:
        return 0
definefunction(isgreater, 2, 'isgreater')

# If Function (3 parameters)
def iffunc(l):
    if l[0] > 0:
        return l[1]
    else:
        return l[2]

definefunction(iffunc, 3, 'if')

func_list = np.array(func_list)
# TODO: How to create a memory view to speed up numpy array: cdef int[:] func_list_cview = func_list. See NumPy Tutorial above. Except this won't work for the function array. I need a better way.



# Program Trees: Lists or Dynamic arrays that follow this format:
# [TYPE #, Function #, value or param indx, lock, node id, child1offset, child1length, child2offset, child2length, child3offset, child3length]
# TODO: Dynamic 2d arrays in Cython: https://stackoverflow.com/questions/25917593/how-to-declare-2d-c-arrays-dynamically-in-cython
# TODO: Or use memoryviews (see above)

#Fake Enum for Column Names
TYPE_COL = 0
FUNC_NUM = 1
VALUE_OR_PARAM = 2
LOCK = 3
ID = 4
CHILD1OFFSET = 5
CHILD1LENGTH = 6
CHILD2OFFSET = 7
CHILD2LENGTH = 8
CHILD3OFFSET = 9
CHILD3LENGTH = 10
NUM_COLS = CHILD3LENGTH + 1

treecounter = 0
nodes = 0

class tree:
    # TODO: cdef version of instance variables
    # cdef list treelist
    # cdef np.array tree
    # cdef int id
    # cdef int next
    def __init__(self):
        global treecounter #TODO: extern?
        self.treelist = []
        self.tree = None # This will be the numpy array
        self.id = treecounter
        treecounter += 1
        self.next = 0

    def getnextid(self):
        next = self.next
        self.next += 1
        return next

def createtree(node_type, funcnum, children, val_or_param=-1, lock=False):
    # Get node id
    global nodes
    id = nodes
    nodes += 1
    # Create base node
    node = [node_type, funcnum, val_or_param, lock, id]
    # If this is a function, get number of parameters expected vs of children given
    if node_type == FUNCTION:
        param_count = func_list[funcnum][PARAM_COUNT]
        # parameter count and number of children in the list must match or else we have an error
        assert param_count == len(children)
        index = 1
        children_array = None
        for i in range(MAX_PARAMS):
            if i < len(children):
                child = children[i].reshape(-1,NUM_COLS)
                assert type(child) == np.ndarray
                node.append(index)
                # TODO: cdef Pyssize_t see http://docs.cython.org/en/latest/src/userguide/numpy_tutorial.html
                size = len(child)
                node.append(size)
                index += size
                if children_array is None:
                    children_array = np.array(child).reshape(-1, NUM_COLS)
                else:
                    children_array = np.concatenate([children_array, child]).reshape(-1, NUM_COLS)
            else:
                node = node + [-1,-1]
    else:
        node = node + [-1,-1,-1,-1,-1,-1]


    node = np.array(node)
    if node_type == FUNCTION:
        tree = np.concatenate([node.reshape(-1,NUM_COLS),children_array]).reshape(-1,NUM_COLS)
    else:
        tree = node

    return tree


def makerandomtree(param_count, maxdepth=4, func_prob=0.5, param_prob=0.6):
    if random() < func_prob and maxdepth > 0:
        func_num = randint(0, len(func_list)-1)
        children = [makerandomtree(param_count, maxdepth - 1, func_prob, param_prob)
                    for i in range(func_list[func_num][PARAM_COUNT])]
        return createtree(FUNCTION, func_num, children)
    elif random() < param_prob:
        return createtree(PARAM_NODE, -1, None, randint(0, param_count - 1))
    else:
        return createtree(CONST_NODE, -1, None, randint(0, 10))



def evaluate(treearray, parameters):
    pass





def runexperiment():
    return makerandomtree(2)

def main():
    return runexperiment()

if __name__ == "__main__":
    main()