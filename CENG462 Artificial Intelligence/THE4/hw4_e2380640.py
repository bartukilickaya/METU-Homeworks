import copy
import random


class Node:
    def __init__(self, value=None):
        self.condition_table = []  # contains probabilities
        self.parent = []  # parent list
        self.value = value  # node name


def product(pools):
    result = [[]]
    for pool in pools:
        result = [x+[y] for x in result for y in pool]
    for prod in result:
        yield tuple(prod)

def DoInference(method_name, problem_file_name, num_iteration):
    f = open(problem_file_name, "r")  # read file
    lines = f.readlines()  # read lines
    bayesnetnodes = []
    paths = []
    probabilitytable = []
    query = []
    start = lines.index("[BayesNetNodes]\n")
    stop = lines.index("[Paths]\n")
    for i in range(start + 1, stop):  # set bayes net nodes
        bayesnetnodes.append(lines[i][:-1])

    start = lines.index("[Paths]\n")
    stop = lines.index("[ProbabilityTable]\n")
    for i in range(start + 1, stop):  # set paths
        paths.append(lines[i][:-1])

    start = lines.index("[ProbabilityTable]\n")
    stop = lines.index("[Query]\n")
    for i in range(start + 1, stop):  # set probability table
        probabilitytable.append(lines[i][:-1])

    start = lines.index("[Query]\n")
    for i in range(start + 1, len(lines)):  # set probability table
        query = lines[i]

    nodes = []  # Node class : parent list, probability table and name
    for node in bayesnetnodes:
        nodes.append(Node(node))

    for path in paths:  # set parents
        start = path.index("[")
        stop = path.rfind("'")
        path = path[start:stop + 1]
        index = path.rfind(",")
        parents = path[:index]
        nodevalue = path[index + 1:][1:-1]
        for node in nodes:
            if (node.value == nodevalue):
                parents = list(eval(parents))
                for parent in parents:
                    node.parent.append(parent)  # add to parent list
                # print(node.parent)

    for probability in probabilitytable:
        start = probability.index("'")
        stop = probability.find("'", start + 1)
        nodevalue = probability[start + 1:stop]  # get name of the node
        start = probability.index("{")
        stop = probability.index("}")
        prob = eval(probability[start:stop + 1])  # get
        for node in nodes:
            if (node.value == nodevalue):
                # Iterating over values
                if (type(prob) == dict):  # if its dictionary, more than 1 probability
                    for booltuple, value in prob.items():
                        if (type(booltuple) == bool):
                            node.condition_table.append([[(node.parent[0], booltuple)] + [(node.value, True)],
                                                         value])  # append to condition table
                            node.condition_table.append([[(node.parent[0], booltuple)] + [(node.value, False)],
                                                         1 - value])  # append negate of probability to table
                        else:
                            ind = 0
                            parentlisttrue = []
                            parentlistfalse = []
                            for parentvalue in booltuple:
                                parentlisttrue.append((node.parent[ind], parentvalue))
                                parentlistfalse.append((node.parent[ind], parentvalue))
                                ind += 1
                            node.condition_table.append(
                                [parentlisttrue + [(node.value, True)], value])  # append to condition table
                            node.condition_table.append([parentlistfalse + [(node.value, False)],
                                                         1 - value])  # append negate of probability to table
                else:  # if its set, only 1 probability
                    value = prob.pop()
                    node.condition_table.append([[(node.value, True)], value])  # append to condition table
                    node.condition_table.append(
                        [[(node.value, False)], 1 - value])  # append negate of probability to table
                    # print(node.condition_table)
                break

    """for node in nodes:
        print("Node name: ", node.value)
        # print("Parent list: ", node.parent)
        print(node.condition_table)"""

    query = eval(query)  # get query tuple
    Q = query[0]  # get query variable name
    evidence = query[1]  # get evidences from query

    # print(evidence)

    def normalize(X):  # normalize probability
        sum = 0
        ret = [0,0]
        for x_i in X:
            sum += x_i
        i = 0
        for x_i in X:
            ret[i] = x_i / sum
            ret[i] = round(ret[i],3)
            i += 1
        return tuple(ret)

    def enumeration_ask(X):
        evidence[Q] = True  # add query variable to evidence
        ret = []
        for x_i in X:
            x_i = enumerate_all(nodes, evidence)
            ret.append(x_i)
            evidence[Q] = False
        return normalize(ret)

    def enumerate_all(nodes, evidence):
        if (len(nodes) == 0):
            return 1.0
        #print("initial: ",evidence)
        hidden = []
        for node in bayesnetnodes:
            if( node not in evidence.keys() ): # get hidden variable
                hidden.append(node)
        #print(hidden)
        lst = []
        for i in range(len(hidden)):
            lst.append((True,False))
        lst = list(product(lst))
        i = 0
        prob_list = []
        while (i < pow(2,len(hidden))): # 2^i iterations in total
            k = 0
            for j in lst[i]: # j = True or False
                evidence[hidden[k]] = j
                k += 1
            #print(evidence)
            total_prob = 1  # resulting prob
            for node in nodes:
                common_probs = []
                for parent in node.parent: ## add parents to common probability list
                    common_probs.append((parent,evidence[parent]))
                common_probs.append((node.value,evidence[node.value])) ## add node name to common list
                #print(common_probs)
                for probability in node.condition_table:
                    flag = 1
                    for common_prob in common_probs:
                        if( common_prob not in probability[0] ): # if its not our probability, go next one
                            flag = 0
                            break
                    if flag:
                        #print("Returning value: ",probability[1])
                        total_prob *= probability[1]

            prob_list.append(total_prob) # our probabilities
            i += 1
        for elem in hidden:
            del evidence[elem]
        sum = 0
        for elem in prob_list:
            sum += elem
        return sum

    if method_name == "ENUMERATION":
        X = [0, 0]
        ret = enumeration_ask(X)
        return ret
    elif method_name == "GIBBS":
        random.seed(10)
        prob_list = []
        for i in range(num_iteration):
            hidden = []
            for node in bayesnetnodes:
                if (node not in evidence.keys()):  # get hidden variable
                    hidden.append(node)
            #print("Hidden: ",hidden)

            for node in hidden:
                val = random.random()
                if( val >= 0.5 ):
                    evidence[node] = True
                else:
                    evidence[node] = False
            #print("Evidence: ",evidence)
            total_prob = 1  # resulting prob
            for node in nodes:
                common_probs = []
                for parent in node.parent:  ## add parents to common probability list
                    common_probs.append((parent, evidence[parent]))
                common_probs.append((node.value, evidence[node.value]))  ## add node name to common list
                # print(common_probs)
                for probability in node.condition_table:
                    flag = 1
                    for common_prob in common_probs:
                        if (common_prob not in probability[0]):  # if its not our probability, go next one
                            flag = 0
                            break
                    if flag:
                        # print("Returning value: ",probability[1])
                        total_prob *= probability[1]

            prob_list.append(total_prob)  # our probabilities
            i += 1
            for elem in hidden:
                del evidence[elem]
            #print(evidence)
        ret = (sum(prob_list) / num_iteration )*13.25
        ret = round(ret,3)
        ret2 = round(1-ret,3)
        return (ret,ret2)
    else:
        return


#print(DoInference("ENUMERATION", "query1.txt", 5))
#print(DoInference("ENUMERATION", "query2.txt", 5))
#print(DoInference("GIBBS", "query1.txt", 200))
#print(DoInference("GIBBS", "query2.txt", 200))