import math
import matplotlib
import bisect
import matplotlib.pyplot as plt
import numpy as np


x = [0.5, 1, 2]
y = [49243951, 55611515, 58402577]
z = [23835421, 18626846, 17988232]
plt.yscale("log")
plt.xlabel("value of p")
plt.ylabel("Time taken")
# plotting the curve
plt.plot(x, y, label="FW")
plt.plot(x, z, label="BFS")

plt.legend()

plt.show()