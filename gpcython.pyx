from random import random, randint, choice
from copy import deepcopy
from math import log
import datetime
import operator
from cpython cimport array
import array
from libc.stdio cimport printf
#from numba import njit, jit

# Utility Functions for Cython (when I can't remember how to do it). See: https://cython.readthedocs.io/en/latest/src/tutorial/array.html
# Note: Type is the available types from the built in Python arrays: https://docs.python.org/3/library/array.html
# Initially I'm constraining to 'i' = int, 'l' = long, 'd' = double, 'f' = float, 
# cdef _int_carray_from_list(list l):
#     cdef array.array a = array.array('i', l)
#     cdef int[:] ca = a
#     return ca

# cdef _long_carray_from_list(list l):
#     cdef array.array a = array.array('l', l)
#     cdef long[:] ca = a
#     return ca

# cdef _float_carray_from_list(list l):
#     cdef array.array a = array.array('f', l)
#     cdef float[:] ca = a
#     return ca

cdef double[:] _double_carray_from_list(list l):
    cdef array.array a = array.array('d', l)
    cdef double[:] ca = a
    return ca


cdef double[:] carray_from_list(list l):
    return _double_carray_from_list(l)


# Stats Code
cdef mean(list data):
    cdef double[:] cdata
    cdef int size
    size = len(data)
    cdata = carray_from_list(data)
    printf("size:%d\n",size)
    printf("*\n")
    print(cdata)

    return cmean(cdata, size)

cdef stddev(list data):
    cdef double[:] cdata
    cdef int size
    size = len(data)
    cdata = carray_from_list(data)
    printf("size:%d\n",size)
    printf("*\n")
    print(cdata)
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
class fwrapper:
    def __init__(self, funct, params, name):
        self.function = funct
        self.params = params
        self.name = name


# Functions with 2 parameters
addw = fwrapper(lambda p: p[0] + p[1], 2, 'add')
subw = fwrapper(lambda p: p[0] - p[1], 2, 'subtract')
mulw = fwrapper(lambda p: p[0] * p[1], 2, 'multiply')
# If and > Function
def iffunc(l):
    if l[0] > 0:
        return l[1]
    else:
        return l[2]

ifw = fwrapper(iffunc, 3, 'if')


def isgreater(l):
    if l[0] > l[1]:
        return 1
    else:
        return 0
gtw = fwrapper(isgreater, 2, 'isgreater')

# List of possible functions
flist = [addw, mulw, ifw, gtw, subw]



class node:
    def __init__(self, fwrappper, children):
        self.function = fwrappper.function
        self.name = fwrappper.name
        self.children = children
        self.lock = False


    def evaluate(self, inp):
        results = [n.evaluate(inp) for n in self.children]
        return self.function(results)


    def display(self, indent=0):
        if self.lock:
            add = "*"
        else:
            add = ""
        print( (' ' * indent) + self.name + add)
        for c in self.children:
            c.display(indent + 1)


class paramnode:
    def __init__(self, idx):
        self.idx = idx

    def evaluate(self, inp):
        return inp[self.idx]

    def display(self, indent=0):
        print('%sp%d' % (' ' * indent, self.idx))


class constnode:
    def __init__(self, value):
        self.value = value

    def evaluate(self, inp):
        return self.value

    def display(self, indent=0):
        print('%s%d' % (' ' * indent, self.value))



class exampletree(node):
    def __init__(self):
        children = [node(gtw, [paramnode(0), constnode(3)]),
                    node(addw, [paramnode(1), constnode(5)]),
                    node(subw, [paramnode(1), constnode(2)])]

        node.__init__(self, ifw, children)




def makerandomtree(pc, maxdepth=4, fpr=0.5, ppr=0.6):
    if random() < fpr and maxdepth > 0:
        f = choice(flist)
        children = [makerandomtree(pc, maxdepth - 1, fpr, ppr)
                    for i in range(f.params)]
        return node(f, children)
    elif random() < ppr:
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


def scorefunction(tree, dataset, penalizecomplexity=False):
    start = datetime.datetime.now()
    dif = 0
    for row in dataset:
        val = tree.evaluate([row[0], row[1]])
        dif += abs(val - row[2])

    if penalizecomplexity == False:
        return (dif, tree)
    else:
        end = datetime.datetime.now()
        delta = end - start
        seconds = delta.total_seconds()
        return (dif, tree, seconds)



