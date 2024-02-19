import numpy as np
from itertools import permutations 
 
def findDistance(path, matrix):
    distance = 0
    for i in range(len(path) - 1):
        distance += matrix[path[i]][path[i+1]]
    return distance

def BranchandBound(matrix):
    ub = 9999
    path = []
    stack = []
    stack.append(0)
    length = len(distance_matrix)

    while len(stack) != 0:
        for location in list(range(0,length)):
            if (location not in stack and location != path[-1]):
                stack.append(location)

        if (len(stack) == matrix.shape):
            dist = findDistance(stack, matrix) 
            if dist < ub:
                ub = dist
                path = stack
            stack.pop()
    return ub, path

if __name__ == "__main__":
    f = open("data\\ai_20.matrix", "r")
    line = []
    line = [line.split() for line in f]
    distance_matrix = np.array(line)
    print(distance_matrix.shape)
    print(BranchandBound(distance_matrix))
