# augean stables

## Problem

For $n$ stalls we are given 5 parameters: $f_i, c_i, k_i, l_i, m_i$.

We are asked if there exist 3 real numbers  $0 \leq h_1, h_2, h_3 \leq1$ such that:

$$
f_i - h_1\cdot k_i - h_2\cdot l_i - h_3 \cdot m_i \leq c_i \hspace{1cm} \forall i \in [n]
$$

We are also given 2 lists of 24 values $a_1...a_{24}\ \text{and}\ p_1...p_{24}$. If the previous did not occur, we are asked to find the minimum $i+j$ such that there exist   $0 \leq h_1, h_2, h_3 \leq1$  and the following holds:

$$
f_i - h_1\cdot (k_i+a^2) - h_2\cdot (l_i+p^2) - h_3 \cdot (m_i+a\cdot p) \leq c_i \hspace{1cm} \forall i \in [n]
$$

Where $a = a_1+...+a_i\ \text{and}\ p = p_1+...+p_j$.

## Solution

We can construct an lp that given $a$ and $p$ determines whether there exist  $0 \leq h_1, h_2, h_3 \leq1$ such that:

$$
f_i - h_1\cdot (k_i+a^2) - h_2\cdot (l_i+p^2) - h_3 \cdot (m_i+a\cdot p) \leq c_i \hspace{1cm} \forall i \in [n]
$$

We now need to find a way to determine the min $i+j$ that would satisfy the above condition in case a solution wasn’t found for $a=0$ and $p = 0$.

Increasing $i$ or $j$ only increases $a$ and $p$ respectively as all values $a_i$ and $p_j$ are $\geq 0$ thus the sum is nondecreasing. For utility you can directly store the sum at each index.

Assume you have 2 pointers `l, r` and define their midpoint as `mid=(l+r)/2`, if for all combinations of $i+j$=`mid`, no $a$ and $p$ exists that solves the lp, then the min number that solves the lp may only reside in the range `(mid, r]`. Otherwise the min number will surely reside in the range `[l, mid]` and we can keep carrying this out until the min value is found.

There’s still an addition needed to pass the last testcase. In fact computing all combinations of $i+j=$`mid` is not efficient. If we have already computed $a$ and $p$ for $i$ and $j$ and the lp was unfeasible, we are guaranteed that solving the lp for $i$ and $j'\leq j$ will also be infeasible as $p$  will surely be smaller equal.
Thus we can have a `memo` which for each $i$ stores the highest $j$ that didn’t solve the lp. When doing binary search, we need to check the lp only for $j'\geq$ `memo[i]` .

## Time complexity

The LP has complexity of: $O(max(M,N)) \rightarrow O(n)$

We define $m = 48$ , the max sum $i+j$.

Binary search has complexity of: $O(log\ m)$

The overall complexity:

$$
O(n\ m\ log\ m)
$$