# This is the locksubtree function that locks subtrees
def locksubtree(t, probchange=0.05, top=True):
    result = deepcopy(t)

    if not hasattr(result, "children"):
        return result
    if result.lock:
        return result
    if random() < probchange and not top:
        result.lock = True
        return result
    else:
        result.children = [locksubtree(c, probchange, top=False) for c in result.children]
        return result




def mutate(t, pc, probchange=0.1):
    if random() < probchange:
        return makerandomtree(pc)
    else:
        result = deepcopy(t)
        if hasattr(t, "children"):
            result.children = [mutate(c, pc, probchange) for c in t.children]

        return result


def crossover(t1, t2, probswap=0.1, top=True):
    # print "t1:", getattr(t1, "id", -1), "t2:", getattr(t2, "id", -1)
    if random() < probswap and not top:
        # print "return t2:", getattr(t2, "id", -1)
        return deepcopy(t2)
    else:
        result = deepcopy(t1)
        if hasattr(t1, 'children') and hasattr(t2, 'children') and not getattr(result, "lock", False):
            result.children = [crossover(c, choice(t2.children), probswap, top=False)
                               for c in t1.children]
        # print "return crossover:", getattr(result, "id", -1)
        return result


def getrankfunction(dataset):
    def rankfunction(population, penalizecomplexity=False):
        scores = [scorefunction(t, dataset, penalizecomplexity) for t in population]
        if penalizecomplexity:
            scores = sorted(scores, key=operator.itemgetter(0, 2))
        else:
            scores = sorted(scores, key=operator.itemgetter(0))

        return scores

    return rankfunction


def getscore(scores):
    vals = [(row[0], getattr(row[1],"id", -1)) for row in scores]
    return vals


def getids(population):
    vals = [getattr(row, "id", -1) for row in population]
    return vals


def evolve(pc, popsize, rankfunction, maxgen=500,
           mutationrate=0.2, breedingrate=0.1, fitnesspref=0.7, probnew=0.1,
           penalizecomplexity=False, detectstuck=False, modularize=False, mute=False):
    # Returns a random number, tending towards lower numbers. The lower pexp
    # is, more lower numbers you will get
    def selectindex():
        return int(log(random()) / log(fitnesspref))

    # Create a random initial population
    population = [makerandomtree(pc) for i in range(popsize)]
    lastscore = None
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
            if random() > probnew:
                newpop.append(mutate(
                    crossover(scores[selectindex()][1],
                              scores[selectindex()][1],
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


def getstats(rounds=50, maxgen=50, mutationrate=0.05, breedingrate=0.10, fitnesspref=0.95, probnew=0.10, penalizecomplexity=False, modularize=False, mute=False):
    dataset = buildhiddenset()
    rf = getrankfunction(dataset)
    tries = []
    for i in range(rounds):
        if not mute:
          print("*******Round: ", i+1, "*******")
        start = datetime.datetime.now()
        scores, generations = evolve(2, 500, rf, maxgen=maxgen, mutationrate=mutationrate, breedingrate=breedingrate, fitnesspref=fitnesspref, probnew=probnew, penalizecomplexity=penalizecomplexity, modularize=modularize, mute=mute)
        best = scores[0][1]
        score = scorefunction(best, dataset)
        end = datetime.datetime.now()
        delta = end - start
        seconds = delta.total_seconds()
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

    # print "Final Population", getids(population)
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
    # print("Best Paramaters************")
    # getstats(rounds=5, maxgen=50, mutationrate=0.05, breedingrate=0.10, fitnesspref=0.95, probnew=0.10, mute=False)
    # print(" ")
    # print(" ")
    print("Penalize Complexity*********")
    getstats(rounds=2, maxgen=10, mutationrate=0.05, breedingrate=0.10, fitnesspref=0.95, probnew=0.10, penalizecomplexity=True, mute=False)
    # print " "
    # print " "
    # print "Modualization*********"
    # getstats(rounds=100, maxgen=50, mutationrate=0.05, breedingrate=0.10, fitnesspref=0.95, probnew=0.10, penalizecomplexity=True, modularize=True, mute=True)

def main():
    runexperiment()

if __name__ == "__main__":
    main()