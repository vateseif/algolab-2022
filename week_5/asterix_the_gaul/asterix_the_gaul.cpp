#include <iostream>
#include <vector>
#include <algorithm>
#include <limits> 

#define trace(x) std::cerr << #x << " = " << x << std::endl;

std::vector<long> potions;
std::vector<long> memo_distance; // stores max distance reached for each # of movements used
std::vector<std::pair<int, int>> M;

int min_gulps(int& n, int& m, long& D, long& T){
  memo_distance.clear(); 
  memo_distance.resize(n, std::numeric_limits<long>::min());
  for (int s = 0; s < 1<<n; ++s) { // Iterate through all subsets
    long sum_t = 0;
    long sum_d = 0;
    int n_movements = 0;
    for (int i = 0; i < n; ++i) {
      // If i-th element in subset
      if (s & 1<<i) {
        sum_d += M[i].first;
        sum_t += M[i].second;
        n_movements++;
      }
      if (sum_d>=D && sum_t<T) return 0;
    }
    if (sum_t<T) memo_distance[n_movements] = std::max(memo_distance[n_movements], sum_d);
  }

  for (int i=0; i<m; i++){
    long si = potions[i];
    for (int n_mov=0; n_mov<n; n_mov++){
      if (memo_distance[n_mov] + n_mov*si >= D) return i+1;
    }
  }

  return -1;
}


void testcase(){
  int n, m;
  long D, T;
  std::cin >> n >> m >> D >> T;

  M.clear(); M.resize(n);
  for (int i=0; i<n; i++){
    long d, t;
    std::cin >> d >> t;
    M[i] = std::make_pair(d, t);
  }

  potions.clear();
  potions.resize(m); 
  for (int i=0; i<m; i++){
    long s;
    std::cin >> s;
    potions[i] = s;
  }

  int gulps = min_gulps(n, m, D, T);
  if (gulps == -1){
    std::cout << "Panoramix captured" << std::endl;
  }else{
    std::cout << gulps << std::endl;
  }
  

  return;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int T; std::cin >> T;
  while(T--)testcase();
  return 0;
}