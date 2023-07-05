import random
import copy
import numpy as np


class GameBoard:
    def __init__(self):
        self.current_state = "---------"

    def print_board(self):  # print current state of the board
        print(self.current_state)

    def clear_board(self):  # clear board
        self.current_state = "---------"

    def get_all_available_cells(self):
        lst = []
        i = 0
        for cell in self.current_state:
            if cell == "-":
                lst.append(i)
            i += 1
        return lst  # [0,4] indexes of all possible cells

    def make_move(self, player, action):  # make action with given action which is guaranteed to be available position
        if action in self.get_all_available_cells():
            new_current_state = self.current_state[:action] + player + self.current_state[action+1:]
            self.current_state = copy.deepcopy(new_current_state)
        else:
            print("Wrong position detected.")

    def check_finish(self):
        winning_indices = np.array(
            [[0, 1, 2], [3, 4, 5], [6, 7, 8], [0, 3, 6], [1, 4, 7], [2, 5, 8], [0, 4, 8], [2, 4, 6]])
        for index in winning_indices:
            if self.current_state[index[0]] == "X" and self.current_state[index[1]] == "X" and self.current_state[index[2]] == "X":
                return "X"  # X is winner
            elif self.current_state[index[0]] == "O" and self.current_state[index[1]] == "O" and self.current_state[index[2]] == "O":
                return "O"  # O is winner
        if ("-" not in self.current_state):  # draw, return 0
            return "Draw"
        return False  # game is not finished


class Agent:
    def __init__(self, player):
        self.player = player  # agent is whether X or O
        self.q_table = dict()  # {(0, 0): [('---------'),0.0]}

    def initialize_q_table(self):
        for i in range(3):
            for j in range(3):
                pass
                # self.q_table[(i,j)] =


def product(lst, repeat=1):
    pools = [tuple(pool) for pool in lst] * repeat
    result = [[]]
    for pool in pools:
        result = [x + [y] for x in result for y in pool]
    for prod in result:
        # print(tuple(prod))
        yield list(prod)


def is_finished_states(board):
    winning_indices = np.array([[0, 1, 2], [3, 4, 5], [6, 7, 8], [0, 3, 6], [1, 4, 7], [2, 5, 8], [0, 4, 8], [2, 4, 6]])
    for index in winning_indices:
        if board[index[0]] == "X" and board[index[1]] == "X" and board[index[2]] == "X":
            return True  # finished state return True
        elif board[index[0]] == "O" and board[index[1]] == "O" and board[index[2]] == "O":
            return True  # finished state return True
    return False


