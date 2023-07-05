



def UnInformedSearch ( method_name , problem_file_name):
    file = open(problem_file_name, mode='r', encoding='utf-8-sig')
    text = file.read()[1:-1:] # get rid of { }
    parse1 = text.split(",")
    parse2 = parse1[0].split(":")
    minimum_visit = int(parse2[1]) # minimum visit
    grid = ""
    grid = grid.join(parse1[1::])
    grid = grid.split(":")
    grid = grid[1::][0]
    grid = grid[2:-1:].split(" ")
    lst = []
    x = 0
    total_customer_visit = x
    return_list = []

    for line in grid: ### set grid
        lst.append(line[1:-1:])
    #print(lst)

    i = 0
    j = 0
    length = len(lst)
    num_of_vertices = 0
    vertices = []
    while i < length:
        j = 0
        while j < len(lst[i]):
            if lst[i][j] == "S":
                s = (i,j)
                vertices.append((i,j))
                num_of_vertices +=1
            elif lst[i][j] == "F":
                f = (i,j)
                vertices.append((i,j))
                num_of_vertices += 1
            elif lst[i][j] == "C":
                vertices.append((i,j))
                num_of_vertices += 1
            else:
                pass
            j += 1
        i += 1

    rows, cols = (num_of_vertices, num_of_vertices)
    graph = [[0 for i in range(rows)] for j in range(rows)]
    i = 0
    j = 0
    while i < num_of_vertices:
        j = 0
        while j < num_of_vertices:
            graph[i][j] = 0
            j += 1
        i += 1
    i = 0
    j = 0
    #print(vertices)
    for elem in vertices:
        j = 0
        if elem == s:
            start_index = i
        elif elem == f:
            finish_index = i
        for elem2 in vertices:
            #print(elem,elem2)
            distance = abs(elem[0] - elem2[0]) + abs(elem[1] - elem2[1])
            #print(distance)
            #print(graph[i][j])
            graph[i][j] = distance
            j += 1
        #print(graph)
        i += 1
    #print(vertices)
    #print(start_index,finish_index)
    ############## graph fully set before search (adjacent matrix with weights)  #######################
    def sortlist(queue): # queue = [([i,j,k,l],12,1),(..),(..)] [(path,distance,total_customer),(path,distance,total_customer)]
        queue_length = len(queue)
        if (queue_length > 1):
            for i in range(queue_length):
                for j in range(queue_length - i - 1):
                    if (queue[j][1] > queue[j + 1][1]):
                        temp = queue[j]
                        queue[j] = queue[j + 1]
                        queue[j + 1] = temp
        return queue
    # BFS solution
    if method_name == "BFS":
        if minimum_visit + 2 > len(vertices):
            return None
        elif minimum_visit <= 0:
            return [vertices[start_index],vertices[finish_index]]
        visited = [False] * num_of_vertices
        q = [start_index]
        visited[start_index] = True
        while q:
            visit  = q.pop(0)
            return_list.append(vertices[visit])
            if( visit != start_index and visit != finish_index): # check for minimum requirement
                total_customer_visit += 1
            if( total_customer_visit == minimum_visit):
                return_list.append(vertices[finish_index])
                return return_list
            for i in range(num_of_vertices):
                if (graph[visit][i] != 0 and not visited[i] and i != start_index and i != finish_index and total_customer_visit < minimum_visit):
                    #print(i)
                    q.append(i) # push queue if not visited
                    visited[i] = True # set visited
        return None # return none for no solution

    # DFS solution
    elif method_name == "DFS":
        if minimum_visit + 2 > len(vertices):
            return None
        elif minimum_visit <= 0:
            return [vertices[start_index],vertices[finish_index]]
        visited = [False] * num_of_vertices
        stack = []
        stack.append(start_index)
        while( len(stack) > 0):
            visit = stack[-1]
            stack.pop()
            if (not visited[visit] and total_customer_visit < minimum_visit and visit != start_index and visit != finish_index):
                total_customer_visit += 1
                visited[visit] = True
                return_list.append(vertices[visit])
            elif (not visited[visit] and total_customer_visit < minimum_visit):
                visited[visit] = True
                return_list.append(vertices[visit])
            for i in range(num_of_vertices):
                if( graph[visit][i] != 0 and not visited[i] and i != start_index and i != finish_index and total_customer_visit < minimum_visit):
                    stack.append(i)
        return_list.append(vertices[finish_index])
        return return_list
    # UCS solution
    elif method_name == "UCS":
        def UCS():
            if minimum_visit + 2 > len(vertices):
                return None
            visited = [False] * num_of_vertices
            answer = []
            queue = []
            queue.append([[start_index], 0, 0])
            visited[start_index] = True
            while (len(queue) > 0):
                queue = sortlist(queue)
                visit = queue[0]  # shortest path so far
                queue.pop(0)
                if (len(queue) > 0 and visit[1] != queue[0][1] and visit[-1] == minimum_visit):
                    visited[visit[0][-1]] = True  # set visited
                elif len(queue) == 0:
                    visited[visit[0][-1]] = True  # set visited
                if (visit[-1] == minimum_visit and visit[0][-1] == finish_index):
                    for x in visit[0]:
                        answer.append(vertices[x])
                    return answer
                elif (visit[0][-1] == finish_index):
                    continue
                for i in range(num_of_vertices):
                    if (graph[visit[0][-1]][i] != 0 and not visited[
                        i] and i != start_index and i != finish_index and i not in visit[0]):
                        tmp2 = []
                        for x in visit[0]:
                            tmp2.append(x)
                        tmp2.append(i)
                        tmp = [tmp2, visit[1], visit[2]]
                        queue.append([tmp[0], tmp[1] + graph[tmp[0][-2]][i],
                                      tmp[-1] + 1])  # if we visit customer, increase customer_count
                    elif (graph[visit[0][-1]][i] != 0 and not visited[i] and i not in visit[0]):
                        tmp2 = []
                        for x in visit[0]:
                            tmp2.append(x)
                        tmp2.append(i)
                        tmp = [tmp2, visit[1], visit[2]]
                        queue.append([tmp[0], tmp[1] + graph[tmp[0][-2]][i], tmp[-1]])  # if we visit start or finish again in interval node, don't increase customer_count

        if minimum_visit + 2 > len(vertices): # if not possible, return none directly
            return None
        elif minimum_visit <= 0: # minvisit == 0,start,finish is the solution direcrly
            return [vertices[start_index],vertices[finish_index]]
        def sortlist(queue):  #path = [[1,2,4,5,6,7],30] path = [path indexes,total cost]
            queue_length = len(queue)
            if (queue_length > 1):
                for i in range(queue_length):
                    for j in range(queue_length - i - 1):
                        if (queue[j][-1] > queue[j + 1][-1]):
                            temp = queue[j]
                            queue[j] = queue[j + 1]
                            queue[j + 1] = temp
            return queue
        def calcperm(arr, size):
            result = list([[]])
            for dummy_idx in range(size):
                temp = []
                for dummy_lst in result:
                    for dummy_outcome in arr:
                        if len(dummy_lst) == 0:
                            dummy_lst.append(start_index)
                        if dummy_outcome not in dummy_lst:
                            new_seq = list(dummy_lst)
                            new_seq.append(dummy_outcome)
                            temp.append(list(new_seq))
                result = temp
            return result
        permlist = []
        for i in range(num_of_vertices):
            if i != start_index and i != finish_index:
                permlist.append(i)
        indexes = calcperm(permlist,minimum_visit)
        for i in indexes:
            i.append(finish_index)
        sz = len(indexes[0])
        for path in indexes:
            total_customer_visit = 0
            i = 0
            while( i < sz-1):
                total_customer_visit += graph[path[i]][path[i+1]]
                i += 1
            path.append(total_customer_visit) ## add path cost to each paths
        sortlist(indexes)
        indices = indexes[0]
        i = 0
        path = []
        while (i < sz): # path indies to x,y points
            path.append(vertices[indices[i]])
            i+= 1
        return path
    else:
        return None

#print(UnInformedSearch("DFS","sample3.txt"))
#print(UnInformedSearch("BFS","sample3.txt"))
#print(UnInformedSearch("UCS","sample3.txt"))