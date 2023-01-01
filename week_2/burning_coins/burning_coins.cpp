#include <iostream>
#include <vector>
#include <algorithm>

std::vector<int> values;
// forward declaration of dp_max funztion
int dp_max(int s, int e);

int dp_min(int s, int e){
  if (s==e) return 0;
  int val = std::min(dp_max(s+1, e), dp_max(s, e-1));
  return val;
}

int dp_max(int s, int e){
  if (s==e) return values[s];
  int val = std::max(values[s]+dp_min(s+1, e), values[e]+dp_min(s, e-1));
  return val;
}

void testcase(){
  int n;
  std::cin>>n;

  values.clear(); values.resize(n);
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