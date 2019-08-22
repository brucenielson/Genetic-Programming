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
from libc.math cimport log
from copy import deepcopy
import operator
from cpython cimport array
import array
## distutils: language = c++





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
cdef func2param func_array2[5]
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
    # func_array2[i] = <func2param>func_list[i][FUNCTION]

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


    def __init__(self):
        pass



    # TODO: Can python call a super class method?
    cdef long evaluate(self, list inp):
        super.evaluate(inp)


    def display(self, indent=0):
        super.display(indent)



# node objects
cdef class functnode(node):
    # cdef str name
    cdef object function
    cdef list children
    cdef bint lock
    cdef Py_ssize_t func_num
    # cdef Py_ssize_t param_num
    cdef Py_ssize_t size

    def __cinit__(self, Py_ssize_t func_num, list children=None):
        self.setnode(func_num, children)

    @cython.boundscheck(False)
    @cython.wraparound(False)
    @cython.nonecheck(False)
    cdef void setnode(self, Py_ssize_t func_num, list children=None):
        self.node_type = FUNC_NODE
        self.func_num = func_num
        self.lock = False
        self.function = func_array[self.func_num]
        # self.name = name_array[self.func_num]
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
            # return self.function(*results)
            return self.function(results[0], results[1], results[2])
        # TODO: fix to use return self.function(*results)


    def display(self, indent=0):
        if self.lock:
            add = "*"
        else:
            add = ""
        name = name_array[self.func_num]
        print( (' ' * indent) + name + add)
        for c in self.children:
            c.display(indent + 1)

    def __deepcopy__(self,memo_dictionary):
        cp = functnode.__new__(functnode, self.func_num, self.children)
        return cp


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
            print('%sp%d' % (' ' * indent, self.idx))

    def __deepcopy__(self,memo_dictionary):
        cp = paramnode.__new__(paramnode, self.idx)
        return cp

cdef class constnode(node):
    cdef long constval

    def __cinit__(self, long constval):
        self.constval = constval
        self.node_type = CONST_NODE


    @cython.boundscheck(False)
    @cython.wraparound(False)
    @cython.nonecheck(False)
    cdef long evaluate(self, list inp):
        return self.constval

    def display(self, indent=0):
            print('%s%d' % (' ' * indent, self.constval))  

    def __deepcopy__(self,memo_dictionary):
        cp = constnode.__new__(constnode, self.constval)
        return cp


def hiddenfunction(x, y):
    return x ** 2 + 2 * y + 3 * x + 5


def buildhiddenset():
    rows = []
    for i in range(200):
        x = crandint(0, 40)
        y = crandint(0, 40)
        rows.append([x, y, hiddenfunction(x, y)])
    return rows



# class exampletree(node):
#     def __init__(self):
#         children = [node(gtw, [paramnode(0), constnode(3)]),
#                     node(addw, [paramnode(1), constnode(5)]),
#                     node(subw, [paramnode(1), constnode(2)])]

#         node.__init__(self, ifw, children)



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



cdef object scorefunction(object tree, list dataset, bint penalizecomplexity=False):
    cdef double dif, val, start, end, x, y, z
    cdef int i, size
    if penalizecomplexity:
        start = time.time()
    dif = 0
    size = len(dataset)
    for i in range(size):
        x, y, z = dataset[i]
        val = (<node>tree).evaluate([x, y])
        dif += abs(val - z)

    if penalizecomplexity:
        end = time.time()
        return (dif, tree, end-start)
    else:
        return (dif, tree)



# This is the locksubtree function that locks subtrees
cdef object locksubtree(object t, float probchange=0.05, bint top=True):
    result = deepcopy(t)

    if not hasattr(result, "children"):
        return result
    if result.lock:
        return result
    if crandom() < probchange and not top:
        result.lock = True
        return result
    else:
        result.children = [locksubtree(c, probchange, top=False) for c in result.children]
        return result




cdef object mutate(object t, int pc, float probchange=0.1):
    if crandom() < probchange:
        return makerandomtree(pc)
    else:
        result = deepcopy(t)
        if hasattr(t, "children"):
            result.children = [mutate(c, pc, probchange) for c in t.children]

        return result


cdef object crossover(object t1, object t2, float probswap=0.1, bint top=True):
    cdef Py_ssize_t child_num
    cdef object child
    # print "t1:", getattr(t1, "id", -1), "t2:", getattr(t2, "id", -1)
    if crandom() < probswap and not top:
        # print "return t2:", getattr(t2, "id", -1)
        return deepcopy(t2)
    else:
        result = deepcopy(t1)
        if hasattr(t1, 'children') and hasattr(t2, 'children') and not getattr(result, "lock", False):
            child_num = crandint(0,len(t2.children)-1)
            child = t2.children[child_num]
            result.children = [crossover(c, child, probswap, top=False)
                               for c in t1.children]
        # print "return crossover:", getattr(result, "id", -1)
        return result


