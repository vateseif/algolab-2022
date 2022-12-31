#include<iostream>
#include<vector>
#include<algorithm>

int main(){
  // init
  std::ios_base::sync_with_stdio(false); 
  int t; std::cin >> t;
  
  for (int i=0; i<t; i++){
    int n; std::cin >> n;
    std::vector<int> v;
    int max_idx = 0;
    for (int j=0; j<n; j++){
      int h; std::cin >> h;
      if (j+h-1 > max_idx){
        v.push_back(j+h-1);
        max_idx = j+h-1;
      }else{
        v.push_back(max_idx);
      }
    }
    
    int l, k = 0;
    while(true){
      l = v.at(k);
      if (l <= k){
        std::cout << l+1 << std::endl;
        break;
      }
      if (l > n){
        std::cout << n << std::endl;
        break;
      }
      k = l;
    }
    
  }
  return 0;
}