///2
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <limits>
#include <algorithm>

#define trace(x) std::cerr << #x << " = " << x << std::endl;

struct SubSet {
long sum_t, sum_d;
// This constructor is convenient for a concise initialisation. It can also
// be omitted and the member variables be set manually.
SubSet(long t, long d) : sum_t(t), sum_d(d) {}
};

int n, m;
long D, T;
std::vector<long> extra;
std::vector<std::pair<long, long>> M;


bool list(int start, int stop, std::vector<SubSet>& S){
  for (int s = 0; s < 1<<stop; ++s) { // Iterate through all subsets
    long sum_d = 0;
    long sum_t = 0;
    int counter = 0; // count number of movements selected
    for (int i = 0; i < stop; ++i) {
      if (s & 1<<i) {
        counter++;
        sum_d += M[start+i].first;
        sum_t += M[start+i].second;
        if (sum_d >= D && sum_t < T) return true;
      }
    }
    if (sum_t < T && counter) S.push_back(SubSet(sum_t, sum_d));
  }
  return false;
}

bool solve(){
  // split original list
  int n1 = n/2;
  int n2 = n - n1;
  // init subsets
  std::vector<SubSet> S1, S2;

  // compute sums. if already found solution return
  if (list(0, n1, S1)) return true;
  if (list(n1, n2, S2)) return true;

  // sort 2nd list
  std::sort(S2.begin(), S2.end(), [](SubSet a, SubSet b) {
    if (a.sum_t == b.sum_t) return a.sum_d < b.sum_d;
    return a.sum_t < b.sum_t;   
  });

  int s2_size = S2.size();
  for (int i=1; i<s2_size; i++){
    S2[i].sum_d = std::max(S2[i].sum_d, S2[i-1].sum_d);
  }

  // for each (sum_t, sum_d, n)_i in S1 do binary search to find
  // latest (sum_t, sum_d, n)_j in S2 such that sum_t(i)+sum_t(j)<T
  //long s_missing = std::numeric_limits<long>::max();
  for (SubSet s : S1){
    auto it = std::lower_bound(S2.begin(), S2.end(), T-s.sum_t, [](SubSet s, long t) {
      return s.sum_t < t;   });
    if (it!=S2.begin() && s.sum_d+(--it)->sum_d>=D) return true;  
  }

  return false;
}

void testcase(){
  
  std::cin>> n >> m >> D >> T;

  // store movement set M
  M.clear();
  M.resize(n);
  for (int i=0; i<n; i++){
    long di, ti;
    std::cin >> di >> ti;
    M[i] = std::make_pair(di, ti);
  }

  // store extra steps
  extra.clear(); extra.resize(m);
  for (int i=0; i<m; i++){
    long si;
    std::cin >> si;
    extra[i] = si;
  }

  if (solve()) {
    std::cout << 0 << std::endl;
    return;
  }else if(m==0) {
    std::cout << "Panoramix captured" << std::endl;
    return;
  }

  int l=0;
  int r=m;
  while (l<r){
    int mid = (l+r)/2;
    long s = extra[mid];
    for (int i=0; i<n; i++){
      M[i].first += s;
    }
    if (solve()){
      r = mid;
    }else{
      l = mid+1;
    }
    for (int i=0; i<n; i++){
      M[i].first -= s;
    }
  }
  
  if (l==m){
    std::cout << "Panoramix captured";
  }else{
    std::cout << l+1;
  }
  std::cout << std::endl;
  
  return;
}



int main(){
  std::ios_base::sync_with_stdio(false);
  int t; std::cin>>t;
  while (t--) testcase();
  return 0;
}