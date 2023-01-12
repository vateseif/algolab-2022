///1
#include <iostream>
#include <vector>
#include <cmath>
#include <deque>
#include <algorithm>
#include <unordered_map>

#define trace(x) std::cerr << #x << " = " << x << std::endl;


int MIN_VAL = std::numeric_limits<int>::min();

int n, k, m;
std::vector<int> types;
std::vector<std::vector<std::vector<std::vector<int>>>> memo;

std::vector<int> excitement;
std::vector<std::vector<int>> update_rule; // (Q_list, type) -> Q_list


void init(){
  update_rule.clear();
  update_rule.resize(7, std::vector<int>(2, 0));
  update_rule[0][0] = 1;
  update_rule[1][0] = 3;
  update_rule[2][0] = 5;
  update_rule[3][0] = 3;
  update_rule[4][0] = 5;
  update_rule[5][0] = 3;
  update_rule[6][0] = 5;

  update_rule[0][1] = 2;
  update_rule[1][1] = 4;
  update_rule[2][1] = 6;
  update_rule[3][1] = 4;
  update_rule[4][1] = 6;
  update_rule[5][1] = 4;
  update_rule[6][1] = 6;

  excitement.clear();
  excitement.resize(7);
  excitement[0] = 0; 
  excitement[1] = 1000;
  excitement[2] = 1000;
  excitement[3] = 1000;
  excitement[4] = 2000;
  excitement[5] = 2000;
  excitement[6] = 1000;
}


int dp(int u, int qp_diff, int Q, int P){
  if (u == n) return 0;
  if (memo[u][qp_diff][Q][P] > MIN_VAL)
    return memo[u][qp_diff][Q][P];

  int max_c = MIN_VAL;
  //1
  int Q1 = update_rule[Q][types[u]];
  int c1 = -std::pow(2, std::abs(qp_diff+1-10)) + excitement[Q1];
  if (c1 >= 0) 
    max_c = std::max(max_c, c1+dp(u+1, qp_diff+1, Q1, P));
  //2
  int P1 = update_rule[P][types[u]];
  int c2 = -std::pow(2, std::abs(qp_diff-1-10)) + excitement[P1];
  if (c2>=0) 
    max_c = std::max(max_c, c2+dp(u+1, qp_diff-1, Q, P1));

  memo[u][qp_diff][Q][P] = max_c;
  return max_c;
}


void testcase(){
  std::cin >> n >> k >> m;

  types.clear();
  types.resize(n);
  for(int i=0; i<n; i++){
    int xi; std::cin >> xi;
    types[i] = xi;
  }

  memo.clear();
  memo.resize(n, std::vector<std::vector<std::vector<int>>>(21,
   std::vector<std::vector<int>>(7, std::vector<int>(7, MIN_VAL))));
  std::cout << dp(0, 10, 0, 0) << std::endl;

  return;
}


int main(){
  std::ios_base::sync_with_stdio(false);  
  int T; std::cin >> T;
  init();

  while(T--) testcase();
  return 0;
}