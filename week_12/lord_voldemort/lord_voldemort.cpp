///3
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>


#define trace(x) std::cerr << #x << " = " << x << std::endl;

typedef std::pair<int, int> PI;

int n, m, k;
std::vector<int> V;
std::vector<std::pair<int, int>> intervals;

std::vector<int> memo_m;
std::unordered_map<int, int> memo;

/*
int dp(PI i_prev, int n_inter, int sum){
  if (n_inter==m) {
    memo_m[m-1] = std::max(sum, memo_m[m-1]);
    return sum;
  }
  if (memo[b_prev] > 0) {
    trace("memo called");
    return memo[b_prev];
  }
  // index of first element whose start is after previous finish
  auto start_idx = std::upper_bound(intervals.begin(), intervals.end(), i_prev.b, [](int b_p, PI i1) {
        return i1.first > b_p;   
    });
  // if no element left, return current sum
  if (start_idx==intervals.end()) return sum;
  // compute dp recursion for all feasible intervals (a_i+1 > b_i)
  int max_sum = sum;
  for (auto it = start_idx; it!=intervals.end(); it++){
    max_sum = std::max(max_sum, dp(*it, n_inter+1, 1+sum+it->second-it->first));
  }
  // return max number of elements achieved without overlaps
  memo[b_prev] = max_sum;
  return max_sum;
}
*/

int dp(PI i_prev, int n_inter, int sum){
  // number of horcruxes in i_prev
  int n_horcrux = i_prev.second - i_prev.first + 1;
  // if m intervals looked, stop
  if (n_inter == m) {
    memo_m[m-1] = std::max(sum, memo_m[m-1]);
    return sum;
  }
  // if already computed, return memo
  if (memo[i_prev.second]>0) return memo[i_prev.second];

  // index of first element whose start is after previous finish
  auto start_idx = std::upper_bound(intervals.begin(), intervals.end(), i_prev.b, [](int b_p, PI i1) {
        return i1.first > b_p;   
    });
  // if no feasible just return number of horcrux for i_prev
  if (start_idx == intervals.end()) return sum;

  int max_horcrux = std::numeric_limits<int>::min();
  for (auto it=start_idx; it!=intervals.end(); it++){
    int new_horcrux = dp(*it, n_inter+1, sum + n_horcrux);
    max_horcrux = std::max(max_horcrux, new_horcrux);
  }
  
  memo[i_prev.second] = max_horcrux;
  return max_horcrux;
}


void testcase(){
  std::cin >> n >> m >> k;

  V.clear();
  V.resize(n);
  for (int i=0; i<n; i++){
    std::cin >> V[i];
  }


  // run sliding window
  int back=0;
  int sum_v=0;
  intervals.clear(); // store intervals where sum_v == k
  for (int i=0; i<n; i++){
    if (sum_v<k){
      sum_v += V[i];
    }
    while (sum_v>=k){
      if (sum_v==k) intervals.push_back(std::make_pair(back, i));
      sum_v -= V[back++];
    }
  }

  memo.clear();
  memo_m.clear();
  memo_m.resize(m);
  // compute disjoint intervals with max number of horocrux
  dp(std::make_pair(-1,-1), 0, 0);


  std::string out = memo_m[m-1]==0? "fail" : std::to_string(memo_m[m-1]); 
  std::cout << out << std::endl;
  


  return;
}



int main(){
  std::ios_base::sync_with_stdio(false);
  int T; std::cin >> T;
  T=1;
  while(T--) testcase();
  return 0;
}