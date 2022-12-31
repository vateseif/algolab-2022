#include<iostream>
#include<vector>

int num_even(std::vector<int> vec){
  int even = 0;
  int odd = 0;
  for (int i:vec){
    if (i%2==0){
      even++;
    }
    else{
      odd++;
    }
  }
  return even*(even-1)/2 + odd*(odd-1)/2 + even;
}

int main(){
  // init
  std::ios_base::sync_with_stdio(false);
  int T; std::cin >> T;
  
  for (int t=0; t<T; t++){
    int n; std::cin >> n;
    
    typedef std::vector<int> VI;
    typedef std::vector<VI> VVI;
    typedef std::vector<VVI> VVVI;
    VVI s(n, VI(n)), x(n , VI(n));
    VVVI d(n, VVI(n, VI(n)));
    // retrieve input data
    for (int i=0; i<n; i++){
      for (int j=0; j<n; j++){
        std::cin >> x[i][j];
        if (j == 0){
          s[i][j] = x[i][j];
        }
        else{
          s[i][j] = s[i][j-1] + x[i][j];
        }
      }
    }

    int result = 0;
    for (int j1=0; j1<n; j1++){
        for (int j2=j1; j2<n; j2++){
            // VI pS(n);
            int even=0, odd=0;
            for (int i=0; i<n; i++){
                int ps = s[i][j2] - s[i][j1] + x[i][j1];
                if (i == 0){
                    d[j1][j2][i] = ps;
                }else{
                    d[j1][j2][i] = d[j1][j2][i-1] + ps;
                }
                if (d[j1][j2][i] % 2 ==0){
                    even++;
                }else{
                    odd++;
                }
            }
            result += even*(even-1)/2 + odd*(odd-1)/2 + even;
        }
    }
    
    std::cout << result << std::endl;
  }
  
  return 0;
}




