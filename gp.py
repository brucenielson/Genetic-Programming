from random import random, randint, choice
from copy import deepcopy
from math import log
import datetime
import stats
import functionwrapper as fw

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


class paramnode(node):
    def __init__(self, idx):
        self.idx = idx

    def evaluate(self, inp):
        return inp[self.idx]

    def display(self, indent=0):
        print '%sp%d' % (' ' * indent, self.idx)


class constnode(node):
    def __init__(self, value):
        self.value = value

    def evaluate(self, inp):
        return self.value

    def display(self, indent=0):
        print '%s%d' % (' ' * indent, self.value)




def exampletree():
    return node(fw.ifw, [
        node(fw.gtw, [paramnode(0), constnode(3)]),
        node(fw.addw, [paramnode(1), constnode(5)]),
        node(fw.subw, [paramnode(1), constnode(2)])])



def makerandomtree(pc, maxdepth=4, fpr=0.5, ppr=0.6):
    if random() < fpr and maxdepth > 0:
        f = choice(fw.flist)
        children = [makerandomtree(pc, maxdepth - 1, fpr, ppr)
                    for i in range(f.childcount)]
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


def evolve(pc, popsize, rankfunction, maxgen=500,
           mutationrate=0.1, breedingrate=0.4, pexp=0.7, pnew=0.05):
    # Returns a random number, tending towards lower numbers. The lower pexp
    # is, more lower numbers you will get
    def selectindex():
        return int(log(random()) / log(pexp))

    # Create a random initial population
    population = [makerandomtree(pc) for i in range(popsize)]
    for i in range(maxgen):
        scores = rankfunction(population)
        print "Generation:", i+1, "Score:", scores[0][0]
        if scores[0][0] == 0: break

        # The two best always make it
        newpop = [scores[0][1], scores[1][1]]

        # Build the next generation
        while len(newpop) < popsize:
            if random() > pnew:
                newpop.append(mutate(
                    crossover(scores[selectindex()][1],
                              scores[selectindex()][1],
                              probswap=breedingrate),
                    pc, probchange=mutationrate))
            else:
                # Add a random node to mix things up
                newpop.append(makerandomtree(pc))

        population = newpop
    scores[0][1].display()
    return (scores[0][1], i+1)


def get_stats(rounds=50):
    dataset = buildhiddenset()
    rf = getrankfunction(dataset)
    tries = []
    for i in range(rounds):
        print "*******Round: ", i+1, "*******"
        start = datetime.datetime.now()
        best, generations = evolve(2, 500, rf, maxgen=50, mutationrate=0.2, breedingrate=0.1, pexp=0.7, pnew=0.1)
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

