///1
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <stack>

typedef std::vector<int> VI;
typedef std::vector<bool> VB;

int n, m;
int MIN_VAL = std::numeric_limits<int>::min();

VI S;
std::vector<VI> edges;
std::vector<std::pair<int, VB>> reachable; 


void compute_reachable(int u){

  std::stack<int> Q;
  std::vector<bool> vis(n, false);
  Q.push(u);
  vis[u] = true;
  int sum = 0;
  while (!Q.empty()){
    int v = Q.top(); Q.pop();
    sum += S[v];
    for (int next_v : edges[v]){
      if (vis[next_v]) continue;
      Q.push(next_v);
      vis[next_v] = true;
    }
  }

  reachable[u] = std::make_pair(sum, vis);
}



void testcase(){
  
  std::cin >> n >> m;

  S.clear();
  S.resize(n);
  for(int i=0; i<n; i++){
    std::cin>>S[i];
  }

  edges.clear();
  edges.resize(n);
  for(int i=0; i<m; i++){
    int u, v;
    edges[u].push_back(v);
  }

  reachable.clear();
  reachable.resize(n);
  for (int i=0; i<n; i++){
    compute_reachable(i);
  }

  int max_s = MIN_VAL;
  for (int i=0; i<n-1; i++){
    for (int j=i+1; j<n; j++){
      if (reachable[i].second[j]) continue;
      int s = 0;
      for (int k=0; k<n; k++){
        if (reachable[i].second[k] || reachable[j].second[k]){
          s += S[k];
        }
      }
      max_s = std::max(max_s, s);
    }
  }

  std::string out = max_s>0? std::to_string(max_s) : "impossible";

  std::cout << out << std::endl;

  
  return;
}




int main(){
  std::ios_base::sync_with_stdio(false);
  int T; std::cin >> T;
  while (T--) testcase();

  return 0;
}