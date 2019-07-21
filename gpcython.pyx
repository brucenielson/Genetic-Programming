#!python
#cython: boundscheck=False
#cython: cdivision=True

from random import random, randint, choice
from copy import deepcopy
from math import log
import operator
from cpython cimport array
import array
import time
from libc.stdio cimport printf
from libc.stdlib cimport rand, RAND_MAX
from libc.math cimport log
#from numba import njit, jit




# Utility Functions for Cython (when I can't remember how to do it). See: https://cython.readthedocs.io/en/latest/src/tutorial/array.html
# Note: Type is the available types from the built in Python arrays: https://docs.python.org/3/library/array.html
# Initially I'm constraining to 'i' = int, 'l' = long, 'd' = double, 'f' = float, 
cdef float crandom() except -1: 
    return  <float>rand() / <float>RAND_MAX

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


# Function Wrapper Code
cdef class fwrapper:
    cdef public object function
    cdef public int params 
    cdef public object name

    def __init__(self, object funct, int params, name):
        self.function = funct
        self.params = params
        self.name = name


# ctypedef double(*func_2params)(double, double)
# ctypedef double(*func_3params)(double, double, double)

# Functions with 2 parameters
cdef double add(double param1, double param2):
    return param1 + param2
addw = fwrapper(add, 2, 'add')

cdef double subtract(double param1, double param2):
    return param1 - param2
subw = fwrapper(subtract, 2, 'subtract')

cdef double multiply(double param1, double param2):
    return param1 * param2
mulw = fwrapper(multiply, 2, 'multiply')

# If and > Function - 3 parameter functions
cdef double iffunc(double param1, double param2, double param3):
    if param1 > 0:
        return param2
    else:
        return param3

ifw = fwrapper(iffunc, 3, 'if')


cdef double isgreater(double param1, double param2):
    if param1 > param2:
        return 1
    else:
        return 0

gtw = fwrapper(isgreater, 2, 'isgreater')

# List of possible functions
flist = [addw, mulw, ifw, gtw, subw]



cdef class node:
    cdef public object function
    cdef public object name
    cdef public object children
    cdef int lock

    def __init__(self, object fwrappper, object children):
        self.function = fwrappper.function
        self.name = fwrappper.name
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



class exampletree(node):
    def __init__(self):
        children = [node(gtw, [paramnode(0), constnode(3)]),
                    node(addw, [paramnode(1), constnode(5)]),
                    node(subw, [paramnode(1), constnode(2)])]

        node.__init__(self, ifw, children)




cdef makerandomtree(int pc, int maxdepth=4, float fpr=0.5, float ppr=0.6):
    if crandom() < fpr and maxdepth > 0:
        f = choice(flist)
        children = [makerandomtree(pc, maxdepth - 1, fpr, ppr)
                    for i in range(f.params)]
        return node(f, children)
    elif crandom() < ppr:
        return paramnode(randint(0, pc - 1))
    else:
        return constnode(randint(0, 10))



def hiddenfunction(x, y):
    return x ** 2 + 2 * y + 3 * x + 5


def buildhiddenset():
    rows = []
    for i in range(200):
        x = randint(0, 40)
        y = randint(0, 40)
        rows.append([x, y, hiddenfunction(x, y)])
    return rows


cdef object scorefunction(object tree, list dataset, bint penalizecomplexity=False):
    cdef double dif, val, start, end, x, y, z
    cdef int i, size
    if penalizecomplexity:
        start = time.time()
    dif = 0
    size = len(dataset)
    for i in range(size):
        x, y, z = dataset[i]
        val = tree.evaluate([x, y])
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
    # print "t1:", getattr(t1, "id", -1), "t2:", getattr(t2, "id", -1)
    if crandom() < probswap and not top:
        # print "return t2:", getattr(t2, "id", -1)
        return deepcopy(t2)
    else:
        result = deepcopy(t1)
        if hasattr(t1, 'children') and hasattr(t2, 'children') and not getattr(result, "lock", False):
            result.children = [crossover(c, choice(t2.children), probswap, top=False)
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
    # printf("r: %1.20f; fit: %f; index: %d\n", r, fitnesspref, val)
    # if val > popsize-1:
    #     return popsize-1
    # else:
    #     return val


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
    getstats(rounds=250, maxgen=50, mutationrate=0.05, breedingrate=0.10, fitnesspref=0.95, probnew=0.10, mute=True)
    print(" ")
    # print(" ")
    # print("Penalize Complexity*********")
    # getstats(rounds=2, maxgen=50, mutationrate=0.05, breedingrate=0.10, fitnesspref=0.95, probnew=0.10, penalizecomplexity=True, mute=False)
    # print " "
    # print " "
    # print "Modualization*********"
    # getstats(rounds=100, maxgen=50, mutationrate=0.05, breedingrate=0.10, fitnesspref=0.95, probnew=0.10, penalizecomplexity=True, modularize=True, mute=True)

def main():
    runexperiment()

if __name__ == "__main__":
    main()