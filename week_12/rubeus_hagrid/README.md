# Rubeus Hagrid
Tags: greedy

## Problem
Given a directed tree with weighted nodes and edges, find the order of exploring the tree such that you can maximize the sum of weights given the fact that for each unit of time the weight of every unexplored node decreases by 1.

## Solution
Iterate over every node and for each node compute the subree of its neighbors. The subtree is defined as the number of nodes in the subtree, the time needed to explore it fully (back and forth) and the gold collected (sensitive of time).
Sort each subtree according to `t_i*n_j < t_j*n_i`. This is the optimal order of exploration. Every subtree explored (apart from the first) will have a decrease of gold due to the time needed to explore its predecessors and its gold will be decreased by its number of nodes times the sum of time needed to explore its predecessors.