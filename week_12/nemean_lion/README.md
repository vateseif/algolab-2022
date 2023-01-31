# Nemean Lion
Tags: triangulation, geometry
## Problem
Given a graph of $n$ locations, we are asked to find the number of admissible sets of size 2 or 3 for the minimum possible distance $d_k$. We also need to find the max number of admissible sets for distance $s$ and the max number of sets for any distance.

## Solution

### a2
This equals to the number of egdes in the triangulation with minimum distance. In fact the midpoint will have the same distance to the source and the target. Because we take the edge with minimum distance, ther exist no other point that has equal minimum distance to 2 nodes.

### a3
This equals to the number of faces whose dual circle has minimum radius among all faces. The center of the circle has the same distance to all vertices of the face. The exact kernel is needed for this.

### as
All points along the dual of an edge of the triangulation, have the same distance to the source and target points of the original edge. The dual of an edge can be a segment if the adjacent faces of the edge are both finite, or a ray if one face is infinite and the other is finite.
If the dual edge intersects the edge, an equidistant point on the dual edge has distance lower bounded by the middle of the original edge and upperbounded by the furthest point on the dual edge. For rays this may be infinite.
If the dual edge doesn't intersect the original edge, the lower bound is the closest point of the dual edge and the upper bound is the furthest point on the dual edge.
If this lower and upper bounds contain the distance $s$ then we increase `as` by 1.

### a_max
We can store all lower bounds from the previous search and all upper bounds that are not infinity (in case of rays). Via sliding window we can determine the max number of overlaps between upper and lower bounds and that represents the max number of admissible sets of size 2 for any arbitrary distance.