import gymnasium as gym
import cv2
import tensorflow as tf
from keras.models import load_model

env=gym.make('CartPole-v1',render_mode='rgb_array')

q_net=load_model("q_learning_qnet.h5")

# Parameters
ALPHA=0.01
EPS=1
eps_decay=1.001
GAMMA=0.9
NUM_EPISODES=5

def policy(state,explore=0.0):
    actions=tf.argmax(q_net(state)[0],output_type=tf.int32)
    if tf.random.uniform(shape=(),maxval=1) < explore:
        actions=tf.random.uniform(shape=(),minval=0,maxval=2,dtype=tf.int32)
    return actions

for episode in range(NUM_EPISODES):
    terminated=False
    total_reward=0
    episode_length=0

    state,_=env.reset()
    state=tf.convert_to_tensor(state, dtype=tf.float32)
    state=tf.expand_dims(state, axis=0)  # Add batch dimension

    while not terminated:
        frame=env.render()
        cv2.imshow("CartPole", frame)
        cv2.waitKey(100) 

        action=policy(state,EPS)
        state,reward,terminated,truncated,info=env.step(action.numpy().item())  # Convert tensor to numpy for env.step
        state=tf.convert_to_tensor(state, dtype=tf.float32) 
        state=tf.expand_dims(state, axis=0)   

env.close()