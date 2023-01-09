#include <iostream>
#include <vector>
#include <algorithm>

std::vector<int> A, B;

void testcase(){
  int n;

  A.clear(); A.resize(n);
  for(int i=0; i<n; i++){
    std::cin>>A[i];
  }

  B.clear(); B.resize(n);
  for(int i=0; i<n; i++){
    std::cin>>B[i];
  }

  return;
}


int main(){
  std::ios_base::sync_with_stdio(false);
  int T; std::cin>>T;
  while(T--) testcase();
  return 0;
}