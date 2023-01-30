# suez

## Problem definition

We are given the central location of rectangles as well as their side lengths ($w\ \text{and}\ h$). 

For $n$ rectangles we are able to choose a magnifying coefficient $a_i \geq 1$  such that its new side length are:  $\hspace{0.2cm} a_i \cdot w\ \text{and}\ a_i \cdot h \hspace{0.4cm} \forall i \in [n]$  

The magnifying coefficients shall be chosen such that no rectangle overlaps and the perimeter of the magnified rectangles is maximized.

## Mathematical modelling

The problem sounds like a typical LP, moreover we are told that $n \leq 30$, making it a suitable implementation also algorithmically.

## Algorithmic implementation

An optimization problem is defined by: decision variables, constraints and objective function.

### *Decision variables*

We are given the centre location of each rectangle, thus the only variable we have control over are the magnifying coefficients:

$$
a_0\ ...\ a_{n-1}
$$

### *Objective function*

We are told to maximize the perimeter of the magnified rectangles, thus the objective function will be:

$$
min -2\cdot(h+w)\sum_i a_i
$$

We are replacing $max$ with $min$ and adding a minus sign.

### *Constraints*

This is probably the hardest part of the exercise. We are required for rectangles not to overlap and we need to define constraints that would guarantee this.

Starting from testcase 1, the rectangles are guaranteed not to overlap with each other if the distance between their centre is bigger than the sum of half their width.

We define $d_x = |x_i - x_j|$ as the x distance between rectangles $i\ \text{and}\ j$. They don’t overlap if: 

$$
(a_i+a_j)\frac{w}{2} \leq d_x
$$

For the next testcases, where it may be $y_i \neq 0$, we need to reformulate the overlap condition. 

Since the centre positions are fixed, and both sides of the rectangles are magnified with the same coefficient $a_i$ we can formulate a similar constraint as above, however after some checks. 

In fact, 2 rectangles overlap if:   $(a_i+a_j)\frac{w}{2} \geq d_x$ **and** $(a_i+a_j)\frac{h}{2} \geq d_y$.

This means that they do not overlap if one **or** both of those conditions is not violated. However, union (**or**) is not a convex operator. It is important to notice that magnifying the sides with the same coefficient allows to determine which of the 2 conditions above is violated second. Until the second condition is violated, the rectangles are not overlapping. We can plot the distance in x and y direction of the closest vertices of 2 rectangles and determine which condition reaches 0 second.  The distances are respectively>  $d_x - (a_i+a_j)\frac{w}{2}$   and    $d_y - (a_i+a_j)\frac{h}{2}$.  The plot would look like this:

![Screenshot 2022-11-24 at 18.29.41.png](suez%20f65906e8a04444f799eb61e95abd8f50/Screenshot_2022-11-24_at_18.29.41.png)

It’s only once both distances reach values $\leq 0$ that the 2 rectangles are overlapping. Thus we only need to make sure that that the second one is not $\leq 0$.

We can then define the following constraint:

$$
(a_i+a_j)\frac{b}{2} \leq d \\
\\

\hspace{-3cm}\text{where} \\

d = \begin{cases}
d_{xij} &\quad \text{if} \quad \frac{2\cdot d_{xij}}{w} > \frac{2\cdot d_{yij}}{h}\\
d_{yij} &\quad \text{otherwise}
\end{cases} \\
b = \begin{cases}
w &\quad \text{if} \quad \frac{2\cdot d_{xij}}{w} > \frac{2\cdot d_{yij}}{h}\\
h &\quad \text{otherwise}
\end{cases}
$$

The same thing applies for rectangles whose lengths are fixed and we can simply substitute $a_j=1$.

### *Overall formulation*

 The overall optimization problem is formulated as:

$$
\min_{a_0...a_n-1} -2\cdot(h+w)\sum_i a_i \\
\hspace{-3cm} \text{s.t.} \\
(a_i+a_j)\frac{b}{2} \leq d \quad \forall i,j \in [n],\quad i\neq j\\
\hspace{1.7cm}(a_i+1)\frac{b}{2} \leq d \quad \forall i \in [n],\ \forall j=n...(n+m-1) \\
\\

\hspace{-5cm}\text{where} \\

d = \begin{cases}
d_{xij} &\quad \text{if} \quad \frac{2\cdot d_{xij}}{w} > \frac{2\cdot d_{yij}}{h}\\
d_{yij} &\quad \text{otherwise}
\end{cases} \\
b = \begin{cases}
w &\quad \text{if} \quad \frac{2\cdot d_{xij}}{w} > \frac{2\cdot d_{yij}}{h}\\
h &\quad \text{otherwise}
\end{cases}
$$

Here the indices of the $n$ magnified rectangles are $\forall i \in [n]$ while the fixed rectangles have indices $j = n...(n+m-1)$.

## Implementation and testing

The `i/o` types used are the following:

```cpp
typedef long IT;
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;
```

This is possible by rewriting the inequalities such that no fractions are present and we can then use integer types for faster computation:

$$
\min_{a_0...a_n-1} -2\cdot(h+w)\sum_i a_i \\
\hspace{-3cm} \text{s.t.} \\
(a_i+a_j)b \leq 2d \quad \forall i,j \in [n],\quad i\neq j\\
\hspace{1.7cm}(a_i+1)b \leq 2d \quad \forall i \in [n],\ \forall j=n...(n+m-1) \\
\\
$$

