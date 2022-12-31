#include <iostream>

void testcase(){
  int n; std::cin>>n;
  int sum = 0;
  for (int i=0; i<n; i++){
    int a; std::cin>>a;
    sum += a;
  }
  std::cout << sum << std::endl;
  return;
}


int main(){
  std::ios_base::sync_with_stdio(false);
  int T; std::cin>>T;
  while(T--) testcase();
}