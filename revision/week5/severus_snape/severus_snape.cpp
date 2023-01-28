///3
#include <iostream>
#include <vector>
#include <limits>
#include <cmath>
#include <algorithm>

#define trace(x) std::cerr << #x << " = " << x << std::endl;

int n, m;
long a, b, P, H, W;
long MIN_VAL = std::numeric_limits<long>::min();
std::vector<long> B;
std::vector<std::pair<long, long>> A;
std::vector<std::vector<std::vector<long>>> memo;

void init_memo(){
  memo.clear();
  memo.resize(n+1, std::vector<std::vector<long>>(n+1, std::vector<long>(H+1, MIN_VAL)));

  for (int j=1; j<=n; j++){
    std::pair<long, long> ph = A[j];
    for (int h=0; h<=H; h++){
      memo[1][j][h] = ph.second>=h? std::max(memo[1][j-1][h], ph.first) : memo[1][j-1][h];
    }
  }

  for (int j=2; j<=n; j++){
    for (int i=2; i<=j; i++){
      for (int h=0; h<=H; h++){
        std::pair<long, long> ph = A[j];
        int h_prev = std::max(0, h - (int)ph.second);
        memo[i][j][h] = std::max(memo[i][j-1][h], memo[i-1][j-1][h_prev] + ph.first);
      }
    }
  }
}


void testcase(){
  
  std::cin >> n >> m >> a >> b >> P >> H >> W;

  A.clear(); A.resize(n+1);
  for (int i=1; i<=n; i++){
    long pi, hi;
    std::cin >> pi >> hi;
    A[i] = std::make_pair(pi, hi);
  }

  B.clear(); B.resize(m);
  for (int i=0; i<m; i++) std::cin >> B[i];

  init_memo();
  // sort B by w increasingly
  std::sort(B.begin(), B.end(), [](long w1, long w2) {
        return w1 > w2;   
  });

  
  long sum_w = 0;
  for (long nb=0; nb<=m; nb++){
    if (sum_w>=W){
      int na_max = a==0? n : std::min((int)std::floor((sum_w-W)/a), n);
      //int na_max = std::min((int)std::floor((W-sum_w)/a), n);
      for (int na=1; na<=na_max; na++){
        if (sum_w<W+na*a) break;
        if (memo[na][n][H] >= P+nb*b){
          std::cout << na+nb << std::endl;
          return;
        }
      }
    }
    sum_w += B[nb];
  }

  std::cout << -1 << std::endl;
  return;


}



int main(){
  std::ios_base::sync_with_stdio(false);
  int T; std::cin >> T;
  while (T--) testcase();
  return 0;
}