The first constraint, indicating that the magnified rectangles do not overlap is defined as follows:

```cpp
// new postesrs do not overlap
  for (int i=0; i<n-1; i++){
    for (int j=i+1; j<n; j++){
      long dx = std::abs(pn[i].first - pn[j].first);
      long dy = std::abs(pn[i].second - pn[j].second);
      long d, b;
      if (dx*h>dy*w){
        d = dx;
        b = w;
      }else{
        d = dy;
        b = h;
      }
      lp.set_a(i, n_c, b);
      lp.set_a(j, n_c, b);
      lp.set_b(n_c, 2*d);
      n_c++;
    }
  }
```

One thing to notice is that the constraint of not overlapping is reciprocal, so we do not need to define it twice for $i,j\ \text{then}\ j,i$.

Something similar is done for the constraint between the magnified and the fixed rectangles. It is to be noticed that $m \leq 10^3$ however for each $a_i$ only one constraint will hold while the other will be redundant. As it can be seen from the problem formulation, the constraint on $a_i$ can be rewritten as:

$$
a_i \leq \frac{2d}{b} - 1
$$

When looping over the fixed rectangles, we can add the constraint only if is stricter than the strictest one applied so far:

```cpp
for (int i=0; i<n; i++){
    long max_dx = std::numeric_limits<long>::max();
    long max_dy = std::numeric_limits<long>::max();
    for (int j=0; j<m; j++){
      long dx = std::abs(pn[i].first - po[j].first);
      long dy = std::abs(pn[i].second - po[j].second);
      long d, b;
      if (dx*h>dy*w){
        d = dx;
        b = w;
        if (d > max_dx) continue;
        max_dx = d;
      }else{
        d = dy;
        b = h;
        if (d > max_dy) continue;
        max_dy = d;
      }
      lp.set_a(i, n_c, b);
      lp.set_b(n_c, 2*d-b);
      n_c++;
    }
  }
```

The objective function is then trivial. The problem is correctly solved within time limit.

![Screenshot 2022-11-24 at 21.58.36.png](suez%20f65906e8a04444f799eb61e95abd8f50/Screenshot_2022-11-24_at_21.58.36.png)

## Challenges

1. It took me 2.30h to solve the problem, and I feel it’s one of the easiest. Do you have advice for improving my speed? It helped starting from the 1st testcase and then developing from there
2. The way the constraints between the magnified rectangles and the fixed ones, still has worst-case $m$  constraints which can go up to $10^3$ and would be too slow. Is a better solution computing all the distances, sorting them and then only adding the constraint with the shortest one (so the strictest)?

## Code

```cpp
#include <iostream>
#include <vector>
#include <cstdlib>
#include <climits>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

#define trace(x) std::cerr << #x << " = " << x << std::endl;

typedef long IT;
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

int n, m, h, w;
std::vector<std::pair<int, int>> po, pn;  // old and new posters (x, y)

double roundUp(CGAL::Quotient<ET> r){
  double out = std::ceil(CGAL::to_double(r));
  while (out < r) out +=1;
  while (out-1 >= r) out-=1;
  return out;
}

void objective(Program& lp){
  for (int i=0; i<n; i++){
    lp.set_c(i, -2*(h+w));
  }
}

void constraints(Program& lp){
  int n_c = 0; // counter of constraints
  // new postesrs do not overlap
  for (int i=0; i<n-1; i++){
    for (int j=i+1; j<n; j++){
      long dx = std::abs(pn[i].first - pn[j].first);
      long dy = std::abs(pn[i].second - pn[j].second);
      long d, b;
      if (dx*h>dy*w){
        d = dx;
        b = w;
      }else{
        d = dy;
        b = h;
      }
      lp.set_a(i, n_c, b);
      lp.set_a(j, n_c, b);
      lp.set_b(n_c, 2*d);
      n_c++;
    }
  }
  // old and new posters do not overlap
  for (int i=0; i<n; i++){
    long max_dx = std::numeric_limits<long>::max();
    long max_dy = std::numeric_limits<long>::max();
    for (int j=0; j<m; j++){
      long dx = std::abs(pn[i].first - po[j].first);
      long dy = std::abs(pn[i].second - po[j].second);
      long d, b;
      if (dx*h>dy*w){
        d = dx;
        b = w;
        if (d > max_dx) continue;
        max_dx = d;
      }else{
        d = dy;
        b = h;
        if (d > max_dy) continue;
        max_dy = d;
      }
      lp.set_a(i, n_c, b);
      lp.set_b(n_c, 2*d-b);
      n_c++;
    }
  }
}

void testcase(){
  std::cin >> n >> m >> h >> w;

  pn.clear(); pn.resize(n);
  for (int i=0; i<n; i++){
    int x, y;
    std::cin >> x >> y;
    pn[i] = std::make_pair(x, y);
  }

  po.clear(); po.resize(m);
  for (int i=0; i<m; i++){
    int x, y;
    std::cin >> x >> y;
    po[i] = std::make_pair(x, y);
  }
  // construct lp
  Program lp (CGAL::SMALLER, true, 1, false, 0); 
  constraints(lp);
  objective(lp);
  
  //solve
  Solution s = CGAL::solve_linear_program(lp, ET());
  std::cout << roundUp(-s.objective_value()) << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  std::cout << std::setprecision(17); // 17 for doubles
  int T; std::cin>>T;
  while(T--)testcase();
  return 0;
}
```