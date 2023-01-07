# asterix the gaul

## Problem

We are given a set of moves $M$ where each move is represented by $(d, t)$. We are also given a list of $m$ ordered numbers $1 \leq s_0 \leq s_1 \leq ... \leq s_{m-1}$.

The task is that of finding whether a subset $M'$ of $M$  can be found such that $\sum_{M'} d' \geq D$ and $\sum_{M'} t' < T$.

If such a subset doesn’t exist we are asked to find the smallest index $i\leq m$ such that:
$card(M')\cdot s_i + \sum_{M'}d' \geq D$  and $\sum_{M'} t' < T$. 
It is still possible that no index $i$ allows for it.

## Solution

The problem is NP-hard as we need to iterate over all combination of moves that can be picked. We can do so by having an integer (since $n \leq 32$ or we’d need a long) whose bits represent an item of the set $M$. We can then compute $\sum d'$ and $\sum t'$ of each subset. To pass the last testcase we’ll first need to use ***split and list*** so to reduce the time complexity of listing the possible subsets.

If a subset is found such that $\sum d'  \geq D$ and $\sum t' < T$ then we can output `0` .

If not, we need to carry out a binary search to find the minimum index $i$ such that
$card(M')\cdot s_i + \sum_{M'}d' \geq D$.

Ofc if $m=0$ this can’t be done and we need to output `“Panoramix captured”`.

Since the values of $s_i$ are sorted, we can start by setting 2 pointers `l=0` and `r=m` and compute their midpoint `i=(l+r)/2` .

After adding $s_i$ to all values of $d$ in the set $M$, we can check if there exists a subset such that $\sum d'  \geq D$ and $\sum t' < T$ then:

- if such subset exists, the minimum $i$ will be between `l` and the midpoint
- if such a subset doesn’t, the minimum may only be between the midpoint+1 and `r`.

We can carry this out until `l==r` at which point we’d have the minimum $i$. It’s possible that no subset is found and again we output `“Panoramix captured”`; this can be seen if `l=r=m`.

## Time complexity

Binary search over the optimal $i$ has complexity: $O(log\ m)$

Split and list has complexity: $O(n\ 2^{n/2})$

So the overall complexity is:

$$
O( 2^{\frac{n}{2}}\ n\ log\ m)
$$