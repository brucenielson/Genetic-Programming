# http://quantsoftware.gatech.edu/MC3-Project-1
import random
import math
import numpy as np




"""
TreeLearner is my generic implementation that can do any type of decision tree. 
It is not used in the Georgia Tech class, but I am including it so that this code is reusable outside
of class in the future. Plus it's a template of how to setup the other classes.
"""
class TreeLearner(object):
    def __init__(self, leaf_size=1, verbose=False, tree_type="correlation", output_type="regression", is_continuous=True):
        self.leaf_size = leaf_size
        self.verbose = verbose
        self.tree_type = tree_type
        self.output_type = output_type
        self.input_type = is_continuous

    def author(self):
        return 'bnielson7'

    def addEvidence(self, Xtrain, Ytrain=None):
        Xtrain = make_ndarray(Xtrain)
        if Ytrain is None:
            data = Xtrain
        else:
            Ytrain = make_ndarray(Ytrain)
            data = np.c_[Xtrain, Ytrain]

        # TODO: make it use ndarrays instead
        # ??data = np.concatenate([Xtrain, Ytrain], axis=1)
        self.model = build_tree(data, leaf_size=self.leaf_size, verbose=self.verbose, tree_type=self.tree_type,
                                output_type=self.output_type)
        return self.model



    def query(self, X):
        return query_tree(X, self.model)



"""
Example of DTLearner:
import DTLearner as dt
learner = dt.DTLearner(leaf_size = 1, verbose = False) # constructor
learner.addEvidence(Xtrain, Ytrain) # training step
Y = learner.query(Xtest) # query
"""
class DTLearner(TreeLearner):
    def __init__(self, leaf_size=1, verbose=False):
        self.leaf_size = leaf_size
        self.verbose = verbose
        self.tree_type = "correlation"
        self.output_type = "regression"
        self.input_type = True






# query expects a list with rows as samples and columns as features WITHOUT Y connected
def query_tree(X, model):
    X = make_ndarray(X)
    # Loop through each sample in a Row
    y = []
    root = model[0]
    index=0
    for i in range(0, len(X)):
        current_node = root
        index=0
        while current_node[0] != "leaf":
            split_feature = current_node[0]
            split_val = current_node[1]
            value = X[i,split_feature]
            if value >= split_val:
                # Take left branch
                offset = current_node[2]
                current_node = model[index + offset]
                index += offset
            else:
                # Take right branch
                offset = current_node[3]
                current_node = model[index + offset]
                index += offset
        # We are on a leaf
        if current_node[0] != "leaf": raise Exception("Something wrong with structure of tree model.")
        guess = current_node[1]
        y.append(guess)

    return y


def make_ndarray(X):
    if type(X) != np.ndarray:
        X = np.asarray(X)
    return X


def split_array(data, split_feature, split_val):
    data = make_ndarray(data)
    right_data = []
    left_data = []
    for i in range(data.shape[0]):
        if data[i,split_feature] >= split_val:
            left_data.append(data[i])
        else:
            right_data.append(data[i])

    return np.asarray(left_data), np.asarray(right_data)



def build_tree(data, leaf_size=1, verbose=False, tree_type="correlation", output_type="regression"):
    data = make_ndarray(data)

    # See http://quantsoftware.gatech.edu/images/4/4e/How-to-learn-a-decision-tree.pdf
    #build_tree(data)
    # if data.shape[0] == 1:    return [leaf, data.y, NA, NA]
    # If only one row left, it's a leaf by default
    if len(data) <= leaf_size:
        # For regression trees, use the average of the Y value in the rows making up this leaf
        return ["leaf", float(data[:,-1].mean()), -1, -1]

    #if all    data.y    same:    return [leaf, data.y, NA, NA]
    # Check to see if final column, i.e. Y, is all the same value. If so, terminate.
    if len(np.unique(data[:,-1])) == 1:
        # Since all Y results are identical in the remaining rows, just return the first Y value for the rows
        return ["leaf", float(data[0,-1]), -1, -1]

    else:
        # Main loop
        #determine best or random feature i to split on
        if tree_type == 'random':
            # TODO: Fix so that it more intellgiently handles selection of a feature and split that cause an error due to all being on one side
            split_feature = get_random_feature(data)
            # Find where to split the feature up into by randomly selecting a feature and the mean of two rows for that feature
            # SplitVal	=	(data[random,i]	+	data[random,i])	/	2
            split_val = get_split_val(data, split_feature)
        elif tree_type=="correlation":
            # Both entropy (for trees with only discrete values) and variance (for trees with some continuous values) bot need to get the best feature by that criteria
            # Precondition: data must have rows
            split_feature, split_val = get_best_feature(data)
            # Post conditiion: split_feature and split_val give the best split in terms of entropy or variance
        else:
            raise Exception("'tree_type' must be random, or correlation")

        # Split data up by puttings all of split_feature above split value into left tree and the rest in right tree
        # Split data up
        #leftree = build_tree(data[data[:, i] <= SplitVal])
        #righttree = build_tree(data[data[:, i] > SplitVal])
        #left_data = data[data[:][split_feature] >= split_val]
        #right_data = data[data[:][split_feature] < split_val]
        left_data, right_data = split_array(data, split_feature, split_val)

        # If the split fails - i.e. all of the data went into one branch or neither branch has rows -- create a leaf
        data_size = len(data)
        left_size = len(left_data)
        right_size = len(right_data)
        if data_size != left_size + right_size:
            raise Exception("We lost data in the split.")
        if data_size == 0:
            raise Exception("Data contains no rows.")
        if left_size == 0 and right_size == 0:
            raise Exception("Both left tree and right tree can't both be zero")
        if left_size == data_size or right_size == data_size:
            if output_type == "classification":
                counts = dict(data[:,-1].value_counts())
                maximum_value = max(counts, key=counts.get)
                return ["leaf", float(maximum_value), -1, -1]
            elif output_type=='regression':
                return ["leaf", float(data[:,-1].mean()), -1, -1]
            else:
                raise Exception("'output_type' must be classification or regression")

        if verbose:
            # Show statistics
            print("")
            print("")
            print("Split on:" + str(split_feature) + " at " + str(split_val))
            print("Left Tree:")
            print("Rows: " + str(len(left_data)))
            print("")
            print("Right Tree:")
            print("Rows: " + str(len(right_data)))
            print("*************************************")


        # The split is good -- there are rows in both left and right tree -- so call recursively
        left_tree = build_tree(left_data, leaf_size=leaf_size, verbose=verbose, tree_type=tree_type, output_type=output_type)
        right_tree = build_tree(right_data, leaf_size=leaf_size, verbose=verbose, tree_type=tree_type, output_type=output_type)

        # Create root
        # root = [i,	SplitVal,	1,	leftree.shape[0]	+	1]
        if (left_tree[0] != 'leaf'):
            root = [split_feature, split_val, 1, len(left_tree) + 1]
        else:
            # If left tree is a single node, then just add it and put the right size at 2
            root = [split_feature, split_val, 1, 2]

        # Return tree
        return create_rows(root, left_tree, right_tree)



