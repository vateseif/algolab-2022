# Placing Knights
Tags: min-cost\max-flow

## Problem
We are given a squared board of side $n$ and we are told that some of the squares in the board are missing. We need to find the maximum number of knights that can be placed on the board whithout any knight threatening another.

## Solution
It can be seen that knights on light squares cannot threaten knights on black squarers and viceversa. We can construct a graph with all squares and connect white squares (if present) to the source and black squares (if present) to the sink. For every white square we connect it to the black squares that the knight would target. The min-cut/max-flow of this graph equals to the minimum number of knights that needs to be removed such that there is no more collisions.

The final output equals `n*n - empty_squares - min_cut`