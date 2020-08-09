import math
import matplotlib
import bisect
import matplotlib.pyplot as plt
import numpy as np


x = [0.5, 1]
y = [118711694, 132442490]
z = []
plt.yscale("log")
plt.xlabel("value of p")
plt.ylabel("Time taken")
# plotting the curve
plt.plot(x, y, label="Nodes = 20000")

plt.legend()

plt.show()