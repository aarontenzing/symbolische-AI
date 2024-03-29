import numpy as np
import os
import time as Time

# Read the matrix from the file
BASE = os.path.dirname(os.path.abspath(__file__))
dir = os.path.join(BASE, 'data')
input = "ai_11.matrix"
f = open(os.path.join(dir, input),"r")
l = [line.split() for line in f]
arr = np.array(l).astype(int)

# Calculate the distance of the final path
def finalDistance(node):
    dist = 0
    for i in range(len(node)-1):
        
        dist += arr[node[i],node[i+1]]
    
    return dist + arr[node[0],node[-1]]

def Distance(node):
    dist = 0
    for i in range(len(node)-1):
        
        dist += arr[node[i],node[i+1]]
    
    return dist

# Calculate the lowerbound of the cost. If the lowerbound is greater than the upperbound, the node is pruned.
def bestCost(cnode, options):
    lb = 0
    opt = list(np.setdiff1d(options,cnode))
    lb += 2*finalDistance(cnode)
    # lb += 2*Distance(cnode) # Alternatief
    for x in opt:
        distances = list(arr[x])
        distances.sort()
        lb += distances[0] + distances[1]
        # lb += distances[1] + distances[2] # Alternatief

    return lb/2

def BranchAndBound():
    q = [] #Stack
    ub = np.inf # Upperbound
    options = list(range(0,arr.shape[0])) # Options
    node = []
    q.append([options[0]]) #Insert N_0
    options.pop(0)
    
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
                if finalDistance(cnode) <= ub: # Eerste kostfunctie
                # if bestCost(cnode, opt) <= ub: # Tweede kostfunctie
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
    print("Endsolution:", ub, sol)
