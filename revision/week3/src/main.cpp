///1
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#define trace(x) std::cerr << #x << " = " << x<< std::endl; 

int k;
std::vector<std::vector<long>> memo; // memo[u][k] max scores in k moves starting from u
std::vector<std::vector<std::pair<int, long>>> edges;


long dp(int u, int left){
  if (left==0) return 0;
  if (memo[u][left] != -1) return memo[u][left];
  long max_score = 0;
  if (!edges[u].empty()){
    for (std::pair<int, long> vs : edges[u]){
      long score = vs.second + dp(vs.first, left-1);
      max_score = std::max(max_score, score);
    }
  }else{
    max_score = dp(0, left);
  }
  // store max_score in memo
  memo[u][left] = max_score;
  return max_score;
}


void testcase(){
  int n, m;
  long x;
  std::cin >> n >> m >> x >> k;

  // stores edges 
  edges.clear(); edges.resize(n);
  for (int i=0; i<m; i++){
    int u, v;
    long p;
    std::cin >> u >> v >> p;
    edges[u].push_back(std::make_pair(v, p));
  }

  // start at node 0 with 1...k moves
  memo.clear();
  memo.resize(n, std::vector<long>(k+1, -1));
  for (int i=k; i>0; i--) dp(0, i);

  // index of lower bound represents number of moves needed to do at least x
  auto min_k = std::lower_bound(memo[0].begin(), memo[0].end(), x, [](long a, long min_x) {
      return a < min_x;   
    });
  // if no value>=x then solution is impossible
  std::string out = min_k==memo[0].end()? "Impossible" : std::to_string(std::distance(memo[0].begin(), min_k));
  
  std::cout << out << std::endl;

  return;
}


int main(){
  std::ios_base::sync_with_stdio(false);
  int T; std:: cin >> T;
  while (T--) testcase();
  return 0;
}