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
