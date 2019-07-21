from unittest import TestCase
import numpy as np
import geneticprogramming as gp
import random

# TODO: Convert unit tests to Cython: https://stackoverflow.com/questions/42259741/how-to-test-functions-cdefd-in-cython

class TestEvaluate(TestCase):
    def test_evaluate(self):
        tree = [[1, 2, -1,  0,  8,  1,  1,  2,  7, -1, -1], # Multiply
                [3, -1,  7,  0,  0, -1, -1, -1, -1, -1, -1],
                [1,  1, -1,  0,  7,  1,  5,  6,  1, -1, -1], # Subtract
                [1,  2, -1,  0,  5,  1,  3,  4,  1, -1, -1], # Multiply
                [1,  0, -1,  0,  3,  1,  1,  2,  1, -1, -1], # Add
                [3, -1,  2,  0,  1, -1, -1, -1, -1, -1, -1],
                [2, -1,  0,  0,  2, -1, -1, -1, -1, -1, -1],
                [2, -1,  1,  0,  4, -1, -1, -1, -1, -1, -1],
                [3, -1, 10,  0,  6, -1, -1, -1, -1, -1, -1]]
        treearray = np.array(tree)
        input = [5,2]
        self.assertEqual(gp.evaluate(treearray, input), 28)
        input = [2,5]
        self.assertEqual(gp.evaluate(treearray, input), 70)

        tree = [[1,  4, -1, 0, 20,  1,  7,  8,  1,  9,  1], # IF
                [1,  0, -1, 0, 17,  1,  5,  6,  1, -1, -1], # Add
                [1,  3, -1, 0, 15,  1,  1,  2,  3, -1, -1], # >
                [2, -1,  0, 0, 11, -1, -1, -1, -1, -1, -1],
                [1,  2, -1, 0, 14,  1,  1,  2,  1, -1, -1], # Multiply
                [3, -1,  7, 0, 12, -1, -1, -1, -1, -1, -1],
                [2, -1,  0, 0, 13, -1, -1, -1, -1, -1, -1],
                [2, -1,  1, 0, 16, -1, -1, -1, -1, -1, -1],
                [2, -1,  1, 0, 18, -1, -1, -1, -1, -1, -1],
                [2, -1,  0, 0, 19, -1, -1, -1, -1, -1, -1]]
        treearray = np.array(tree)
        input = [5,2]
        self.assertEqual(gp.evaluate(treearray, input), 2)
        input = [2,5]
        self.assertEqual(gp.evaluate(treearray, input), 5)


class TestMakeRandomTree(TestCase):
    def test_makerandomtree(self):
        random.seed(10)
        treearray = gp.makerandomtree(2)
        self.assertEqual(treearray.shape, (1,11))
        self.assertEqual(gp.evaluate(treearray, [5, 2]), 5)

        treearray = gp.makerandomtree(2)
        self.assertEqual(treearray.shape, (9,11))
        self.assertEqual(gp.evaluate(treearray, [100, 200]), 1)

        treearray = gp.makerandomtree(3)
        self.assertEqual(treearray.shape, (3,11))
        self.assertEqual(gp.evaluate(treearray, [1, 2, 3]), -1)

        treearray = gp.makerandomtree(4)
        self.assertEqual(treearray.shape, (1,11))
        self.assertEqual(gp.evaluate(treearray, [4, 3, 2, 1]), 2)

        treearray = gp.makerandomtree(5)
        self.assertEqual(treearray.shape, (1,11))
        self.assertEqual(gp.evaluate(treearray, [2, 4, 6, 8, 10]), 10)
