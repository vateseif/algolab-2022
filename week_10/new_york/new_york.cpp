///1
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

int n, m, k;
std::vector<int> temp; // route and temp for each island
std::vector<std::vector> routes;


void solve2(){
  int src = 0;
  // BFS to find vertex set S
  std::vector<int> vis(n, false); // visited flags
  std::stack<int> Q; // BFS queue (from std:: not boost::)
  vis[src] = true; // Mark the source as visited
  Q.push(src);
  while (!Q.empty()) {
    const int u = Q.top();
    Q.pop();
    for (int v : routes[u]){
      if (vis[v]) continue;
      vis[v] = true;
      Q.push(v);
    }
  }
}

void solve(){
  std::vector<int> critical;
  for (int i=0; i<n; i++){
    int counter = 1;
    int v = routes[i];
    int min_h = temp[i];
    int max_h = temp[i];
    while (v!=-1){
      counter++;
      min_h = std::min(min_h, temp[v]);
      max_h = std::max(max_h, temp[v]);
      if (counter == m){
        if (max_h - min_h <= k){
          critical.push_back(i);
        }
        break;
      }
      v = routes[v];
    }
  }
  for (int i=0; i<n; i++) 


  if (critical.size()>0){
    for (int s : critical) std::cout  << s << " ";
  } else{
    std::cout  << "Abort mission";
  }
  std::cout  << std::endl;

}

void testcase(){
  std::cin >> n >> m >> k;
  temp.clear();
  temp.resize(n);
  for (int i=0; i<n; i++){
    int hi; std::cin >> hi;
    temp[i] = hi;
  }

  routes.clear();
  routes.resize(n);
  for (int i=0; i<n-1; i++){
    int u, v;
    std::cin >> u >> v;
    routes[u].push_back(v);
  }

  solve();

  return;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int T; std::cin >> T;
  while(T--) testcase();
  return 0;
}