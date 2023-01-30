# Moving Book

Tags: greedy

## Problem
There's $m$ friends with strength `s_i` each and $m$ books with weight `w_j` each. You need to find the minimum time to move all boxes such that a friend can carry boxes at most as heavy as his strength and it takes 2 units of time to carry a box down and 1 to come back up.

## Solution 1

Sort the friends decreasingly by strength and boxes increasingly by weight.
If the strongest friend can't take the heaviest box then output `"impossible"`.

Otherwise for each round, loop over each friend such that starting from the strongest they take the heaviest box they can. When a box is taken, erase it from the sorted vector. If for some friend `i` there's no box he can take, we can stop before checking for `i+1...n`.

The output is the `3*n_roudns-1`. In fact in the last round the friends don't have togo upstairs to pick any more boxes.

For this approach you need to store the boxes in a `multiset`, the reason is that the function `upper_bound` used to find the heaviest box pickable is more efficient for multisets than in the standard library.

## Solution 2
Sort friends and boxes decreasingly by strength and weight respectively.

Apart from the impossible case, we can run binary search. The smallest number of rounds needed equals to the case where each friend can pick a box at each round so `low = ceil(m/n)` and the highest is when only the strongest person has to pick all the boxes so `high = m`.

For each iteration of binary search, all boxes are moved in `t` rounds if every friend `i` has enough strength for all `[t*i, t*(i+1))` boxes, i.e. if he can lift box `t*i` because all previous boxes should have been taken by stronger friends.