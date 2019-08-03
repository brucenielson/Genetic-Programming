from distutils.core import setup
from Cython.Build import cythonize
import numpy

import Cython.Compiler.Options
Cython.Compiler.Options.annotate = True

setup(
    # ext_modules = cythonize("geneticprogrammingcython2.pyx", annotate=True), include_dirs=[numpy.get_include()]
    ext_modules = cythonize(["geneticprogrammingcython.pyx", "geneticprogrammingcython2.pyx"], annotate=True), include_dirs=[numpy.get_include()]
)