def SolveMDP(method_name, problem_file_name, random_seed):
    #print("START")
    f = open(problem_file_name)
    lines = [line.rstrip() for line in f]
    f.close()
    alpha = float(lines[1])  # set alpha
    gamma = float(lines[3])  # set gamma
    epsilon = float(lines[5])  # set epsilon
    epsilon_count = int(lines[7])  # set epsilon count
    all_possible_boards = list(product(["XO-", "XO-"], 4))
    random.seed(random_seed)  # set random seed
    np.random.seed(random_seed)
    if method_name == "SARSA":
        agent = Agent("X")
        agent_opponent = Agent("O")
        for i in range(3):
            for j in range(3):
                board_alternatives = []  # all possible board alternatives for each cell
                for board in all_possible_boards:
                    if (i * 3 + j != 8):
                        temp = board[:i * 3 + j] + ["-"] + board[i * 3 + j:]
                    else:  # index out of range check
                        temp = board[:i * 3 + j] + ["-"]
                    listToStr = ''.join([elem for elem in temp])
                    if not is_finished_states(listToStr):  # if game state is not finished add to board list
                        board_alternatives.append((listToStr, 0.0))

                agent.q_table[(i, j)] = copy.deepcopy(board_alternatives)
                agent_opponent.q_table[(i, j)] = copy.deepcopy(board_alternatives)
        # print(agent_opponent.q_table)
        """
        Agents' initialization fully complete so far including their q_tables, both opponent and player
        """
        for episode in range(epsilon_count):  # total episodes
            #print("Episode Number: ",episode)
            game_board = GameBoard()  # initialize game board (S)
            utility_value = -1000
            i = -1
            j = -1
            index_of_selected_state = -1
            if random.random() <= epsilon:
                possible_actions = game_board.get_all_available_cells()
                #print(possible_actions)
                random_index = np.random.randint(0, len(possible_actions))
                index_of_cell = possible_actions[random_index]  # get the index (i*3+j) from possible list
                i = int(index_of_cell / 3)  # row index
                j = index_of_cell % 3  # column index
                ind = 0
                for board, value in agent.q_table[(i, j)]:
                    if (game_board.current_state == board):  # found the correct index and board
                        utility_value = value  # set utility
                        index_of_selected_state = ind  # set index
                        break
                    ind += 1
            else:  # use max utility if not random
                for key, boardlist in agent.q_table.items():
                    ind = 0
                    for board, value in boardlist:
                        if (game_board.current_state == board):  # found the correct board, add to
                            if (value > utility_value):
                                utility_value = value  # set new utility value
                                index_of_selected_state = ind
                                i, j = key  # index of cell corresponds to maximum utility so far
                            break
                        ind += 1
            while True:  # loop for each episode
                game_board.make_move("X", i * 3 + j)  # set X on  (Take action A, observe R,S')
                #game_board.print_board()
                if (game_board.check_finish() == "X"):  # if game finished with my move
                    #print("1")
                    selected_state_board, _ = agent.q_table[(i, j)][index_of_selected_state]
                    new_utility = (1 - alpha) * utility_value + alpha  # reward = 1 Q(S',A') = 0
                    agent.q_table[(i, j)][index_of_selected_state] = (selected_state_board, new_utility)  # Q(S,A) = (1-a)Q(S,A) + a[R + gammaQ(S',A')]
                    _,util = agent_opponent.q_table[(i,j)][index_of_selected_state]
                    new_utility2 = (1 - alpha) * util - alpha# reward -1
                    agent_opponent.q_table[(i,j)][index_of_selected_state] = (selected_state_board,new_utility2)
                    break
                elif( game_board.check_finish() == "Draw"): # draw
                    #print("2")
                    selected_state_board, _ = agent.q_table[(i, j)][index_of_selected_state]
                    new_utility = (1 - alpha) * utility_value  # reward = 0 Q(S',A') = 0
                    agent.q_table[(i, j)][index_of_selected_state] = (selected_state_board, new_utility)  # Q(S,A) = (1-a)Q(S,A) + a[R + gammaQ(S',A')]
                    _,util = agent_opponent.q_table[(i,j)][index_of_selected_state]
                    new_utility2 = (1 - alpha) * util # reward 0
                    agent_opponent.q_table[(i,j)][index_of_selected_state] = (selected_state_board,new_utility2)
                    break
                """
                Player X(SARSA) played 1 move, now player O turn (Q-learning) 
                """
                utility_value3 = -1000  # utility of O
                i3 = -1  # index of O
                j3 = -1  # index of O
                index_of_selected_state3 = -1  # index of O
                if random.random() <= epsilon:
                    possible_actions = game_board.get_all_available_cells()
                    random_index = np.random.randint(0, len(possible_actions))
                    index_of_cell = possible_actions[random_index]  # get the index (i*3+j) from possible list
                    i3 = int(index_of_cell / 3)  # row index
                    j3 = index_of_cell % 3  # column index
                    ind = 0
                    for board, value in agent_opponent.q_table[(i3, j3)]:
                        if (game_board.current_state == board):  # found the correct index and board
                            utility_value3 = value  # set utility
                            index_of_selected_state3 = ind  # set index
                            break
                        ind += 1
                else:  # use max utility if not random
                    for key, boardlist in agent_opponent.q_table.items():
                        ind = 0
                        for board, value in boardlist:
                            if (game_board.current_state == board):  # found the correct board, add to
                                if (value > utility_value3):
                                    utility_value3 = value  # set new utility value
                                    index_of_selected_state3 = ind
                                    i3, j3 = key  # index of cell corresponds to maximum utility so far
                                break
                            ind += 1
                game_board.make_move("O", i3 * 3 + j3)  # set O on (Take action A, observe R,S')
                #game_board.print_board()
                utility_value4 = -1000
                i4 = -1
                j4 = -1
                index_of_selected_state4 = -1
                if (game_board.check_finish() == "O"):  # if game finished with opponent's move
                    selected_state_board, _ = agent_opponent.q_table[(i3, j3)][index_of_selected_state3]
                    new_utility = (1 - alpha) * utility_value3 + alpha  # reward = 1 Q(S',a) = 0
                    agent_opponent.q_table[(i3, j3)][index_of_selected_state3] = (selected_state_board, new_utility)  # Q(S,A) = (1-a)Q(S,A) + a[R + gammaQ(S',A')]
                    _,util = agent.q_table[(i3,j3)][index_of_selected_state3]
                    new_utility2 = (1 - alpha) * util - alpha# reward -1
                    agent.q_table[(i3,j3)][index_of_selected_state3] = (selected_state_board,new_utility2)
                    break
                elif( game_board.check_finish() == "Draw"):
                    selected_state_board, _ = agent_opponent.q_table[(i3, j3)][index_of_selected_state3]
                    new_utility = (1 - alpha) * utility_value3  # reward = 0 Q(S',a) = 0
                    agent_opponent.q_table[(i3, j3)][index_of_selected_state3] = (selected_state_board, new_utility)  # Q(S,A) = (1-a)Q(S,A) + a[R + gammaQ(S',A')]
                    _, util = agent.q_table[(i3, j3)][index_of_selected_state3]
                    new_utility2 = (1 - alpha) * util  # reward 0
                    agent.q_table[(i3, j3)][index_of_selected_state3] = (selected_state_board, new_utility2)
                    break
                else:  # if game is not finished with this action
                    # use max utility since Q-learning
                    for key, boardlist in agent_opponent.q_table.items():
                        ind = 0
                        for board, value in boardlist:
                            if (game_board.current_state == board):  # found the correct board, add to
                                if (value > utility_value4):
                                    utility_value4 = value  # set new utility value
                                    index_of_selected_state4 = ind
                                    i4, j4 = key  # index of cell corresponds to maximum utility so far
                                break
                            ind += 1
                    selected_state_board, _ = agent_opponent.q_table[(i3, j3)][index_of_selected_state3]
                    new_utility = (1 - alpha) * utility_value3 + alpha * (gamma * utility_value4)  # reward = 0
                    agent_opponent.q_table[(i3, j3)][index_of_selected_state3] = (selected_state_board, new_utility)  # Q(S,A) = (1-a)Q(S,A) + a[R + gammaQ(S',A')]

                """
                O's q_table set, now set X's
                """
                utility_value2 = -1000
                i2 = -1
                j2 = -1
                index_of_selected_state2 = -1
                if (game_board.check_finish() == "X"):  # if game finished with my move
                    selected_state_board, _ = agent.q_table[(i, j)][index_of_selected_state]
                    new_utility = (1 - alpha) * utility_value + alpha  # reward = 1 Q(S',A') = 0
                    agent.q_table[(i, j)][index_of_selected_state] = (selected_state_board, new_utility)  # Q(S,A) = (1-a)Q(S,A) + a[R + gammaQ(S',A')]
                    _,util = agent_opponent.q_table[(i,j)][index_of_selected_state]
                    new_utility2 = (1 - alpha) * util - alpha# reward -1
                    agent_opponent.q_table[(i,j)][index_of_selected_state] = (selected_state_board,new_utility2)
                    break
                elif( game_board.check_finish() == "Draw"): # draw
                    selected_state_board, _ = agent.q_table[(i, j)][index_of_selected_state]
                    new_utility = (1 - alpha) * utility_value  # reward = 0 Q(S',A') = 0
                    agent.q_table[(i, j)][index_of_selected_state] = (selected_state_board, new_utility)  # Q(S,A) = (1-a)Q(S,A) + a[R + gammaQ(S',A')]
                    _,util = agent_opponent.q_table[(i,j)][index_of_selected_state]
                    new_utility2 = (1 - alpha) * util # reward 0
                    agent_opponent.q_table[(i,j)][index_of_selected_state] = (selected_state_board,new_utility2)
                    break
                else:  # if game is not finished with this action
                    if random.random() <= epsilon:
                        possible_actions = game_board.get_all_available_cells()
                        random_index = np.random.randint(0, len(possible_actions))
                        index_of_cell = possible_actions[random_index]  # get the index (i*3+j) from possible list
                        i2 = int(index_of_cell / 3)  # row index
                        j2 = index_of_cell % 3  # column index
                        ind = 0
                        for board, value in agent.q_table[(i2, j2)]:
                            if (game_board.current_state == board):  # found the correct index and board
                                utility_value2 = value  # set utility
                                index_of_selected_state2 = ind  # set index
                                break
                            ind += 1
                    else:  # use max utility if not random
                        for key, boardlist in agent.q_table.items():
                            ind = 0
                            for board, value in boardlist:
                                if (game_board.current_state == board):  # found the correct board, add to
                                    if (value > utility_value2):
                                        utility_value2 = value  # set new utility value
                                        index_of_selected_state2 = ind
                                        i2, j2 = key  # index of cell corresponds to maximum utility so far
                                    break
                                ind += 1
                    selected_state_board, _ = agent.q_table[(i, j)][index_of_selected_state]
                    new_utility = (1 - alpha) * utility_value + alpha * (gamma * utility_value2)  # reward = 0
                    agent.q_table[(i, j)][index_of_selected_state] = (selected_state_board, new_utility)  # Q(S,A) = (1-a)Q(S,A) + a[R + gammaQ(S',A')]
                i = i2 # S <= S'
                j = j2 # S <= S'
                index_of_selected_state = index_of_selected_state2 # A <= A'
                utility_value = utility_value2
        return agent.q_table


    elif method_name == "Q-learning":
        agent = Agent("X")
        agent_opponent = Agent("O")
        for i in range(3):
            for j in range(3):
                board_alternatives = []  # all possible board alternatives for each cell
                for board in all_possible_boards:
                    if (i * 3 + j != 8):
                        temp = board[:i * 3 + j] + ["-"] + board[i * 3 + j:]
                    else:  # index out of range check
                        temp = board[:i * 3 + j] + ["-"]
                    listToStr = ''.join([elem for elem in temp])
                    if not is_finished_states(listToStr):  # if game state is not finished add to board list
                        board_alternatives.append((listToStr, 0.0))

                agent.q_table[(i, j)] = copy.deepcopy(board_alternatives)
                agent_opponent.q_table[(i, j)] = copy.deepcopy(board_alternatives)
        # print(agent_opponent.q_table)
        """
        Agents' initialization fully complete so far including their q_tables, both opponent and player
        """
        for episode in range(epsilon_count):  # total episodes
            #print("Episode Number: ",episode)
            game_board = GameBoard()  # initialize game board (S)
            utility_value3 = -1000  # utility of O
            i3 = -1  # index of O
            j3 = -1  # index of O
            index_of_selected_state3 = -1  # index of O
            if random.random() <= epsilon:
                possible_actions = game_board.get_all_available_cells()
                random_index = np.random.randint(0, len(possible_actions))
                index_of_cell = possible_actions[random_index]  # get the index (i*3+j) from possible list
                i3 = int(index_of_cell / 3)  # row index
                j3 = index_of_cell % 3  # column index
                ind = 0
                for board, value in agent.q_table[(i3, j3)]:
                    if (game_board.current_state == board):  # found the correct index and board
                        utility_value3 = value  # set utility
                        index_of_selected_state3 = ind  # set index
                        break
                    ind += 1
            else:  # use max utility if not random
                for key, boardlist in agent.q_table.items():
                    ind = 0
                    for board, value in boardlist:
                        if (game_board.current_state == board):  # found the correct board, add to
                            if (value > utility_value3):
                                utility_value3 = value  # set new utility value
                                index_of_selected_state3 = ind
                                i3, j3 = key  # index of cell corresponds to maximum utility so far
                            break
                        ind += 1
            game_board.make_move("X", i3 * 3 + j3)  # set O on (Take action A, observe R,S')
            # game_board.print_board()
            utility_value4 = -1000
            i4 = -1
            j4 = -1
            index_of_selected_state4 = -1
            if (game_board.check_finish() == "X"):  # if game finished with opponent's move
                selected_state_board, _ = agent.q_table[(i3, j3)][index_of_selected_state3]
                new_utility = (1 - alpha) * utility_value3 + alpha  # reward = 1 Q(S',a) = 0
                agent.q_table[(i3, j3)][index_of_selected_state3] = (selected_state_board, new_utility)  # Q(S,A) = (1-a)Q(S,A) + a[R + gammaQ(S',A')]
                _, util = agent_opponent.q_table[(i3, j3)][index_of_selected_state3]
                new_utility2 = (1 - alpha) * util - alpha  # reward -1
                agent_opponent.q_table[(i3, j3)][index_of_selected_state3] = (selected_state_board, new_utility2)
                break
            elif (game_board.check_finish() == "Draw"):
                selected_state_board, _ = agent.q_table[(i3, j3)][index_of_selected_state3]
                new_utility = (1 - alpha) * utility_value3  # reward = 0 Q(S',a) = 0
                agent.q_table[(i3, j3)][index_of_selected_state3] = (selected_state_board, new_utility)  # Q(S,A) = (1-a)Q(S,A) + a[R + gammaQ(S',A')]
                _, util = agent_opponent.q_table[(i3, j3)][index_of_selected_state3]
                new_utility2 = (1 - alpha) * util  # reward 0
                agent_opponent.q_table[(i3, j3)][index_of_selected_state3] = (selected_state_board, new_utility2)
                break
            else:  # if game is not finished with this action
                # use max utility since Q-learning
                for key, boardlist in agent.q_table.items():
                    ind = 0
                    for board, value in boardlist:
                        if (game_board.current_state == board):  # found the correct board, add to
                            if (value > utility_value4):
                                utility_value4 = value  # set new utility value
                                index_of_selected_state4 = ind
                                i4, j4 = key  # index of cell corresponds to maximum utility so far
                            break
                        ind += 1
                selected_state_board, _ = agent.q_table[(i3, j3)][index_of_selected_state3]
                new_utility = (1 - alpha) * utility_value3 + alpha * (gamma * utility_value4)  # reward = 0
                agent.q_table[(i3, j3)][index_of_selected_state3] = (selected_state_board, new_utility)  # Q(S,A) = (1-a)Q(S,A) + a[R + gammaQ(S',A')]

            """
            X moved once. now in the loop, we go like O - X - O - X respectively
            """
            utility_value = -1000
            i = -1
            j = -1
            index_of_selected_state = -1
            if random.random() <= epsilon:
                possible_actions = game_board.get_all_available_cells()
                # print(possible_actions)
                random_index = np.random.randint(0, len(possible_actions))
                index_of_cell = possible_actions[random_index]  # get the index (i*3+j) from possible list
                i = int(index_of_cell / 3)  # row index
                j = index_of_cell % 3  # column index
                ind = 0
                for board, value in agent_opponent.q_table[(i, j)]:
                    if (game_board.current_state == board):  # found the correct index and board
                        utility_value = value  # set utility
                        index_of_selected_state = ind  # set index
                        break
                    ind += 1
            else:  # use max utility if not random
                for key, boardlist in agent_opponent.q_table.items():
                    ind = 0
                    for board, value in boardlist:
                        if (game_board.current_state == board):  # found the correct board, add to
                            if (value > utility_value):
                                utility_value = value  # set new utility value
                                index_of_selected_state = ind
                                i, j = key  # index of cell corresponds to maximum utility so far
                            break
                        ind += 1
            while True:  # loop for each episode
                game_board.make_move("O", i * 3 + j)  # set X on  (Take action A, observe R,S')
                #game_board.print_board()
                if (game_board.check_finish() == "O"):  # if game finished with my move
                    #print("1")
                    selected_state_board, _ = agent_opponent.q_table[(i, j)][index_of_selected_state]
                    new_utility = (1 - alpha) * utility_value + alpha  # reward = 1 Q(S',A') = 0
                    agent_opponent.q_table[(i, j)][index_of_selected_state] = (selected_state_board, new_utility)  # Q(S,A) = (1-a)Q(S,A) + a[R + gammaQ(S',A')]
                    _,util = agent.q_table[(i,j)][index_of_selected_state]
                    new_utility2 = (1 - alpha) * util - alpha# reward -1
                    agent.q_table[(i,j)][index_of_selected_state] = (selected_state_board,new_utility2)
                    break
                elif( game_board.check_finish() == "Draw"): # draw
                    #print("2")
                    selected_state_board, _ = agent_opponent.q_table[(i, j)][index_of_selected_state]
                    new_utility = (1 - alpha) * utility_value  # reward = 0 Q(S',A') = 0
                    agent_opponent.q_table[(i, j)][index_of_selected_state] = (selected_state_board, new_utility)  # Q(S,A) = (1-a)Q(S,A) + a[R + gammaQ(S',A')]
                    _,util = agent.q_table[(i,j)][index_of_selected_state]
                    new_utility2 = (1 - alpha) * util # reward 0
                    agent.q_table[(i,j)][index_of_selected_state] = (selected_state_board,new_utility2)
                    break
                """
                Player O(SARSA) played 1 move, now player X turn (Q-learning) 
                """
                utility_value3 = -1000  # utility of O
                i3 = -1  # index of O
                j3 = -1  # index of O
                index_of_selected_state3 = -1  # index of O
                if random.random() <= epsilon:
                    possible_actions = game_board.get_all_available_cells()
                    random_index = np.random.randint(0, len(possible_actions))
                    index_of_cell = possible_actions[random_index]  # get the index (i*3+j) from possible list
                    i3 = int(index_of_cell / 3)  # row index
                    j3 = index_of_cell % 3  # column index
                    ind = 0
                    for board, value in agent.q_table[(i3, j3)]:
                        if (game_board.current_state == board):  # found the correct index and board
                            utility_value3 = value  # set utility
                            index_of_selected_state3 = ind  # set index
                            break
                        ind += 1
                else:  # use max utility if not random
                    for key, boardlist in agent.q_table.items():
                        ind = 0
                        for board, value in boardlist:
                            if (game_board.current_state == board):  # found the correct board, add to
                                if (value > utility_value3):
                                    utility_value3 = value  # set new utility value
                                    index_of_selected_state3 = ind
                                    i3, j3 = key  # index of cell corresponds to maximum utility so far
                                break
                            ind += 1
                game_board.make_move("X", i3 * 3 + j3)  # set O on (Take action A, observe R,S')
                # game_board.print_board()
                utility_value4 = -1000
                i4 = -1
                j4 = -1
                index_of_selected_state4 = -1
                if (game_board.check_finish() == "X"):  # if game finished with opponent's move
                    selected_state_board, _ = agent.q_table[(i3, j3)][index_of_selected_state3]
                    new_utility = (1 - alpha) * utility_value3 + alpha  # reward = 1 Q(S',a) = 0
                    agent.q_table[(i3, j3)][index_of_selected_state3] = (
                    selected_state_board, new_utility)  # Q(S,A) = (1-a)Q(S,A) + a[R + gammaQ(S',A')]
                    _, util = agent_opponent.q_table[(i3, j3)][index_of_selected_state3]
                    new_utility2 = (1 - alpha) * util - alpha  # reward -1
                    agent_opponent.q_table[(i3, j3)][index_of_selected_state3] = (selected_state_board, new_utility2)
                    break
                elif (game_board.check_finish() == "Draw"):
                    selected_state_board, _ = agent.q_table[(i3, j3)][index_of_selected_state3]
                    new_utility = (1 - alpha) * utility_value3  # reward = 0 Q(S',a) = 0
                    agent.q_table[(i3, j3)][index_of_selected_state3] = (
                    selected_state_board, new_utility)  # Q(S,A) = (1-a)Q(S,A) + a[R + gammaQ(S',A')]
                    _, util = agent_opponent.q_table[(i3, j3)][index_of_selected_state3]
                    new_utility2 = (1 - alpha) * util  # reward 0
                    agent_opponent.q_table[(i3, j3)][index_of_selected_state3] = (selected_state_board, new_utility2)
                    break
                else:  # if game is not finished with this action
                    # use max utility since Q-learning
                    for key, boardlist in agent.q_table.items():
                        ind = 0
                        for board, value in boardlist:
                            if (game_board.current_state == board):  # found the correct board, add to
                                if (value > utility_value4):
                                    utility_value4 = value  # set new utility value
                                    index_of_selected_state4 = ind
                                    i4, j4 = key  # index of cell corresponds to maximum utility so far
                                break
                            ind += 1
                    selected_state_board, _ = agent.q_table[(i3, j3)][index_of_selected_state3]
                    new_utility = (1 - alpha) * utility_value3 + alpha * (gamma * utility_value4)  # reward = 0
                    agent.q_table[(i3, j3)][index_of_selected_state3] = (
                    selected_state_board, new_utility)  # Q(S,A) = (1-a)Q(S,A) + a[R + gammaQ(S',A')]

                """
                X's q_table set, now set O's
                """
                utility_value2 = -1000
                i2 = -1
                j2 = -1
                index_of_selected_state2 = -1
                if (game_board.check_finish() == "O"):  # if game finished with my move
                    selected_state_board, _ = agent_opponent.q_table[(i, j)][index_of_selected_state]
                    new_utility = (1 - alpha) * utility_value + alpha  # reward = 1 Q(S',A') = 0
                    agent_opponent.q_table[(i, j)][index_of_selected_state] = (
                    selected_state_board, new_utility)  # Q(S,A) = (1-a)Q(S,A) + a[R + gammaQ(S',A')]
                    _, util = agent.q_table[(i, j)][index_of_selected_state]
                    new_utility2 = (1 - alpha) * util - alpha  # reward -1
                    agent.q_table[(i, j)][index_of_selected_state] = (selected_state_board, new_utility2)
                    break
                elif (game_board.check_finish() == "Draw"):  # draw
                    selected_state_board, _ = agent_opponent.q_table[(i, j)][index_of_selected_state]
                    new_utility = (1 - alpha) * utility_value  # reward = 0 Q(S',A') = 0
                    agent_opponent.q_table[(i, j)][index_of_selected_state] = (
                    selected_state_board, new_utility)  # Q(S,A) = (1-a)Q(S,A) + a[R + gammaQ(S',A')]
                    _, util = agent.q_table[(i, j)][index_of_selected_state]
                    new_utility2 = (1 - alpha) * util  # reward 0
                    agent.q_table[(i, j)][index_of_selected_state] = (selected_state_board, new_utility2)
                    break
                else:  # if game is not finished with this action
                    if random.random() <= epsilon:
                        possible_actions = game_board.get_all_available_cells()
                        random_index = np.random.randint(0, len(possible_actions))
                        index_of_cell = possible_actions[random_index]  # get the index (i*3+j) from possible list
                        i2 = int(index_of_cell / 3)  # row index
                        j2 = index_of_cell % 3  # column index
                        ind = 0
                        for board, value in agent_opponent.q_table[(i2, j2)]:
                            if (game_board.current_state == board):  # found the correct index and board
                                utility_value2 = value  # set utility
                                index_of_selected_state2 = ind  # set index
                                break
                            ind += 1
                    else:  # use max utility if not random
                        for key, boardlist in agent_opponent.q_table.items():
                            ind = 0
                            for board, value in boardlist:
                                if (game_board.current_state == board):  # found the correct board, add to
                                    if (value > utility_value2):
                                        utility_value2 = value  # set new utility value
                                        index_of_selected_state2 = ind
                                        i2, j2 = key  # index of cell corresponds to maximum utility so far
                                    break
                                ind += 1
                    selected_state_board, _ = agent_opponent.q_table[(i, j)][index_of_selected_state]
                    new_utility = (1 - alpha) * utility_value + alpha * (gamma * utility_value2)  # reward = 0
                    agent_opponent.q_table[(i, j)][index_of_selected_state] = (selected_state_board, new_utility)  # Q(S,A) = (1-a)Q(S,A) + a[R + gammaQ(S',A')]
                i = i2  # S <= S'
                j = j2  # S <= S'
                index_of_selected_state = index_of_selected_state2  # A <= A'
                utility_value = utility_value2
        return agent.q_table
    else:
        return


#print(SolveMDP("SARSA", "mdp1.txt", 37))
#print(SolveMDP("Q-learning","mdp1.txt",462))
