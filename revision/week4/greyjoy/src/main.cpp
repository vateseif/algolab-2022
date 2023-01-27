///1
#include <iostream>
#include <vector>
#include <limits>
#include <cmath>
#include <algorithm>
#include <unordered_map>

#define trace(x) std::cerr << #x << " = " << x << std::endl;

struct MemoStruct{
  int n_islands;
  int island_id; // (0, island, -1)
  MemoStruct(){};
  MemoStruct(int ni, long idi){
    n_islands = ni; island_id = idi;
  }
};


std::vector<int> men;
std::vector<std::vector<int>> waterways;
std::unordered_map<long, MemoStruct> memo;  // each num of soldiers store max number of islands and which (0, island, -1)



void testcase(){
  int n, w;
  long k;
  std::cin >> n >> k >> w;

  men.clear(); men.resize(n);
  for (int i=0; i<n; i++){
    std::cin >> men[i];
  }

  waterways.clear(); waterways.resize(w);
  for (int i=0; i<w; i++){
    int l; std::cin >> l;
    for (int j=0; j<l; j++){
      int island; std::cin >> island;
      waterways[i].push_back(island);
    }
  }

  int max_islands = 0;
  // precomputation: find highest sequence on each waterway
  // plus compute sum of men
  memo.clear();
  for (int i=0; i<w; i++){
    int back = 0;
    long sum_men = men[0];
    if (sum_men == k) max_islands = std::max(max_islands, 1);
    for (int j=1; j<waterways[i].size(); j++){
      int island = waterways[i][j];
      // add men of jth island to current window
      sum_men += men[island];
      // while sum of window greater than k remove islands
      while (sum_men > k) sum_men -= men[waterways[i][back++]];
      // if sum of window = k, check if there are more than max_islands
      if (sum_men == k) max_islands = std::max(max_islands, j-back+1);
      // compute sum up to island j of waterway i
      if (back==0){
        if (memo[sum_men].n_islands == j){
          memo[sum_men] = MemoStruct(j, -1);
        }else if(j > memo[sum_men].n_islands){
          memo[sum_men] = MemoStruct(j, i);
        }
      }
    }
  }

  int i = men[0];
  while (i < k-i+men[0]){
    if (memo[i].island_id==-1 || memo[k-i+men[0]].island_id==-1 || memo[i].island_id!=memo[k-i+men[0]].island_id){
      max_islands = std::max(max_islands, memo[i].n_islands+memo[k-i+men[0]].n_islands-1);
    }
    i++;
  }

  std::cout << max_islands << std::endl;

  return;
}


int main(){
  std::ios_base::sync_with_stdio(false);
  int T; std::cin >> T;
  while (T--)testcase();
  return 0;
}