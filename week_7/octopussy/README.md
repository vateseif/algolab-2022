# octopussy

## Problem

We are given a tree of $n$ nodes, with a branching factor of 2. Each node has a time $t_i$ and we are asked to determine whether each node can be opened (defused) before their respective time.

2 conditions are needed:

1. Opening a node takes 1 unit of time
2. A node can be opened iff its children nodes where opened.

## Solution

A greedy method can be used to solve the problem.

We sort all nodes increasingly by their time $t_i$, while also saving their index.

We target to open each node in the sorted list while keeping count of the number of nodes opened and which nodes have been opened.

When targeting to open a node, we first need to open all its unopened children, in fact we won’t find any unopened child with lower time as it must have been targeted earlier in the sorted list.

## Time complexity

$$
O(n)
$$