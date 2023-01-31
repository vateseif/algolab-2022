///4
#include <iostream>
#include <vector>
#include <limits>
#include <cmath>
#include <algorithm>
#include <unordered_map>

#define trace(x) std::cerr << #x << " = " << x << std::endl;

struct SetIsland{
  int w; // waterway
  int n; // number of consecutive islands apart from 0
  long m; // sum of men includin men[0]
  SetIsland(){};
  SetIsland(int wi, int ni, long mi){
    w=wi, n=ni, m=mi;
  }
};


std::vector<long> men;
std::vector<std::vector<int>> waterways;
std::vector<SetIsland> memo; // memo[i] = vector of pair (sum_men, waterway) for i islands


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

  
  int max_islands = men[0]==k? 1:0;
  memo.clear();
  for (int i=0; i<w; i++){
    int l = 0;
    long sum_men = 0;
    for (int j=0; j<(int)waterways[i].size(); j++){
      int island = waterways[i][j];
      sum_men += men[island];
      while (sum_men>k) {sum_men-=men[waterways[i][l++]];}
      if (sum_men==k) max_islands = std::max(j-l+1, max_islands);
      if (l==0) memo.push_back(SetIsland(i, j+1, sum_men));
    }
  }

  std::sort(memo.begin(), memo.end(), [](SetIsland a, SetIsland b) {
        return a.m < b.m;   
  });
  
  std::vector<std::vector<SetIsland>> is(1, {memo[0]});
  for (int i=1; i<(int)memo.size()-1; i++){
    if (memo[i-1].m==memo[i].m){
      is.back().push_back(memo[i]);
    } else{
      is.push_back({memo[i]});
    } 
  }
  
  int l=0;
  int r=is.size()-1;
  long goal = k+men[0];
  while (l<=r){
    if (is[l][0].m + is[r][0].m < goal){
      l++;
    }else if(is[l][0].m + is[r][0].m > goal){
      r--;
    }else{
      for (SetIsland sil : is[l]){
        for (SetIsland sir : is[r]){
          if (sil.w != sir.w){
            max_islands = std::max(max_islands, sil.n + sir.n - 1);
          }
        }
      }
      l++; r--;
    }
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