cdef getrankfunction(list dataset):
    def rankfunction(population, penalizecomplexity=False):
        scores = [scorefunction(t, dataset, penalizecomplexity) for t in population]
        if penalizecomplexity:
            scores = sorted(scores, key=operator.itemgetter(0, 2))
        else:
            scores = sorted(scores, key=operator.itemgetter(0))

        return scores

    return rankfunction


cdef int selectindex(float fitnesspref, int popsize) except -1:
    # TODO: https://stackoverflow.com/questions/40976880/canonical-way-to-generate-random-numbers-in-cython?noredirect=1&lq=1
    cdef float r
    cdef int val
    r = crandom() + 0.00000000001
    return <int>(log(r) / log(fitnesspref))


def runexperimentold():
    srand(10)
    tree = makerandomtree(2)
    tree.display()
    print(tree.evaluate([5,2]))
    # tree = exampletree()
    # tree.display
    # print(tree.evaluate[5,2])


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
    print("Benchmark for makerandomtree and evaluate (geneticprogrammingcython2.py):  ", mid-start, end-mid)
    
    # Build the next generation
    start = time.time()
    popsize = runs
    newpop = []
    fitnesspref = 0.7
    while len(newpop) < popsize:
        newpop.append(mutate(
            crossover(population[selectindex(fitnesspref, popsize)],
                        population[selectindex(fitnesspref, popsize)],
                        ),
            2))
    end = time.time()
    print("Benchmark for mutate and crossover (geneticprogrammingcython2.py):  ", end-start)




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


cdef object evolve(int pc, int popsize, object rankfunction, int maxgen=500,
           float mutationrate=0.2, float breedingrate=0.1, float fitnesspref=0.7, float probnew=0.1,
           bint penalizecomplexity=False, bint detectstuck=False, bint modularize=False, bint mute=False):
    
    # Returns a random number, tending towards lower numbers. The lower pexp
    # is, more lower numbers you will get
    cdef int i, stuckcounter, lastscore
    
    # Create a random initial population
    population = [makerandomtree(pc) for i in range(popsize)]
    lastscore = -1
    stuckcounter = 0
    for i in range(maxgen):
        scores = rankfunction(population, penalizecomplexity)

        # If we get same value too often, take action
        adj_mutate = mutationrate
        if detectstuck:
            if scores[0][2] == lastscore:
                stuckcounter += 1
            else:
                stuckcounter = 0

            lastscore = scores[0][2]

            if stuckcounter > 0:
                adj_mutate = mutationrate + 2.0 * (float(stuckcounter) / 100.0)
                if adj_mutate > 0.5: adj_mutate = 0.5

        if not mute:
            if penalizecomplexity:
                print("Generation:", i + 1, "Best Score:", scores[0][0], "Time:", scores[0][2])
            else:
                print("Generation:", i + 1, "Best Score:", scores[0][0])

        if scores[0][0] == 0: break

        # The two best always make it
        newpop = [scores[0][1], scores[1][1]]

        if modularize:
            # Next one is same as first one, but modularized
            newpop.append(locksubtree(newpop[0]))

        # Build the next generation
        while len(newpop) < popsize:
            if crandom() > probnew:
                newpop.append(mutate(
                    crossover(scores[selectindex(fitnesspref, popsize)][1],
                              scores[selectindex(fitnesspref, popsize)][1],
                              probswap=breedingrate),
                    pc, probchange=adj_mutate))
            else:
                # Add a random node to mix things up
                newpop.append(makerandomtree(pc))

        population = newpop

    if not mute:
        print("******")
        print("Best Tree Found:")
        scores[0][1].display()

    return (scores, i + 1)


