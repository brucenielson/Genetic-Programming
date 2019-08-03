cdef class node:
    cdef int value

    def __init__(self):
        self.value = 1
    
    cdef int evaluate(self):
        return self.value

def main():
    node1 = node()

    val = node1.evaluate()
    print(val)

