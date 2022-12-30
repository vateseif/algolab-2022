#include<iostream>
#include<vector>
#include<algorithm>

// TODO: solution gets 100%. can be faster making memo the same for black and red

typedef std::vector<std::vector<int>> AL; // adjacency list


int compute_bpath(AL &graph, std::vector<std::vector<int>> &memo, int node, int cost){
  int agent = cost % 2; // 1 = sherlock (max black), 0 = moriarty (min black)
  if (memo[agent][node] > -1) return memo[agent][node];
  
  // init cost of next nodes
  std::vector<int> next_nodes_cost;
  for (int next_node : graph[node]){
    next_nodes_cost.push_back(compute_bpath(graph, memo, next_node, cost+1));
  }
  int node_cost;
  if (agent == 0){
    node_cost = *std::min_element(next_nodes_cost.begin(), next_nodes_cost.end()) + 1;
  }else{
    node_cost = *std::max_element(next_nodes_cost.begin(), next_nodes_cost.end()) + 1;
  }
  memo[agent][node] = node_cost;
  return node_cost;
}


int compute_rpath(AL &graph, std::vector<std::vector<int>> &memo, int node, int cost){
  int agent = cost % 2; // 0 = sherlock (min red), 1 = moriarty (max red)
  if (memo[agent][node] > -1) return memo[agent][node];
  
  // init cost of next nodes
  std::vector<int> next_nodes_cost;
  for (int next_node : graph[node]){
    next_nodes_cost.push_back(compute_rpath(graph, memo, next_node, cost+1));
  }
  int node_cost;
  if (agent == 0){
    node_cost = *std::min_element(next_nodes_cost.begin(), next_nodes_cost.end()) + 1;
  }else{
    node_cost = *std::max_element(next_nodes_cost.begin(), next_nodes_cost.end()) + 1;
  }
  memo[agent][node] = node_cost;
  return node_cost;
}



int main(){
  
  // init
  std::ios_base::sync_with_stdio(false);
  int T; std::cin >> T;
  
  for (int t=0; t<T; t++){
    int n; std::cin >> n; // # of nodes
    int m; std::cin >> m; // # of transitions
    int r; std::cin >> r; // position red
    int b; std::cin >> b; // postion black
    
    AL graph(n); // adjacency list
    
    for (int i=0; i<m; i++){
      int u; std::cin >> u;
      int v; std::cin >> v;
      graph[u].push_back(v);
    }
    
    // memory containig distance to goal
    std::vector<std::vector<int>> rmemo(2, std::vector<int>(n+1, -1)); 
    rmemo[0][n] = 0;
    rmemo[1][n] = 0;
    // compute path from red
    int r_path_cost = compute_rpath(graph, rmemo, r, 0);
    
    // memory containig distance to goal
    std::vector<std::vector<int>> bmemo(2, std::vector<int>(n+1, -1)); 
    bmemo[0][n] = 0;
    bmemo[1][n] = 0;
    // compute path from black
    int b_path_cost = compute_bpath(graph, bmemo, b, 0);
    
    if (r_path_cost % 2 == 0){
      if (r_path_cost < b_path_cost){
        std::cout << 0 << std::endl;
      }
      else{
        std::cout << 1 << std::endl;
      }
    }
    else{
      if (r_path_cost <= b_path_cost){
        std::cout << 0 << std::endl;
      }
      else{
        std::cout << 1 << std::endl;
      }
    }
    
    
      
  }
  
  return 0;
}


