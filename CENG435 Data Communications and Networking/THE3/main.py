import sys
import math  # for inf
import socket
import pickle
import threading
import time

HOST = "127.0.0.1"
infinity = math.inf
neighbors = []  # neighbor list
distance_vectors = []
updated = False
number_of_nodes = 0
node_number = 0
neighbor_distance_vector = []
start = 0
end = 0
def listen():
    global neighbor_distance_vector
    global number_of_nodes
    global updated
    global distance_vectors
    global HOST
    global start
    global end
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    s.bind((HOST, node_number))
    s.listen(number_of_nodes)
    while True:
        #print("Distance vectors of " + str(node_number)  + ": " + str(distance_vectors))
        start = time.time()
        conn, addr = s.accept()  # waiting here
        updated_temp = False
        while 1:
            data = conn.recv(1024)
            neighbor_distance_vector = pickle.loads(data)
            #print("Data received from " + str(neighbor_distance_vector[-1]) + ": " + str(neighbor_distance_vector))
            neighbor_node_index = neighbor_distance_vector[-1]
            for i in range(number_of_nodes):
                if (distance_vectors[neighbor_node_index][1] + neighbor_distance_vector[i][1] < distance_vectors[i][
                    1]):
                    distance_vectors[i][1] = distance_vectors[neighbor_node_index][1] + neighbor_distance_vector[i][
                        1]  # change value
                    updated_temp = True
            if updated_temp:
                updated = True
                updated_temp = False
            conn.close()
            break

        conn.close()


def send():
    global updated
    global neighbors
    global HOST
    global distance_vectors
    while True:
        if updated:
            for neighbor in neighbors:
                s = socket.socket()
                s.connect((HOST, neighbor))
                #print("Size of distance_vector: ",sys.getsizeof(distance_vectors))
                data = pickle.dumps(distance_vectors)
                s.send(data)
                s.close()
            updated = False


if __name__ == '__main__':
    ################## INITIALIZE VECTOR ##################
    with open(sys.argv[1] + ".costs", 'r') as f:
        lines = [line.rstrip() for line in f]
        node_number = int(sys.argv[1])
        PORT = node_number
    number_of_nodes = int(lines[0])
    lines = lines[1::]
    distance_vectors = [[3000 + i, infinity] for i in range(number_of_nodes)]  # all distances to this node
    # set inf
    for i in range(number_of_nodes):
        if (node_number == 3000 + i):
            distance_vectors[i][1] = 0
            distance_vectors.append(i)
    # set distances of the neighbors
    for neighbor in lines:
        temp = neighbor.split(" ")
        neighbors.append(int(temp[0]))  # add to neighbors list
        for i in range(number_of_nodes):
            if (distance_vectors[i][0] == int(temp[0])):
                distance_vectors[i][1] = int(temp[1])
                break
    ################## INITIALIZE VECTOR ##################
    updated = True
    t1 = threading.Thread(target=listen)  # listen incoming vectors
    t1.daemon = True
    t2 = threading.Thread(target=send)  # send if update received from listen
    t2.daemon = True
    t1.start()  # listen thread
    time.sleep(0.1)
    t2.start()  # send thread
    while True:
        end = time.time()
        if( end - start >= 5 ):
            for neighbor in distance_vectors[:-1]:  # print distances
                print(f"{node_number} -{neighbor[0]} | {neighbor[1]}")
            sys.exit(0)
