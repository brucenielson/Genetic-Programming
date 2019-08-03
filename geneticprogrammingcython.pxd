cdef long evaluate(list, list)
# See Optional Arguments under https://cython.readthedocs.io/en/latest/src/userguide/language_basics.html
# It says to not include optional arguments in header
cdef list makerandomtree(int, int maxdepth=*, float func_prob=*, float param_prob=*)
