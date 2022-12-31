#include<iostream>
#include<vector>

int main(){
  // init
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  
  for (int l=0; l<t; l++){
    int n; std::cin >> n;
    std::vector<int> x, s;
      
    for (int i=0; i<n; i++){
      int a; std::cin >> a;
      x.push_back(a);
    }
    
    s.push_back(x.at(0));
    for (int i=1; i<n; i++){
      s.push_back(x[i] + s[i-1]);
    }
      
    int even = 0; int odd = 0;
    for (int i=0; i<n; i++){
      if(s[i] % 2 == 0){
        even++;
      }else{
        odd++;
      }
    }
    // even and odd pairs of S
    int ep = even*(even-1)/2;
    int op = odd*(odd-1)/2;
    
    int result = ep + op + even;
    
    std::cout << result << std::endl;
  }
  return 0;
}
