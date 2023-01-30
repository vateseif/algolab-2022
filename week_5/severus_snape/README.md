# Severus Snape
Tags: greedy, dp, memo

## Problem
We are given $n$ poitons of type A that increase `p`, increase `h` and decrease `w`; we are also given $m$ potions of type B that decrease `p` and increase `w`.
Find the smallest number of potions (combined A and B) that are needed such that the sums of of p, h, w $\geq$ P, H, W. 


## Solution
It is crucial to notice that potions of type A increase `h` and potions of type B do not affect it. Thus we can construct a memo that tells us for each number of potions of type A `i = 0...n` the maxium sum of `p` for sum of `h>=H`.

In particular we define `memo[i][j][h]` as the maximum sum of `p` taking the first `i` out of the first `j` potions with sum of happiness `>=h`.

We start by determining `memo[1][j][h]` for each `h=0...H` and `j=1...n`. For each increment of `j` we check if the `jth` potion has happiness `>=h` and if so we apply:\
`memo[1][j][h] = max(p[j], memo[1][j-1][h])`

Then, for each `h=0...H`, `j=2...n`, `i=2...j`, for each increment of j, we define `h_prev=max(0, h-h[j])` as the sum of happinness of the subset of potions that combined with potion `j` would satisfy being `>=h`. Then we apply:\
`memo[i][j][h] = max(memo[i][j][h] ,memo[i-1][j-1][h_prev] + p[j])`\
In essence this asks if potion `j` can be used with a subset of `i-1` potions up to potion `j-1` such that their sum of happiness is `>=h` and such that their sum of power is greater than using `i` potions up to potion `j-1`.

Now for each number of potions in B `nb=0...m` and number of potions in A `na=0...n`, we find output the first sum `na+nb` such that `memo[na][n][H]>=P+nb*b` and `sum_B[nb]>=W+na*a`.