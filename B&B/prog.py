import numpy as np

f = open("ai_20", "r")
l = []
l = [line.split() for line in f]
print(l)

arr = np.array(l)
print(arr)

