from random import random, randint, choice
from copy import deepcopy
from math import log
import datetime
import stats
import functionwrapper as fw
import math

class node:
    def __init__(self, fw, children):
        self.function = fw.function
        self.name = fw.name
        self.children = children

    def evaluate(self, inp):
        results = [n.evaluate(inp) for n in self.children]
        return self.function(results)

    def display(self, indent=0):
        print (' ' * indent) + self.name
        for c in self.children:
            c.display(indent + 1)


class paramnode:
    def __init__(self, idx):
        self.idx = idx

    def evaluate(self, inp):
        return inp[self.idx]

    def display(self, indent=0):
        print '%sp%d' % (' ' * indent, self.idx)


class constnode:
    def __init__(self, value):
        self.value = value

    def evaluate(self, inp):
        return self.value

    def display(self, indent=0):
        print '%s%d' % (' ' * indent, self.value)



class exampletree(node):
    def __init__(self):
        children = [node(fw.gtw, [paramnode(0), constnode(3)]),
                    node(fw.addw, [paramnode(1), constnode(5)]),
                    node(fw.subw, [paramnode(1), constnode(2)])]

        node.__init__(self, fw.ifw, children)





def makerandomtree(pc, maxdepth=4, fpr=0.5, ppr=0.6):
    if random() < fpr and maxdepth > 0:
        f = choice(fw.flist)
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


def scorefunction(tree, dataset):
    dif = 0
    for row in dataset:
        val = tree.evaluate([row[0], row[1]])
        dif += abs(val - row[2])
    return dif


def mutate(t, pc, probchange=0.1):
    if random() < probchange:
        return makerandomtree(pc)
    else:
        result = deepcopy(t)
        if hasattr(t, "children"):
            result.children = [mutate(c, pc, probchange) for c in t.children]
        return result


def crossover(t1, t2, probswap=0.7, top=1):
    if random() < probswap and not top:
        return deepcopy(t2)
    else:
        result = deepcopy(t1)
        if hasattr(t1, 'children') and hasattr(t2, 'children'):
            result.children = [crossover(c, choice(t2.children), probswap, 0)
                               for c in t1.children]
        return result


def getrankfunction(dataset):
    def rankfunction(population):
        scores = [(scorefunction(t, dataset), t) for t in population]
        scores.sort()
        return scores

    return rankfunction


def evolve(numparams, popsize, rankfunction, maxgen=500,
           mutationrate=0.1, breedingrate=0.4, fitnesspref=0.7, probnew=0.05,
           incprobnew=False, evolvebest=0):
    # incprobnew tracks how often the same fitness score repeats and slowly both
    # increases probnew while decreasing fitnesspref so as to allow more randomness to get unstuck

    def selectindex(fitnesspref):
        # Returns a random number, tending towards lower numbers. The lower pref_fit
        # is, more lower numbers you will get
        return int(log(random()) / log(fitnesspref))

    # Create a random initial population
    population = [makerandomtree(numparams) for i in range(popsize)]
    lastscore = None
    stuckcounter = 0
    for i in range(maxgen):
        scores = rankfunction(population)

        # If we get same value too often, allow in new random nodes
        adj_probnew = probnew
        adj_fitnesspref = fitnesspref
        if incprobnew:
            if scores[0][0] == lastscore:
                stuckcounter += 1
            else:
                stuckcounter = 0
            lastscore = scores[0][0]

            if stuckcounter > 0:
                adj_probnew = probnew + 2.0*(float(stuckcounter)/100.0)
                if adj_probnew > min(probnew * 2.0, 0.5): adj_probnew = min(probnew * 2.0, 0.5)
                # adj_fitnesspref = fitnesspref * (adj_val + 0.5)

        print "Generation:", i+1, "Score:", scores[0][0], "Adj Prob New:", adj_probnew, "Adj Fitness Pref:", adj_fitnesspref
        if scores[0][0] == 0: break

        # The two best always make it
        newpop = [scores[0][1], scores[1][1]]

        # If evolvebest is set, the nmake sure the first program is always evolved that number of times
        if evolvebest > 0:
            for i in range(evolvebest):
                newpop.append(mutate(
                        crossover(scores[randint(0,10)][1],
                                  randint(0,popsize-1),
                                  probswap=0.90),
                        numparams, probchange=min(0.30, 0.5)))

        # Build the next generation
        while len(newpop) < popsize:
            if random() > adj_probnew:
                newpop.append(mutate(
                    crossover(scores[selectindex(adj_fitnesspref)][1],
                              scores[selectindex(adj_fitnesspref)][1],
                              probswap=breedingrate),
                    numparams, probchange=mutationrate))
            else:
                # Add a random node to mix things up
                newpop.append(makerandomtree(numparams))

        population = newpop
    scores[0][1].display()
    return (scores[0][1], i+1)


def getstats(rounds=50, incprobnew=False, evolvebest=0):
    dataset = buildhiddenset()
    rf = getrankfunction(dataset)
    tries = []
    for i in range(rounds):
        print "*******Round: ", i+1, "*******"
        start = datetime.datetime.now()
        best, generations = evolve(2, 500, rf, maxgen=50, mutationrate=0.2, breedingrate=0.3, fitnesspref=0.7, probnew=0.1, \
                                   incprobnew=incprobnew, evolvebest=evolvebest) # breedingrate=0.1 probnew=0.1
        score = scorefunction(best, dataset)
        end = datetime.datetime.now()
        delta = end - start
        seconds = delta.total_seconds()
        row = (score, seconds, generations, best)
        tries.append(row)

    scores = [row[0] for row in tries]
    avg_score = sum(scores) / len(scores)
    std_score = stats.stddev(scores)

    successes = scores.count(0)

    times = [row[1] for row in tries]
    avg_time = sum(times) / len(times)
    std_time = stats.stddev(times)

    generations = [row[2] for row in tries]
    avg_generations = sum(generations) / len(generations)
    std_generations = stats.stddev(generations)


    print "# of Successes:", successes
    print "Average Score:", avg_score, "StD:", round(std_score, 2)
    print "Average Time (Seconds):", avg_time, "StD:", round(std_time, 2)
    print "Average Generations:", avg_generations, "StD:", round(std_generations, 2)

    return successes, avg_score, avg_time, avg_generations

