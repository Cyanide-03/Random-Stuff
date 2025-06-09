import gymnasium as gym
import cv2
import tensorflow as tf
from keras import Model, Input
from keras.layers import Dense

env=gym.make('CartPole-v1')

# Q Network
input=Input(shape=(4,))
x=Dense(64,activation='relu')(input)
x=Dense(32,activation='relu')(x)
output=Dense(2,activation='linear')(x)
q_net=Model(inputs=input,outputs=output)

# Parameters
ALPHA=0.01
EPS=1
eps_decay=1.001
GAMMA=0.9
NUM_EPISODES=500

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
        action=policy(state,EPS)
        next_state,reward,terminated,truncated,info=env.step(action.numpy().item())  # Convert tensor to numpy for env.step
        next_state=tf.convert_to_tensor(next_state, dtype=tf.float32) 
        next_state=tf.expand_dims(next_state, axis=0)   
        next_action=policy(next_state,EPS)

        target=reward+GAMMA*q_net(next_state)[0][next_action]
        if terminated:
            target=reward

        with tf.GradientTape() as tape:
            q_values=q_net(state)

        grads=tape.gradient(q_values, q_net.trainable_weights)
        delta=target-q_values[0][action]

        for i in range(len(grads)):
            q_net.trainable_weights[i].assign_add(ALPHA * delta * grads[i])

        state=next_state
        action=next_action

        total_reward += reward
        episode_length += 1

    EPS /= eps_decay
    print(f"Episode: {episode}, Episode Length: {episode_length}, Total Reward: {total_reward}")

q_net.save("sarsa_qnet.h5")
env.close()