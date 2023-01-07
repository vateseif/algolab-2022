///1
#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>

#define trace(x) std::cerr << #x << " = " << x << std::endl;

std::vector<long> V;

void testcase(){
  int n;
  long k;
  std::cin >> n >> k;

  // store values
  V.clear(); V.resize(n);
  for (int i=0; i<n; i++){
    std::cin >> V[i];
  }


  int l, r;
  int i = 0;
  long sum_v = 0;
  long min_e = std::numeric_limits<long>::max();
  for (int j=0; j<n; j++){
    sum_v += V[j];
    long e = std::abs(k - sum_v);
    if (e < min_e){
      min_e = e;
      l = i;
      r = j;
    }
    while (sum_v > k){
      sum_v -= V[i++];
      long e = std::abs(k - sum_v);
      if (e < min_e){
        min_e = e;
        l = i;
        r = j;
      }
    }
  }

  std::cout << l << " " << r << std::endl;

  return;
}


int main(){
  std::ios_base::sync_with_stdio(false);
  int T; std::cin>>T;
  while(T--) testcase();
  return 0;
}