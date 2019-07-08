import numpy as np

# Fake enum types
NODE = 1
PARAM_NODE = 2
CONST_NODE = 3

# Node format
# [TYPE, Function, name, value or param indx, child1, child2, child3, lock, id]
# https://docs.scipy.org/doc/numpy/user/basics.rec.html

class tree:
    treecounter = 0
    def __init__(self):
        self.id = tree.treecounter
        tree.treecounter += 1


class node:
    treecounter = 0
    def __init__(self, fw, children):
        self.function = fw.function
        self.name = fw.name
        self.children = children
        self.id = node.treecounter
        self.lock = False
        node.treecounter += 1


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
