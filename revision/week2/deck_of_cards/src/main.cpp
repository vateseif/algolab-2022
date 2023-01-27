///3
#include <iostream>
#include <vector>
#include <limits>
#include <cmath>

std::vector<int> val;

void testcase(){
  int n, k;
  std::cin >> n >> k;

  val.clear(); val.resize(n);
  for (int i=0; i<n; i++){
    std::cin >> val[i]; 
  }

  int i=0; int j=0;
  int back = 0;
  int sum = 0; // keep count of sum over range back - i  
  int min_e = std::numeric_limits<int>::max(); // keep track  of smallest error
  for (int l=0; l<n; l++){
    // add new val
    sum+=val[l];
    if (std::abs(k-sum) < min_e) {
      min_e = std::abs(k-sum);
      i = back;
      j = l;
    }
    // adding values will create a bigger error only if sum >= k
    while (sum > k){
      sum -= val[back++];
      if (std::abs(k-sum) < min_e) {
        min_e = std::abs(k-sum);
        i = back;
        j = l;
      }
    }
  }

  std::cout << i << " " << j << std::endl;

  return;
}


int main(){
  std::ios_base::sync_with_stdio(false);
  int T; std::cin >> T;
  while (T--) testcase();
  return 0;
}