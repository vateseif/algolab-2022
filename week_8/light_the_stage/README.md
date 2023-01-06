# light the stage

## Solution

We are given $m$ disks with radius $r_j$ representing people. We are given $n$ lamps with radius $h$.

At each round a lamp is lit, meaning that if there’s an overlap between the players (disk) and the disk formed by the lamp (radius `h`) then the players are removed from the game. The players that last the longer win.

### Tc 1-3

For the case where at least 1 player is guaranteed not to overlap with any lamp, we can loop through each player, find the nearest lamp (because they have same radius) and determine if there’s an overlap. If not, the player is one of the winners.

### Tc 4-5

There’s the possibility that all players eventually overlap with a lamp. And the winners are those who are last hit by the lamp.

Doing a linear search to find the last players would take too much time and exceed time limits. Thus a binary search needs to be carried out:

- Start with `l = 0` , `r = n-1`
- while `l≤r`
    - compute `mid = (l+r)/2`
    - compute triangulation with lamps `[l, mid]` including mid
    - loop through players that didn’t loose yet and find if nearest lamp hits them
    - if all players are hit by lamps ()
        - the winners must have been eliminated in range `[l, mid)`
        - set `r = mid - 1`
    - if some players weren’t hit, among them are the winners
        - the winners are eliminated in range `(mid, r]`
        - set `l = mid + 1`

## Time complexity

Complexity is $O(n\ logn)$ for triangulation, $O(m\ logn)$ for querying nearest lamp to person and this is repeated at each binary search step with complexity $O(logn)$ so overall:

$$
O(log\ n\ ((n+m)\ log\ n))
$$