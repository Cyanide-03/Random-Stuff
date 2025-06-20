import gymnasium as gym # type: ignore
import numpy as np
import cv2 # type: ignore
import pickle as pkl

# Parameters
EPS = 0.1
ALPHA = 0.1
GAMMA = 0.9
NUM_EPISODES = 500

env = gym.make('CliffWalking-v0')

q_table=np.zeros((48,4))

def policy(state,explore_rate=0.0):
    action=int(np.argmax(q_table[state]))
    if np.random.random() <= explore_rate:
        action=np.random.randint(0,4)
    return action

for episode in range(NUM_EPISODES):
    terminated=False
    total_reward=0
    episode_length=0

    state,_=env.reset()

    while not terminated:
        action=policy(state,EPS)
        next_state,reward,terminated,truncated,info=env.step(action)    
        next_action=policy(next_state)
        q_table[state][action]+=ALPHA*(reward+GAMMA*q_table[next_state][next_action]-q_table[state][action])

        state=next_state

        total_reward += reward
        episode_length += 1
    print("Episode:", episode, "Episode Length:", episode_length, "Total Reward: ", total_reward)

env.close()

pkl.dump(q_table, open("q_learning_q_table.pkl", "wb"))
print("Training Complete. Q Table Saved")