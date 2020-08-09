import networkx as nx
import matplotlib.pyplot as plt
import numpy as np
import sys
import random
import math
from sortedcontainers import SortedSet


if __name__ == "__main__":
    sys.setrecursionlimit(5000)
    fw = list()
    bfs = list()
    n = 5000
    p = math.log(n, math.e) / n
    p = p * 0.1
    edge_list = SortedSet()
    print(p)
    str = "{} {}\n"
    file = open(r"Give you local path", "w+")
    gr = nx.erdos_renyi_graph(n, p)
    list1 = [(k, v) for k, v in gr.adj.items()]
    for x, y in list1:
        for k, l in y.items():
            if edge_list.count((k+1, x+1)) == 0:
                edge_list.add((x+1, k+1))
                file.write(str.format(x+1, k+1))
    bc = nx.betweenness_centrality(gr)
    print(bc)