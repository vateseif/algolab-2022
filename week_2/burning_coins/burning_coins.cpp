#include <iostream>
#include <vector>
#include <algorithm>

#define trace(x) std::cerr << #x << " = " << x;

std::vector<int> values;
std::vector<std::vector<int>> memo;
// forward declaration of dp_max funztion
int dp_max(int s, int e);

int dp_min(int s, int e){
  if (s==e) return 0;
  int val = std::min(dp_max(s+1, e), dp_max(s, e-1));
  return val;
}

int dp_max(int s, int e){
  if (s==e) return values[s];
  if (memo[s][e] != -1) return memo[s][e];
  int val = std::max(values[s]+dp_min(s+1, e), values[e]+dp_min(s, e-1));
  memo[s][e] = val;
  return val;
}

void testcase(){
  int n;
  std::cin>>n;

  values.clear(); values.resize(n);
  memo.clear(); memo.resize(n, std::vector<int>(n, -1));
  for (int i=0;i<n; i++){
    int vi; std::cin>>vi;
    values[i] = vi;
  }

  std::cout << dp_max(0, n-1) << std::endl;
  return; 
}


int main(){
  std::ios_base::sync_with_stdio(false);
  int T; std::cin>>T;
  while (T--)testcase();
  return 0;
}