///4
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

#define trace(x) std::cerr << #x << " = " << x << std::endl;

typedef std::pair<long, long> PL; // pair of longs
typedef std::vector<long> VL;
typedef std::vector<std::vector<long>> VVL;
typedef std::vector<std::vector<std::vector<long>>> VVVL;

int n, H;
std::vector<PL> A;
std::vector<int> B;
VVVL memo;

void init_dp(){
  memo.clear();
  memo.resize(n+1, VVL(n+1, VL(H+1, std::numeric_limits<long>::min())));

  for (int h=0; h<=H; h++){
    for (int i=1; i<=n; i++){
      PL ph = A[i-1];
      memo[i][1][h] = ph.second >= h? std::max(ph.first, memo[i-1][1][h]) : memo[i-1][1][h];  
    }
  }
  
  for (int j=2; j<=n; j++){
    for (int i=j; i<=n; i++){
      for (int h=0; h<=H; h++){
        PL ph = A[i-1];
        long h_prev = std::max((long)0, h - ph.second);
        memo[i][j][h] = std::max(memo[i-1][j][h], memo[i-1][j-1][h_prev] + ph.first);
      }
    }
  }
}



void testcase(){
  long m, a, b, P, W;
  
  std::cin >> n >> m >> a >> b >> P >> H >> W;

  A.clear(); A.resize(n);
  for (int i=0; i<n; i++){
    long p, h;
    std::cin >> p >> h;
    A[i] = std::make_pair(p, h);
  }

  B.clear(); B.resize(m);
  for (int i=0; i<m; i++) std::cin >> B[i]; 

  // init dp memo. memo[i][j][h] gives the max power from picking i out of the first j potions of type A with at least h power
  init_dp();


  /*
  Sort B and loop through it. For every sum(w_i) >= W, loop through j until dp[n][j][H] >= P + n_b*b && sum(w_i) >= W+n_a*a
  */
  std::sort(B.begin(), B.end(), std::greater<int>());
  long sw = 0;
  for (int nb=0; nb<=m; nb++){
    if (sw >= W){
      for (int j=1;j<=n;j++){
        if (sw < W + j*a) break;
        if (memo[n][j][H] >= P+nb*b){
          std::cout << j + nb << std::endl;
          return;
        }
      }
    }
    sw += B[nb];
  }

  std::cout << -1 << std::endl;

}


int main(){
  std::ios_base::sync_with_stdio(false);
  int T; std::cin >> T;
  while (T--) testcase();
  return 0;
}