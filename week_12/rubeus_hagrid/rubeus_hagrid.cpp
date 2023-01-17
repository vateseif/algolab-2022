///4
#include <vector>
#include <algorithm>
#include <iostream>
#include <CGAL/Gmpz.h>

#define trace(x) std::cerr << #x << " = " << x<< std::endl;

std::vector<long> gold;
std::vector<std::vector<std::pair<long, long>>> edges; // (v, cost)

struct Subtree{
  long n;
  long t, g; // number of nodes, time needed, gold obtained (time sensitive)
  Subtree(){};
  Subtree(long ni, long ti, long gi){
    n = ni; t = ti; g = gi;
  }
};


Subtree dp(long node, long t0){
  long t = t0; // time needed to explore subtree
  long n = 1; // nodes of subtree
  long g = gold[node] - t0; // gold of subtree
  // if no neighbors
  if (edges[node].empty()){
    return Subtree(n, t, g);
  }
  long n_trees = 0;
  std::vector<Subtree> next_trees;
  for (std::pair<long, long> vl : edges[node]){
    Subtree tree = dp(vl.first, t0+vl.second);
    tree.t += vl.second; // add time to come back from subtree
    next_trees.push_back(tree);
    n_trees++;
  }
  
  // sort trees by time * n_nodes
  std::sort(next_trees.begin(), next_trees.end(), [](Subtree a, Subtree b) {
      return a.t*b.n < b.t*a.n;   
  });
  
  // precompute sum of nodes
  std::vector<long> sum_n(n_trees, 0);
  for (int i=n_trees-1; i>0; i--) sum_n[i-1] = sum_n[i] + next_trees[i].n;

  // compute cumulative time and gold of current tree
  for (int i=0; i<n_trees; i++){
    n += next_trees[i].n;
    t += next_trees[i].t - t0;
    g += next_trees[i].g + (t0 - next_trees[i].t) * sum_n[i];
  } 

  return Subtree(n, t, g);
}

void testcase(){
  int n; std::cin >> n;

  gold.clear(); gold.resize(n+1, 0);
  for (int i=1; i<n+1; i++){
    long gi; 
    std::cin >> gi;
    gold[i] = gi;
  }

  edges.clear(); edges.resize(n+1);
  for (int i=0; i<n; i++){
    long u, v;
    long l;
    std::cin >> u >> v >> l;
    edges[u].push_back(std::make_pair(v, l));
  }

  std::cout << dp(0,0).g << std::endl;

  return;
}


int main(){
  std::ios_base::sync_with_stdio(false);  
  int T; std::cin >> T;
  while (T--) testcase();
  return 0;
}