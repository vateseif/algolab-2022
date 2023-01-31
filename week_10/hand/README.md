# Hand
Tags: Triangulation, Binary Search

## Problem
Given $n$ tent locations, find $(f_0, s)$ and $(f, s_0)$ reasonable assignments such that each of the $f$ families has at least $k$ tents and tents of different families are never closer than $s$.

## Solution
For a given distance $s_0$ we can compute the connected components such that all nodes are closer than s and represent the tents that can belong to a single family. It's possible that some components contain less than $k$ tents, in such a case sort increasingly all components with less than $k$ tents and connect the ones with lowest tents to the one with highest tents until sum is $\geq k$.

For a given $f_0$ we can compute the components as before while doing binary search on the egdes of the triangulation. If a length $s$ is found such that $f \geq f_0$ then set `l = mid+1` otherwise set `r = mid`. The output is `s = egdes[l-1]`.