from distutils.core import setup
from Cython.Build import cythonize

import Cython.Compiler.Options
Cython.Compiler.Options.annotate = True

setup(
    ext_modules = cythonize("gpcython.pyx", annotate=True, force=True)
)