def get_best_feature(data):
    best_score = None
    best_feature = None
    best_split_val = None
    # Run through each feature and see which one has the best (highest) score
    for feature in range(data.shape[1]-1): # Don't include Y
        score, split_val = calc_correlation(feature, data)

        if best_score == None or score >= best_score:
            best_score = score
            best_feature = feature
            best_split_val = split_val

    return best_feature, best_split_val


def get_split_val(data, split_feature):
    # SplitVal	=	(data[random,i]	+	data[random,i])	/	2
    rand_nbr1 = random.randint(0,len(data)-1)
    item1 = data[rand_nbr1,split_feature]
    rand_nbr2 = rand_nbr1
    while rand_nbr2 == rand_nbr1:
        rand_nbr2 = random.randint(0, len(data)-1)
    item2 = data[rand_nbr2,split_feature]
    split_val = (item1 + item2) / 2

    return split_val


# TODO: I should have used a numpy array and used squeeze() to fix the nesting
# TODO: No! Better yet! Use "Ravel()" or "Flatten()" Or use flat_list = [item for sublist in l for item in sublist]
def create_rows(root, left, right):
    root = remove_nesting(root)
    left = remove_nesting(left)
    right = remove_nesting(right)
    ret = [root]
    if type(left[0]) == list:
        for i in range(0,len(left)):
            ret.append(left[i])
    else:
        ret.append(left)
    if type(right[0]) == list:
        for i in range(0,len(right)):
            ret.append(right[i])
    else:
        ret.append(right)
    return ret


def remove_nesting(tree):
    if len(tree) > 1:
        return tree
    else:
        return remove_nesting(tree[0])




def calc_correlation(feature_name, data):
    if len(data)==0: return 0
    data = make_ndarray(data)
    # https://www.mathsisfun.com/data/correlation.html
    # Get feature
    x = data[:, feature_name]
    # Get expected result
    y = data[:,-1]

    #Step 1: Find the mean of x, and the mean of y
    x_mean = x.mean()
    y_mean = y.mean()
    #Step 2: Subtract the mean of x from every x value (call them "a"), do the same for y	(call them "b")
    a = x - x_mean
    b = y - y_mean
    #Step 3: Calculate: a x b, a2 and b2 for every value
    ab = a * b
    a2 = a * a
    b2 = b * b
    #Step 4: Sum up a x b, sum up a2 and sum up b2
    sum_ab = ab.sum()
    sum_a2 = a2.sum()
    sum_b2 = b2.sum()
    #Step 5: Divide the sum of a x b by the square root of [(sum of a2) x (sum of b2)]
    #compare to scipy
    #from scipy.stats.stats import pearsonr
    #corr2 = pearsonr(x, y)
    if not (sum_a2 == 0.0 or sum_b2 == 0.0): # Check that we can even get the correlation
        corr = sum_ab / math.sqrt(sum_a2 * sum_b2)
    else:
        corr = 0.0

    return abs(corr), x_mean



def get_random_feature(data):
    data = make_ndarray(data)
    feature_nbr = random.randint(0, data.shape[1] - 2)
    split_feature = feature_nbr
    return split_feature