cpdef getstats(int rounds=50, int maxgen=50, float mutationrate=0.05, float breedingrate=0.10, float fitnesspref=0.95, float probnew=0.10, bint penalizecomplexity=False, bint detectstuck = False, bint modularize=False, bint mute=False):
    cdef int i
    dataset = buildhiddenset()
    rf = getrankfunction(dataset)
    tries = []
    for i in range(rounds):
        if not mute:
          print("*******Round: ", i+1, "*******")
        start = time.time()
        scores, generations = evolve(2, 500, rf, maxgen=maxgen, mutationrate=mutationrate, breedingrate=breedingrate, fitnesspref=fitnesspref, probnew=probnew, penalizecomplexity=penalizecomplexity, detectstuck=detectstuck, modularize=modularize, mute=mute)
        best = scores[0][1]
        score = scorefunction(best, dataset)
        end = time.time()
        seconds = end - start
        row = (score[0], seconds, generations, best)
        tries.append(row)

    scores = [row[0] for row in tries]

    avg_score = mean(scores)
    std_score = stddev(scores)

    successes = scores.count(0)
    success_perc = float(successes) / float(rounds)
    failures = rounds - successes
    success_failure = []
    for i in range(successes):
        success_failure.append(1.0)
    for i in range(failures):
        success_failure.append(0.0)

    std_successes = stddev(success_failure)

    times = [row[1] for row in tries]
    avg_time = mean(times)
    std_time = stddev(times)

    generations = [row[2] for row in tries]
    avg_generations = mean(generations)
    std_generations = stddev(generations)

    print("# of Successes:", successes, "StD:", round(std_successes * float(rounds),2), "Success %:", success_perc, "StD:", round(std_successes, 2))
    print("Average Score:", avg_score, "StD:", round(std_score, 2))
    print("Average Time (Seconds):", avg_time, "StD:", round(std_time, 2))
    print("Average Generations:", avg_generations, "StD:", round(std_generations, 2))

    return successes, avg_score, avg_time, avg_generations

def runexperiment():
    # print "Default********************"
    # getstats(rounds=100, maxgen=50, mutationrate=0.2, breedingrate=0.1, fitnesspref=0.7, probnew=0.1, mute=True)
    # print " "
    # print " "
    print("Best Paramaters************")
    getstats(rounds=5, maxgen=50, mutationrate=0.05, breedingrate=0.10, fitnesspref=0.95, probnew=0.10, mute=False)
    print(" ")
    # print(" ")
    # print("Penalize Complexity*********")
    # getstats(rounds=2, maxgen=50, mutationrate=0.05, breedingrate=0.10, fitnesspref=0.95, probnew=0.10, penalizecomplexity=True, mute=False)
    # print " "
    # print " "
    # print "Modualization*********"
    # getstats(rounds=100, maxgen=50, mutationrate=0.05, breedingrate=0.10, fitnesspref=0.95, probnew=0.10, penalizecomplexity=True, modularize=True, mute=True)



if __name__ == "__main__":
    main()




cdef _int_carray_from_list(list l):
    cdef array.array a = array.array('i', l)
    cdef int[:] ca = a
    return ca

cdef _long_carray_from_list(list l):
    cdef array.array a = array.array('l', l)
    cdef long[:] ca = a
    return ca

cdef _float_carray_from_list(list l):
    cdef array.array a = array.array('f', l)
    cdef float[:] ca = a
    return ca

cdef _double_carray_from_list(list l):
    cdef array.array a = array.array('d', l)
    cdef double[:] ca = a
    return ca


cdef carray_from_list(char type_code, list l):
    if type_code == 'i':
        return _int_carray_from_list(l), len(l)
    elif type_code == 'l':
        return _long_carray_from_list(l), len(l)
    elif type_code == 'f':
        return _float_carray_from_list(l), len(l)
    elif type_code == 'd':
        return _double_carray_from_list(l), len(l)
    else:
        raise Exception("type_code not a supported type.")


# Stats Code
cdef mean(list data):
    cdef double[:] cdata
    cdef int size
    cdata, size = carray_from_list("d", data)
    return cmean(cdata, size)

cdef stddev(list data):
    cdef double[:] cdata
    cdef int size
    cdata, size = carray_from_list("d", data)
    return cstddev(cdata, size)


cdef double cmean(double[:] data, int size):
    """Return the sample arithmetic mean of data."""
    cdef double tot
    if size < 1:
        raise ValueError('mean requires at least one data point')
    
    # Now loop in C and get mean
    tot = 0
    for i in range(size):
        tot += data[i]
    tot = tot / float(size)
    return tot


cdef double _ss(double[:] data, int size):
    """Return sum of square deviations of sequence data."""
    cdef double c, tot
    c = cmean(data, size)
    tot = 0
    # C loop
    for i in range(size):
        tot += (data[i] - c)**2
    return tot


cdef double cstddev(double[:] data, int size, int ddof=1):
    """Calculates the population standard deviation
    if ddof=0; specify ddof=1 to compute the sample
    standard deviation. (which I made default)"""
    cdef double pvar, ss
    if size < 2:
        raise ValueError('variance requires at least two data points')
    ss = _ss(data, size)
    pvar = ss/(size-ddof)
    return pvar**0.5

