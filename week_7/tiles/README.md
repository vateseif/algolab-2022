# tiles

## Problem

We are given an $h$  x $w$ grid and we are told some of the squares within the grid are empty. We are asked whether an arrangement of 2 x 1 tiles (placed either vertically or horizontally) exists such that all non-empty squares are covered and all empty squares aren’t, without going over the grid.

## Solution

The problem can be solved in 2 different ways

### Maximum Matching

Placing a tile, covers 2 adjacent squares. If a tile is placed, no other tile can be placed that would cover any of the 2 squares. This sounds exactly like a maximum matching problem.

So we can construct a graph where all adjacent (horizontally and vertically) squares are connected by an edge, as long as any of the 2 squares is non-empty.

Then we can solve the problem with `edmonds_maximum_cardinality_matching` and if the matching size is equal half the number of empty tiles then the grid can be covered.

### Max Flow

If we look at the grid as a chessboard with alternating black and white squares, we can notice that any 2 x 1 tile, always covers a black and a white square.

We can create a bipartite graph with white squares connect to a source and black squares connected to the source (if they’re nonempty ofc). The edges have all edge capacity 1.

We then connect all white squares to their adjacent (horizontally and vertically) black squares with flow capacity of 1.

We then compute the maxFlow and it’s equal to half the number of nonempty squares than the grid can be covered.

## Time complexity

### Maximum matching

Using `edmonds_maximum_cardinality_matching` :

$$
O(mn · \alpha (m, n))
$$

### Max Flow

Using `push_relabel` the practical complexity is:

$$
O(n^2)
$$