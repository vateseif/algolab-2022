# ant challenge

Time: 2:30h
Score: 40 (without help)

### Solution

The idea is to construct a graph for each different specie, so that the crossable edges of every species can be found.

We are told that the network for each specie is established starting from the hive and opening the edge with lowest cost from the explored territory. 
Since all edges need to be reached for every network, the exploration is analogous to the minimum spanning tree. 

Once the edges for each network, we create a new graph with the smallest edge weight depending on which specie can traverse that edge and then run dijkstra to get the cost of the shortest path.

![Alt text](Screenshot_2023-01-03_at_21.44.02.png?raw=true "Optional Title")



### Complexity

For each of the $s$ species, we run MST on a graph with $n$ nodes and $m$ edges, so the complexity using `kruskal` for this is:

$$
O(s \cdot m\ log\ m)
$$

Then the shortest path complexity is:

$$
O(n\ log\ n + m)
$$

Since $m \leq \binom{n}{2}\leq124750$, the first part has expected time $\approx 0.7$ s

The second part instead has expected time $\approx 0.02$ s