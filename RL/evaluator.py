import gymnasium as gym # type: ignore
import numpy as np
import cv2 # type: ignore
import pickle as pkl

# Parameters
EPS = 0.1
ALPHA = 0.1
GAMMA = 0.9 
NUM_EPISODES = 5

env = gym.make('CliffWalking-v0')

q_table=pkl.load(open("sarsa_q_table.pkl", "rb"))

# Handy functions for Visuals
def initialize_frame():
    width, height = 600, 200
    img = np.ones(shape=(height, width, 3)) * 255.0
    margin_horizontal = 6
    margin_vertical = 2

    # Vertical Lines
    for i in range(13):
        img = cv2.line(img, (49 * i + margin_horizontal, margin_vertical),
                       (49 * i + margin_horizontal, 200 - margin_vertical), color=(0, 0, 0), thickness=1)

    # Horizontal Lines
    for i in range(5):
        img = cv2.line(img, (margin_horizontal, 49 * i + margin_vertical),
                       (600 - margin_horizontal, 49 * i + margin_vertical), color=(0, 0, 0), thickness=1)

    # Cliff Box
    img = cv2.rectangle(img, (49 * 1 + margin_horizontal + 2, 49 * 3 + margin_vertical + 2),
                        (49 * 11 + margin_horizontal - 2, 49 * 4 + margin_vertical - 2), color=(255, 0, 255),
                        thickness=-1)
    img = cv2.putText(img, text="Cliff", org=(49 * 5 + margin_horizontal, 49 * 4 + margin_vertical - 10),
                      fontFace=cv2.FONT_HERSHEY_SIMPLEX, fontScale=1, color=(255, 255, 255), thickness=2)

    # Goal
    frame = cv2.putText(img, text="G", org=(49 * 11 + margin_horizontal + 10, 49 * 4 + margin_vertical - 10),
                        fontFace=cv2.FONT_HERSHEY_SIMPLEX, fontScale=1, color=(0, 0, 0), thickness=2)
    # Start
    # frame = cv2.putText(img, text="S", org=(49 * 0 + margin_horizontal + 10, 49 * 4 + margin_vertical - 10),
    #                     fontFace=cv2.FONT_HERSHEY_SIMPLEX, fontScale=1, color=(0, 0, 0), thickness=2)
    return frame

def put_agent(img, state):
    margin_horizontal = 6
    margin_vertical = 2
    row, column = np.unravel_index(indices=state, shape=(4, 12))
    cv2.putText(img, text="A", org=(49 * column + margin_horizontal + 10, 49 * (row + 1) + margin_vertical - 10),
                fontFace=cv2.FONT_HERSHEY_SIMPLEX, fontScale=1, color=(0, 0, 0), thickness=2)
    return img

def policy(state,explore_rate):
    action=int(np.argmax(q_table[state]))
    if np.random.random() <= explore_rate:
        action=np.random.randint(0,4)
    return action

for episode in range(10):
    terminated=False
    state,_=env.reset()
    action=policy(state,EPS)
    total_reward=0
    episode_length=0

    while not terminated:
        frame=initialize_frame()
        frame=put_agent(frame,state)
        cv2.imshow("Cliff Walking",frame)
        cv2.waitKey(250)

        next_state,reward,terminated,truncated,info=env.step(action)    
        next_action=policy(next_state,EPS)
        q_table[state][action]+=ALPHA*(reward+GAMMA*q_table[next_state][next_action]-q_table[state][action])

        state=next_state
        action=next_action

        total_reward += reward
        episode_length += 1
    print("Episode:", episode, "Episode Length:", episode_length, "Total Reward: ", total_reward)

env.close()