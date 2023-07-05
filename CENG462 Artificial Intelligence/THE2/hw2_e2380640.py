# PriorityQueue
class PriorityQueue(object):
    def __init__(self):
        self.queue = []

    def __str__(self):
        return ' '.join([str(i) for i in self.queue])

    # for checking if the queue is empty
    def isEmpty(self):
        return len(self.queue) == 0

    # for inserting an element in the queue
    def insert(self, data):
        self.queue.append(data)

    # for popping an element based on Priority
    def delete(self):
        try:
            min_val = 0
            for i in range(len(self.queue)):
                if self.queue[i][1] < self.queue[min_val][1]:
                    min_val = i
            item = self.queue[min_val]
            del self.queue[min_val]
            return item
        except IndexError:
            print()
            exit()

def InformedSearch ( method_name , problem_file_name ):
    f = open(problem_file_name)
    lines = f.readlines() # read grid
    graph = []
    visited = []
    length = len(lines)
    start_index = 0
    end_index = 0
    rows, cols = length, length
    for i in range(rows):
        col = []
        col2 = []
        for j in range(cols):
            col.append(0)
            col2.append(0)
        graph.append(col)
        visited.append(col2)
    i = 0
    for line in lines:
        j = 0
        for ind in line:
            if (ind != "\t" and ind != "\n"):
                if (ind == "S"):
                    start_index = (i,j)
                elif (ind == "E"):
                    end_index = (i,j)
                graph[i][j] = ind
                j += 1

        i += 1
    pqueue = PriorityQueue() # pqueue for shortest path so far
    if method_name == "UCS": # UCS Search
        pqueue.insert([[start_index],0])
        while ( pqueue.isEmpty() == False ):
            visit = pqueue.delete() # shortest path so far
            visited[visit[0][-1][0]][visit[0][-1][1]] = 1 # set visited
            if ( visit[0][-1] == end_index ): # reached end_index
                ret = []
                for (x,y) in visit[0]: # reverse list for output and also x,y --> y,x
                    ret.append((y,x))
                return ret[::-1]
            if ( visit[0][-1][1] > 0 and graph[visit[0][-1][0]][visit[0][-1][1]-1] != "#" and visited[visit[0][-1][0]][visit[0][-1][1]-1] == 0 ): # left,unvisited,not #
                pqueue.insert([visit[0] + [(visit[0][-1][0],visit[0][-1][1]-1)],visit[1]+1]) # push queue
            if ( visit[0][-1][0] > 0 and graph[visit[0][-1][0]-1][visit[0][-1][1]] != "#" and visited[visit[0][-1][0]-1][visit[0][-1][1]] == 0):  # up,unvisited,not #
                pqueue.insert([visit[0] + [(visit[0][-1][0]-1,visit[0][-1][1])],visit[1]+1]) # push queue
            if ( visit[0][-1][1] < length-1 and graph[visit[0][-1][0]][visit[0][-1][1]+1] != "#" and visited[visit[0][-1][0]][visit[0][-1][1]+1] == 0): # right,unvisited,not #
                pqueue.insert([visit[0] + [(visit[0][-1][0],visit[0][-1][1]+1)],visit[1]+1]) # ğush queue
            if ( visit[0][-1][0] < length-1 and graph[visit[0][-1][0]+1][visit[0][-1][1]] != "#" and visited[visit[0][-1][0]+1][visit[0][-1][1]] == 0):  # down,unvisited,not #
                pqueue.insert([visit[0] + [(visit[0][-1][0]+1,visit[0][-1][1])],visit[1]+1]) # queue
        return None
    elif method_name == "AStar": # AStar search
        pqueue.insert([[start_index],0,0]) # [path,h(x)+path_cost,path_cost] ,path costs are real values
        while( not pqueue.isEmpty() ):
            visit = pqueue.delete() # shortest path so far
            visited[visit[0][-1][0]][visit[0][-1][1]] = 1 # set visited
            if ( visit[0][-1] == end_index ): # reached end_index
                ret = []
                for (x,y) in visit[0]: # reverse list for output and also x,y --> y,x
                    ret.append((y,x))
                return ret[::-1]
            # push queue with total(x)+h(x) and keep total(x) in the queue as well
            if ( visit[0][-1][1] > 0 and graph[visit[0][-1][0]][visit[0][-1][1]-1] != "#" and visited[visit[0][-1][0]][visit[0][-1][1]-1] == 0 ): # left,unvisited,not #
                if( graph[visit[0][-1][0]][visit[0][-1][1]-1] == "S" or graph[visit[0][-1][0]][visit[0][-1][1]-1] == "E"): # if its S or E, h(x) = 0
                    pqueue.insert([visit[0] + [(visit[0][-1][0], visit[0][-1][1] - 1)],visit[2], visit[2] + 1])  # push queue
                else:
                    pqueue.insert([visit[0] + [(visit[0][-1][0], visit[0][-1][1] - 1)], visit[2] + int(graph[visit[0][-1][0]][visit[0][-1][1] - 1]), visit[2] + 1])  # push queue
            if ( visit[0][-1][0] > 0 and graph[visit[0][-1][0]-1][visit[0][-1][1]] != "#" and visited[visit[0][-1][0]-1][visit[0][-1][1]] == 0):  # up,unvisited,not #
                if( graph[visit[0][-1][0]-1][visit[0][-1][1]] == "S" or graph[visit[0][-1][0]-1][visit[0][-1][1]] == "E"):
                    pqueue.insert([visit[0] + [(visit[0][-1][0] - 1, visit[0][-1][1])],visit[2],visit[2] + 1])  # push queue
                else:
                    pqueue.insert([visit[0] + [(visit[0][-1][0] - 1, visit[0][-1][1])],visit[2] + int(graph[visit[0][-1][0] - 1][visit[0][-1][1]]), visit[2] + 1])  # push queue
            if ( visit[0][-1][1] < length-1 and graph[visit[0][-1][0]][visit[0][-1][1]+1] != "#" and visited[visit[0][-1][0]][visit[0][-1][1]+1] == 0): # right,unvisited,not #
                if( graph[visit[0][-1][0]][visit[0][-1][1]+1] == "S" or graph[visit[0][-1][0]][visit[0][-1][1]+1] == "E"):
                    pqueue.insert([visit[0] + [(visit[0][-1][0], visit[0][-1][1] + 1)],visit[2], visit[2] + 1])  # ğush queue
                else:
                    pqueue.insert([visit[0] + [(visit[0][-1][0], visit[0][-1][1] + 1)],visit[2] + int(graph[visit[0][-1][0]][visit[0][-1][1] + 1]), visit[2] + 1])  # ğush queue

            if ( visit[0][-1][0] < length-1 and graph[visit[0][-1][0]+1][visit[0][-1][1]] != "#" and visited[visit[0][-1][0]+1][visit[0][-1][1]] == 0):  # down,unvisited,not #
                if( graph[visit[0][-1][0]+1][visit[0][-1][1]] == "S" or graph[visit[0][-1][0]+1][visit[0][-1][1]] == "E"):
                    pqueue.insert([visit[0] + [(visit[0][-1][0] + 1, visit[0][-1][1])],visit[2], visit[2] + 1])  # queue
                else:
                    pqueue.insert([visit[0] + [(visit[0][-1][0] + 1, visit[0][-1][1])],visit[2] + int(graph[visit[0][-1][0] + 1][visit[0][-1][1]]), visit[2] + 1])  # queue
        return None
    else: # wrong input
        return None

#print(InformedSearch("AStar","sampleAstar.txt"))