# Boats

Tags: greedy

## Problem
We are given for `n` boats a position `p_i` and length `l_i`. We need to find the max number of boat such that not boats overlap and the position `p_i` is located within the boat length.

## Solution
The problem can be solved greedily.

We sort all boats increasingly by their position `p_i`. We iterate through each boat storing 2 values along the way: the rightmost point of the last added boat `r_prev` and the rightmost point of the current boat we're considering adding `r`.
The rightmost point is defined as `r_i = max(p_i, r_prev+l_i)` where `r_prev` is the rightmost point of the previously added boat.

If `r_i > p_j` then we know that we can't add both boat `i` and `j` since it would violate our constraint. We then need to consider adding only `i` or `j`. We take the one whose rightmost point is the smallest (wrt to `r_prev`). Thus we update `r = min(r_i, r_j)`.

If `r_i <= p_j` we know we can safely add boat `i` without constraint violation. Then we upadte `r_prev = r` and `r = r_j` and check whether to add boat `j`.