# deck of cards

## Problem

We are given a set of $n$ numbers and a value $k$. We are asked to find the indices $i, j$ such that the  the following is minimized:

$$
e(i,j) = \left| k - \sum_i^j v_i \right|
$$

If there are multiple values of $i, j$ that minimize $e(i, j)$ we need to output the smallest lexixographically.

## Solution

We can use sliding window to solve the problem.
We need to define 2 pointers `i, j` and we  need a variable `s` to keep track of the sum $\sum_i^j v_i$.

Since the $n$ values are all positive, at fixed $i$, increasing $j$ will only increase `s`. If `s≥k` there is no point in further increasing `j` , thus we keep increasing `i` in case we find a smaller difference `e=|k-s|`.

In fact, update `i, j` only if a smaller error `e=|k-s|` is found, in this way we can guarantee that the output is the smallest lexicographically.

## Time complexity

 Since sliding window opens each value at most twice, the complexity is linear:

$$
O(n)
$$