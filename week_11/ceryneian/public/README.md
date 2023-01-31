# Ceryneaian Hind

## Problem
Given a graph with weighted nodes, find a nonempty subset of nodes that maximizes the weight with the constraint that no node in the subset connects to a node in the complementary subset.

## Solution
This problem follows the closure problem. Connect each node with positive weight to the source and each node wth negative weigth tothe sink. If an edge goes from node u to v, add it to the graph with capacity $\infty$. The min-cut/max-flow of this problem will compute 2 separate complementary sets such that there's no edge going from the first to the second.
The output is the sum of all positive weighted nodes minus the min-cut. 