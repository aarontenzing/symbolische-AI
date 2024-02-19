import numpy as np

f = open("ai_11.matrix","r")
l = []
l = [line.split() for line in f]
#print(l)
arr = np.array(l).astype(int)
#print(arr)
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
    
    while len(q) != 0:
        
        node = q.pop()
        # print("node: ",node)
        opt = list(np.setdiff1d(options,node))
        # print("remaining options:",opt)

        #Complete solution
        if (len(node) == arr.shape[0]):
            # print("solution:", node)

            if(finalDistance(node) < ub):
                ub = finalDistance(node)
                sol = node
            
        else: 
            # opt = options
            for i in range(len(opt)):
                # print("node: ",node)
                cnode = node.copy()
                cnode.append(opt[i])

                # print("opt:",opt[i])
                # print("cnode: ",cnode)
                
                #Distance
                if Distance(cnode) <= ub:
                    q.append(cnode)
               
                # print("stack:", q)

    return ub,sol

sol = 0
ub , sol = BranchAndBound()

print("UB:",ub)
print("Endsolution:",sol)