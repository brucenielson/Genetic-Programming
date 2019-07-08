import numpy as np


class tree:
    treecounter = 0
    def __init__(self):
        self.id = tree.treecounter
        tree.treecounter += 1
        # Node format
        # [TYPE, Function, ]


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
