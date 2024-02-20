import numpy as np
import os

BASE = os.path.dirname(os.path.abspath(__file__))
dir = os.path.join(BASE, 'data')
input = "ai_13.matrix"
f = open(os.path.join(dir, input),"r")
l = []
l = [line.split() for line in f]
# print(l)
arr = np.array(l).astype(int)
# print(arr)
q = []

def Distance(node):
    dist = 0
    for i in range(len(node)-1):
        
        dist += arr[node[i],node[i+1]]
    
    return dist


def finalDistance(node):
    dist = 0
    for i in range(len(node)-1):
        
        dist += arr[node[i],node[i+1]]
    
    return dist + arr[node[0],node[-1]]


def best_cost_lowerbound(node, options):
    lb = 0
    lb += (2*finalDistance(node))
    for loc in options:
        distances = list(arr[loc])
        distances.sort()
        lb += distances[1]
        lb += distances[2]
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
            while opt:
                # print("node: ",node)
                cnode = node.copy()
                cnode.append(opt.pop(0))

                # print("opt:",opt[i])
                # print("cnode: ",cnode)
                
                #Distance
                if best_cost_lowerbound(cnode, opt) <= ub:
                    q.append(cnode)
               
                # print("stack:", q)

    return ub,sol


if __name__ == "__main__":
    sol = 0
    ub , sol = BranchAndBound()

    print("UB:",ub)
    print("Endsolution:",sol)