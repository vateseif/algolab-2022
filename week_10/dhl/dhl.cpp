///3
#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>

#define trace(x) std::cerr << #x << " = " << x << std::endl;

int MAX_VAL = std::numeric_limits<int>::max();
std::vector<int> A, B, SA, SB;
std::vector<std::vector<int>> memo;


int dp(int na, int nb){
  if (na==0 && nb==0) return 0;
  if (memo[na][nb]<MAX_VAL) return memo[na][nb];
  
  int diff_a = - SA[na] + A[na];
  int diff_b = - SB[nb] + B[nb];
  
  int sa = A[na];
  if (na==1){
    int sb = SB[1] + diff_b;
    int c = sa*sb;
    memo[na][nb] = c;
    return c;
  }
  
  int sb = B[nb];
  if (nb==1){
    sa = SA[1] + diff_a;
    int c = sa*sb;
    memo[na][nb] = c;
    return c;
  }
  
  int min_cost = MAX_VAL;
  
  sa = A[na];
  for(int kb=nb-1; kb>=1; kb--){
    sb = SB[nb-kb+1] + diff_b;
    int c = sa*sb + dp(na-1, nb-kb);
    min_cost = std::min(min_cost, c);
  }
  
  sb = B[nb];
  for (int ka=na-1; ka>=1; ka--){
    sa = SA[na-ka+1] + diff_a;
    int c = sa*sb + dp(na-ka, nb-1);
    min_cost = std::min(min_cost, c); 
  }
  
  memo[na][nb] = min_cost;
  return min_cost;
}


void testcase(){
  int n;
  std::cin >> n;
  
  A.clear(); A.resize(n+1);
  SA.clear(); SA.resize(n+1);
  for(int i=1; i<n+1; i++){
    int ai; std::cin >> ai;
    A[i] = ai-1;
  }

  B.clear(); B.resize(n+1);
  SB.clear(); SB.resize(n+1);
  for(int i=1; i<n+1; i++){
    int bi; std::cin >> bi;
    B[i] = bi-1;
  }
  
  SA[n] = A[n];
  SB[n] = B[n];
  for (int i=n-1; i>0; i--){
    SA[i] = A[i] + SA[i+1];
    SB[i] = B[i] + SB[i+1];
  }
  
  
  memo.clear();
  memo.resize(n+1, std::vector<int>(n+1, MAX_VAL));
  std::cout << dp(n, n) << std::endl;
  

  return;
}


int main(){
  std::ios_base::sync_with_stdio(false);
  int T; std::cin>>T;
  while(T--) testcase();
  return 0;
}