import random
from math import exp, log
import numpy as np
from random import uniform
import matplotlib.pyplot as plt


def mse(w1, w2, X, Y):
    loss = 0
    for x, y in zip(X, Y):
        y_h = 1 / (1 + exp(-w2 * (1 / (1 + exp(-w1 * x)))))
        loss += y * (1 - y_h) ** 2 + (1 - y) * (y_h ** 2)
    return loss / len(X)


def ce(w1, w2, X, Y):
    loss = 0
    for x, y in zip(X, Y):
        y_h = 1 / (1 + exp(-w2 * (1 / (1 + exp(-w1 * x)))))
        loss = -(y * log(y_h)) - (1 - y) * log(1 - y_h)
    return loss / len(X)


def data(size):
    X = []
    Y = []
    for i in range(size):
        x = round(uniform(-1, 1), 4)
        X.append(x)
        # if 3 * x ** 3 + 2 * x ** 2 + 4 * x + 1 > 0:
        #     Y.append(0)
        # else:
        #     Y.append(1)
        Y.append(random.randint(0,1))
    return X, Y

X, Y = data(100)

w1 = np.linspace(-20,20,100)
w2 = np.linspace(-20,20,100)
W1, W2 = np.meshgrid(w1, w2)
Loss = np.zeros(W1.shape)
for i in range(W1.shape[0]):
    for j in range(W1.shape[1]):
        Loss[i][j] = ce(W1[i][j], W2[i][j], X, Y)

fig = plt.figure()
ax = plt.axes(projection='3d')

ax.plot_surface(W1, W2, Loss,cmap='viridis', edgecolor='none')
ax.set_xlabel('w1')
ax.set_ylabel('w2')
ax.set_zlabel('loss')
ax.set_title('ce')
plt.show()