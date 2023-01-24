///2
#include<iostream>
#include<vector>
#include<algorithm>

#define trace(x) std::cerr << #x << " = " << x << std::endl;

int n;
std::vector<std::vector<int>> edges, memo; // memo[0][u] shortest path u->n-1, memo[1][u] longest_path u->n-1

// forward init
int dp_max(int u);
int dp_min(int u);

int dp_min(int u){
  if (u==n) return 0;
  if (memo[0][u] != -1) return memo[0][u];
  int min_d = std::numeric_limits<int>::max();
  for (int v : edges[u]){
    min_d = std::min(min_d, 1+dp_max(v));
  }
  memo[0][u] = min_d;
  return min_d;
}

int dp_max(int u){
  if (u==n) return 0;
  if (memo[1][u] != -1) return memo[1][u];
  int max_d = std::numeric_limits<int>::min();
  for (int v : edges[u]){
    max_d = std::max(max_d, 1+dp_min(v));
  }
  memo[1][u] = max_d;
  return max_d;
}



void testcase(){
  int m, r, b;
  std::cin >> n >> m >> r >> b;

  
  // store edges
  edges.clear();
  edges.resize(n+1);
  for (int i=0; i<m; i++){
    int u, v;
    std::cin >> u >> v;
    edges[u].push_back(v);
  }

  
  memo.clear();
  memo.resize(2, std::vector<int>(n+1, -1));
  // shortest min-max path from red
  int cost_r = dp_min(r);

  // shortest min_max path from black
  int cost_b = dp_min(b);

  bool sherlock = cost_r<cost_b || (cost_r==cost_b && cost_r%2==1);
  int out = sherlock? 0 : 1;

  std::cout << out << std::endl;
  
}



int main(){
  std::ios_base::sync_with_stdio(false);
  int T; std::cin >> T;
  while (T--) testcase();
  return 0;
}
