"""
Tic Tac Toe Player
"""

import math
import copy

X = "X"
O = "O"
EMPTY = None


def initial_state():
    """
    Returns starting state of the board.
    """
    return [[EMPTY, EMPTY, EMPTY],
            [EMPTY, EMPTY, EMPTY],
            [EMPTY, EMPTY, EMPTY]]

def player(board):
    """
    Returns player who has the next turn on a board.
    """
    if terminal(board):
        return None
    
    x_count=0;o_count=0
    for row in board:
        for cell in row:
            if cell==X:
                x_count+=1
            elif cell==O:
                o_count+=1
    if x_count>o_count:
        return O
    else:
        return X

def actions(board):
    """
    Returns set of all possible actions (i, j) available on the board.
    """
    if terminal(board):
        return set()
    
    possible_actions=set()
    for i in range(3):
        for j in range(3):
            if  board[i][j]==EMPTY:
                possible_actions.add((i,j))

    return possible_actions

def result(board, action):
    """
    Returns the board that results from making move (i, j) on the board.
    """

    if board[action[0]][action[1]] is not EMPTY:
        raise Exception("Invalid action")
    
    board_copy=copy.deepcopy(board)
    board_copy[action[0]][action[1]]=player(board)

    return board_copy
    
def winner(board):
    """
    Returns the winner of the game, if there is one.
    """
    for i in range(3):
         if board[i][0]==board[i][1]==board[i][2]!=EMPTY:
             return board[i][0]
         if board[0][i]==board[1][i]==board[2][i]!=EMPTY:
             return board[0][i]
         
    if board[0][0]==board[1][1]==board[2][2]!=EMPTY:
        return board[0][0]
    if board[0][2]==board[1][1]==board[2][0]!=EMPTY:
        return board[0][2]

def terminal(board):
    """
    Returns True if game is over, False otherwise.
    """
    for i in range(3):
         if board[i][0]==board[i][1]==board[i][2]!=EMPTY:
             return True
         if board[0][i]==board[1][i]==board[2][i]!=EMPTY:
             return True
         
    if board[0][0]==board[1][1]==board[2][2]!=EMPTY:
        return True
    if board[0][2]==board[1][1]==board[2][0]!=EMPTY:
        return True
    
    for row in board:
        for col in row:
            if col==EMPTY:
                return False
            
    return True

def utility(board):
    """
    Returns 1 if X has won the game, -1 if O has won, 0 otherwise.
    """
    if winner(board)==X:
        return 1
    elif winner(board)==O:
        return -1
    else:
        return 0

def minimax(board):
    """
    Returns the optimal action for the current player on the board.
    """
    if terminal(board):
        return None, utility(board)

    turn=player(board)
    possible_actions=actions(board)

    if turn==X:
        best_value=-math.inf
        best_action=None
        for action in possible_actions:
            new_board=result(board,action)
            _,value1=minimax(new_board)
            if value1>best_value:
                best_value=value1
                best_action=action
            
    else:
        best_value=math.inf
        best_action=None
        for action in possible_actions:
            new_board=result(board,action)
            _,value1=minimax(new_board)
            if value1<best_value:
                best_value=value1
                best_action=action

    return best_action, best_value