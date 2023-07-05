
class Node:

    def __init__(self, value = None):
        self.value = value # [1,3,5]
        self.children = []
        self.v = 0 # utility values
        self.index = 0 # index of children which the value comes from
        self.count = 0 # count of total visited nodes from this node to leaf

def SolveGame ( method_name , problem_file_name , player_type ):
    f = open(problem_file_name)
    inp = f.read()[1:-1:1].split(",")
    lst = []
    for x in inp:
        lst.append(int(x))
    ### input parsed to lst
    root = Node(lst +["MAX"]) # root of the tree
    tree = root
    depth = 0  # depth of the tree
    stack = [] # stack for keeping children nodes
    stack.append(tree)
    #### TREE CONSTRUCTION
    while (len(stack) > 0):
        tree = stack.pop()
        # generate children
        i = 0 # index of elem
        for elem in tree.value[:-1]: # traverse through value and set children nodes
            j = 0
            while(j < elem): # [2,3,5] --> [0,3,5],[1,3,5]
                lst = tree.value[:-1]
                lst[i] = j  # same piles and values just change desired value
                if( tree.value[-1]  == "MIN" ): # if successor is MIN node, set child as max
                    node = Node(lst + ["MAX"])
                    tree.children.append(node) # add children list
                else:
                    node = Node(lst + ["MIN"])
                    tree.children.append(node) # add children list
                stack.append(node) # push to stack every child
                j+=1
            i+= 1
    #print_tree(root)
    if method_name == "Minimax": # Minimax solution
        def max_value(state):
            state.v = int(-pow(2,10)) ## consider as int min since we have only -1 and 1 utilities
            i = 0
            index = 0
            while (i < len(state.children)):  # each successor of state
                if (value(state.children[i]).v > state.v):  # max(v,successor)
                    index = i
                    state.v = state.children[i].v
                state.count += state.children[i].count
                i += 1
            state.count+=1
            state.index = index # index of successor which has MAX value
            return state
        def min_value(state):
            state.v = int(pow(2,10))
            i = 0
            index = 0
            while ( i < len(state.children) ):
                if( value(state.children[i]).v < state.v ):
                    index = i
                    state.v = state.children[i].v
                state.count += state.children[i].count
                i+=1
            state.count+=1
            state.index = index
            return state
        def value(state):
            if ( state.value[-1] == "MAX" and  len(state.children) == 0 ): # if its terminal state and MIN player lost return 1
                state.count = 1
                state.v = 1
                return state
            elif ( state.value[-1] == "MIN" and  len(state.children) == 0 ): # if its terminal state and MAX player last moved
                state.count = 1
                state.v = -1
                return state
            else: # not terminal state
                if( state.value[-1] == "MAX" ): # not terminal and max state
                    return max_value(state)
                else: # MIN state
                    return min_value(state)
        value(root)
        ret = []
        ret.append(tuple(root.children[root.index].value[:-1])) # optimal child form root of the tree
        ret.append(root.children[root.index].count-1) # total count -1 since I also counted [1,3,2] as well during computation
        return ret
    elif method_name == "AlphaBeta": # AlphaBeta solution
        alfa = int(-pow(2,10))## -infinity
        beta = int(pow(2,10)) # +infinity
        def max_value(state):
            nonlocal alfa
            nonlocal beta
            state.v = int(-pow(2,10)) ## consider as int min since we have only -1 and 1 utilities
            i = 0
            index = 0
            while (i < len(state.children)):  # each successor of state
                if (value(state.children[i]).v > state.v):  # max(v,successor)
                    index = i
                    state.v = state.children[i].v
                state.count += state.children[i].count
                if ( state.v >= beta ): # prune
                    state.count += 1
                    state.index = index  # index of successor which has MAX value
                    return state
                alfa = max(alfa,state.v)
                i += 1
            state.count+=1
            state.index = index # index of successor which has MAX value
            return state
        def min_value(state):
            nonlocal alfa
            nonlocal beta
            state.v = int(pow(2,10))
            i = 0
            index = 0
            while ( i < len(state.children) ):
                if( value(state.children[i]).v < state.v ):
                    index = i
                    state.v = state.children[i].v
                state.count += state.children[i].count
                if( alfa >= state.v ):
                    state.count += 1
                    state.index = index  # index of successor which has MAX value
                    return state
                beta = min(beta,state.v)
                i+=1
            state.count+=1
            state.index = index
            return state
        def value(state):
            if ( state.value[-1] == "MAX" and  len(state.children) == 0 ): # if its terminal state and MIN player lost return 1
                state.count = 1
                state.v = 1
                return state
            elif ( state.value[-1] == "MIN" and  len(state.children) == 0 ): # if its terminal state and MAX player last moved
                state.count = 1
                state.v = -1
                return state
            else: # not terminal state
                if( state.value[-1] == "MAX" ): # not terminal and max state
                    return max_value(state)
                else: # MIN state
                    return min_value(state)
        value(root)
        ret = []
        ret.append(tuple(root.children[root.index].value[:-1])) # optimal child form root of the tree
        ret.append(root.children[root.index].count-1) # total count -1 since I also counted [1,3,2] as well during computation
        return ret
    else:
        return None
# print tree if you want ,BFS
def print_tree(root):
    queue = []
    queue.append(root)
    while(len(queue) > 0):
        tree = queue.pop(0)
        print(tree.value)
        i = 0
        while(i < len(tree.children)):
            queue.append(tree.children[i])
            i+=1

#print(SolveGame("Minimax","nim1.txt","MAX"))
#print(SolveGame("Minimax","nim2.txt","MAX"))
#print(SolveGame("AlphaBeta","nim1.txt","MAX"))
#print(SolveGame("AlphaBeta","nim2.txt","MAX"))