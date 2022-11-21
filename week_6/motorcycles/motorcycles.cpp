///4
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <CGAL/Gmpq.h>

#define trace(x) std::cerr << #x << " = " << x << std::endl;

int n;
std::vector<bool> flag;
std::vector<CGAL::Gmpq> slopes;
std::vector<std::pair<int, long>>  b;

// sort using a custom function object
struct {
  bool operator()(std::pair<int, long> a, std::pair<int, long> b) const{   
    return a.second < b.second;
  }   
} customLess;

void solve(){
  std::sort(b.begin(), b.end(), customLess);
  
  flag.clear(); flag.resize(n, true);
  CGAL::Gmpq min_slope = slopes[b[0].first];
  for (int i=1; i<n; i++){
    int idx = b[i].first;
    if (CGAL::abs(slopes[idx]) < CGAL::abs(min_slope) || (CGAL::abs(slopes[idx]) == CGAL::abs(min_slope) && slopes[idx] > min_slope) ){
      min_slope = slopes[idx];
    } else if (slopes[idx] < min_slope){
      flag[idx] = false;
    }
  }
  
  min_slope = slopes[b[n-1].first];
  for (int i=n-2; i>=0; i--){
    int idx = b[i].first;
    if (CGAL::abs(slopes[idx]) <= CGAL::abs(min_slope)){
      min_slope = slopes[idx];
    }else if (slopes[idx] > min_slope){
      flag[idx] = false;
    }
  }
  
  for (int i=0; i<n; i++){
    if (flag[i]) std::cout << i << " ";
  }
  
  std::cout << std::endl;

}

void testcase(){
  std::cin >> n;

  b.clear(); b.resize(n);
  slopes.clear(); slopes.resize(n);
  for (int i=0; i<n; i++){
    long y0, x1, y1;
    std::cin >> y0 >> x1 >> y1;
    b[i] = std::make_pair(i, y0);
    slopes[i] = CGAL::Gmpq(y1 - y0, x1);
  }

  solve();
}

int main(){
  int T; std::cin >> T;
  while (T--) testcase();
  return 0;
}