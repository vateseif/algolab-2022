///4
#include <iostream>
#include <algorithm>
#include <vector>

#define trace(x) std::cerr << #x << " = " << x << std::endl;

struct Tree
{
  long n;  // number of nodes in tree
  long t;  // time required to go back and forth in tree + any time ov previous tree
  long g; // gold collected in tree (time sensitive)
  Tree(){};
  Tree(int ni, long ti, long gi){
    n = ni; t = ti; g = gi;
  }
};


std::vector<long> gold;
std::vector<std::vector<std::pair<long, long>>> edges; // edges[u] =(v, l)

Tree dp(long u, long t0){
  // if no neighbors, return gold minus time taken to open u
  if (edges[u].empty()){
    return Tree(1, t0, gold[u]-t0);
  }
  

  // compute the trees of the neighbors
  std::vector<Tree> next_trees;
  for (std::pair<long, long> vl : edges[u]){
    Tree tree = dp(vl.first, t0 + vl.second);
    tree.t += vl.second; // add time to go back through node
    next_trees.push_back(tree);
  }
  // number of next trees
  int next_size = next_trees.size();

  // sort trees according to time*num_nodes. it is the order of opening subtrees
  std::sort(next_trees.begin(), next_trees.end(), [](Tree tree1, Tree tree2) {
    return tree1.t * tree2.n < tree2.t * tree1.n;   
  });

  // compute cumulative gold, time, nodes of current node
  long sum_n = 0;
  long sum_t = 0;
  long sum_g = 0;
  for (int i=0; i<next_size; i++){
    sum_g += next_trees[i].g - sum_t * next_trees[i].n;
    sum_n += next_trees[i].n;
    sum_t += next_trees[i].t - t0;
  }

  return Tree(sum_n+1, t0 + sum_t, sum_g+gold[u]-t0);

}

void testcase(){
  long n; std::cin >> n;

  gold.clear();
  gold.resize(n+1);
  for (int i=0; i<n; i++){
    std::cin >> gold[i+1];
  }

  edges.clear();
  edges.resize(n+1);
  for (int i=0; i<n; i++){
    long u, v, l;
    std::cin >> u >> v >> l;
    edges[u].push_back(std::make_pair(v, l)); 
  }


  std::cout << dp(0, 0).g << std::endl;

  return;
}


int main(){
  std::ios_base::sync_with_stdio(false);
  int T; std::cin >> T;
  while(T--) testcase();
  return 0;
}