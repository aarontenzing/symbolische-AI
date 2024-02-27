import numpy as np
import os
import time as Time

BASE = os.path.dirname(os.path.abspath(__file__))
dir = os.path.join(BASE, 'data')
input = "ai_11.matrix"
f = open(os.path.join(dir, input),"r")
l = []
l = [line.split() for line in f]
# print(l)
arr = np.array(l).astype(int)
# print(arr)
q = []

# Calculate the distance of the final path
def finalDistance(node):
    dist = 0
    for i in range(len(node)-1):
        
        dist += arr[node[i],node[i+1]]
    
    return dist + arr[node[0],node[-1]]

def bestCost(node):

    node1 = node.copy()
    # print("orig node:",node1)
    check = node1.pop()

    lb = 0
    lb += (2*finalDistance(node1))
    # print("node:",node1)
    # print("option:",check)

    distances = list(arr[check])
    distances = [distances[x] for x in node1]

    distances.sort()
    # print("distances:",distances)
    lb += distances[0]
    if(len(distances) > 1):
        lb += distances[1]

    return lb/2

# Calculate the lowerbound of the cost. If the lowerbound is greater than the upperbound, the node is pruned.
def best_cost_lowerbound(node, options):
    lb = 0
    lb += (2*finalDistance(node))
    for x in options:
        if (x == node[-1]):
            continue
        distances = list(arr[x])
        distances.sort()
        lb += distances[1] + distances[2]
        
    return lb/2
            

def BranchAndBound():
    
    ub = np.inf
    options = list(range(0,arr.shape[0]))
    # print("test")
    # print(options)
    node = []
    #Insert N_0
    q.append([options[0]])
    options.pop(0)
    # print(options)
    
    while q:
        
        node = q.pop()
        # print("node: ",node)
        opt = list(np.setdiff1d(options,node))
        # print("remaining options:",opt)

        #Complete solution
        if (len(node) == arr.shape[0]):
            # print("solution:", node)
            distance = finalDistance(node)   
            if(distance < ub):
                ub = distance
                sol = node
            
        else: 
            # opt = options
            for i in range(len(opt)):
                # print("node: ",node)
                cnode = node.copy()
                cnode.append(opt[i])
                
                #Distance
                if bestCost(cnode) <= ub:
                    q.append(cnode)
               
                # print("stack:", q)

    return ub,sol


if __name__ == "__main__":
    sol = 0
    start_time = Time.time()
    ub , sol = BranchAndBound()
    end_time = Time.time()
    print("matrix: ",input)
    print("Time:",end_time - start_time, "seconds")
    print("UB:",ub)
    print("Endsolution:",sol)