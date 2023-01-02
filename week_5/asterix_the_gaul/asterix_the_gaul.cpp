#include <iostream>
#include <vector>
#include <algorithm>

#define trace(x) std::cerr << #x << " = " << x << std::endl;


std::vector<std::pair<int, int>> M;

bool isCaptured(int& n, long& D, long& T){
  for (int s = 0; s < 1<<n; ++s) { // Iterate through all subsets
    long sum_t = 0;
    long sum_d = 0;
    for (int i = 0; i < n; ++i) {
      // If i-th element in subset
      if (s & 1<<i) {
        sum_d += M[i].first;
        sum_t += M[i].second;
      }
      if (sum_d>=D && sum_t<T) return false;
    }
  }
  return true;
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

  for (int i=0; i<m; i++){
    long s;
    std::cin >> s;
  }

  if (isCaptured(n, D, T)){
    std::cout << "Panoramix captured" << std::endl;
  }else{
    std::cout << 0 << std::endl;
  }
  

  return;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int T; std::cin >> T;
  while(T--)testcase();
  return 0;
}