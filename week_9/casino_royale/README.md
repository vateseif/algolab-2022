# casino royale

## Problem description

$m$ missions are given. Each mission is described by (start, stop, cost) indicating where an agent goes on and leaves the train and which cost is received for completing the mission.

We need to determine the maximum cost achievable by selecting missions such that at any point there are no more than $l$ agents on the train along its $n$ stops.

## Solution

From the problem description it seems obvious that the problem has to be solved as a *MinCostMaxFlow* problem, thus a graph needs to be constructed.

It is to be noted that *MinCostMaxFlow* prioritizes the maxFlow and only then picks the maxFlow with minCost. Depending on how the graph is constructed we may risk our algorithm to prioritize the number of missions selected and only then to focus on the cost.

### Flow capacities

For such reason, a graph can be constructed where the maxFlow is constant. The nodes of the graph are all the $n$ stops of the train + a source and a sink node. Then create the following edges:

- source → stop_0: with capacity $l$
- stop_i → stop_i+1: with capacity $l \hspace{1cm} \forall i \in [n-2]$
- stop_n-1 → sink: with capacity $l$

The maxFlow for this graph is constant and it’s $n$.

We need to find some way to include the missions. This can be easily done by creating edges that connect the start → stop for each of the $m$ missions with capacity 1.

The flow of adjacent stops (with max capacity $l$) can now be seen as the number of agents left that can still go on the train. If a mission is selected, the flow along edges of adjacent stops will be decreased by 1 from the start of the mission up to the end stop.

### Cost

There are 2 ways to maximize the sum of costs:

1)
Use following costs for each edge:

- source → stop_0:   $0$
- stop_n-1 → sink:     $0$
- stop_i → stop_i+1:  0  $\forall i \in [n-2]$
- start_j → stop_j:     -cost  $\forall$  mission

We can then solve with `cycle_cancelling` and the minCost will be the negative of the value we’re looking for. This way is too slow for the last testcase.

2)

We need to reformulate the costs such that we can use `successive_shortest_path_nonnegative_weights` as it’s faster than the previous method but requires all costs to be nonnegative.

We also know that the maximum possible cost per mission is $2^7$ so we know that for each mission $2^7$  - cost $\geq 0$.

This also means that picking the missions that minimze the residual of the cost ($2^7$ -cost) is equal to picking the missions that maximize the cost. 
Hence we can use the following as edge costs:

We can use:

- source → stop_0:   $0$
- stop_n-1 → sink:     $0$
- stop_i → stop_i+1:  $2^7-0=2^7$  $\forall i \in [n-2]$
- start_j → stop_j:    (stop_j-start_j) $\cdot  2^7$ - cost  $\forall$  mission

Note: the multiplication of the max cost by (stop_j-start_j) indicates the number of traits the agent  is on the train.

The reason for that is that we’re now working with the residual cost. In fact, imagine the case where one agent starts at 0 and stops at 2 and the train can have only 1 person and also stops at 2. We can not choose the agent and the total residual cost will be $2 \cdot 2^7$ or we can select the mission and the total residual cost will be $(2 \cdot2^7)$ - cost. 
The pattern can be seen as: when a mission is selected, we ought to remove from the max residual cost, the cost of taking the mission.

Thus we can compute the solution of the problem as:

 $(n-1)\cdot l \cdot 2^7$ - minCost(Graph)

$(n-1)\cdot l \cdot 2^7$ can be interpreted as the max residual cost when no mission is taken.

MinCost(Graph) can be interpreted as: $(n-1) \cdot l \cdot 2^7 - \sum_j^m \text {cost}_j \cdot x_j$

Where $x_j = 0\ \text{or}\ 1$ indicates whether mission $j$ is selected.