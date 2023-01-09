# knights

## Problem

We can see the problem as a graph where each node represents an intersection and each hallway is an edge connecting 2 different intersections.

Given the position of $k$ knights located at different intersections, we are asked to find the max number of knights that can escape the labyrinth with the following conditions:

- each edge (hallway) can be traversed only once
- each intersection can host at most $c$ knights

## Solution

The problem sounds a lot like a max flow problem.

We need to construct a graph such that the conditions above hold and then we can in fact solve using max flow. To do so we need to note 2 points

1. For undirected graphs, enforcing the capacity of an edge equals to having 2 edges  $u \rightarrow v$  and  $v \rightarrow u$  both with the capacity of the undirected edge. If 2 knights take the 2 directed edges, the flows will cancel and it will equal to no knight taking that edge.
2. To enforce node capacities, for each intersection, we can have 2 nodes A and B. A indicates all the incoming flows and B all the outgoing flows. The capacity from A to B will then be $c$. When connecting 2 different intersections 1 and 2 we’ll create the following edges: 
1-B → 2-A    and     2-B → 1-A.

We can then connect from the source all intersections (their A node) where knights are present.
We connect all intersections (their B node) that are on the border of the labyrinth to the sink. Intersections that or in the corners will be connected with capacity 2 instead of 1.

## Time complexity

The complexity is that of solving the maxFlow with `push_relabel` so the practical complexity is:

$$
O((2 \cdot n \cdot m)^2)
$$

Where $n\cdot m$ is the number of intersections.