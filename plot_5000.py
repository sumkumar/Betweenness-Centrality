import math
import matplotlib
import bisect
import matplotlib.pyplot as plt
import numpy as np


x = [0.5, 0.75, 1, 3, 10, 20]
y = [1732675, 1989195, 2096663, 2205519, 2308505, 2274614]
z = [639830, 655994, , 700634, 854134, 899405]
plt.yscale("log")
plt.xlabel("value of p")
plt.ylabel("Time taken")
# plotting the curve
plt.plot(x, y, label="Nodes = 5000")

plt.legend()

plt.show()