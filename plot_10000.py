import math
import matplotlib
import bisect
import matplotlib.pyplot as plt
import numpy as np


x = [0.5, 1, 3, 10, 50]
y = [14323279, 16356636, 17963588, 18915988, 18184762]
z = [6637445, 5737022, 5681577, 8482231, 9943447]
plt.yscale("log")
plt.xlabel("value of p")
plt.ylabel("Time taken")
# plotting the curve
plt.plot(x, y, label="FW")
plt.plot(x, z, label="BFS")

plt.legend()

plt.show()