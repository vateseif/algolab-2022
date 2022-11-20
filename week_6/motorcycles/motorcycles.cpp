///1
#include <iostream>
#include <vector>
#include <algorithm>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

#define trace(x) std::cerr << #x << " = " << x << std::endl;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2    P;
typedef K::Ray_2      R;

int n;
std::vector<int> forever;
std::vector<R> rays;
std::vector<std::pair<int, long>>  b;
std::vector<std::pair<long, long>> p;


// sort using a custom function object
struct {
  bool operator()(std::pair<int, long> a, std::pair<int, long> b) const{   
    return a.second < b.second;
  }   
} customLess;

void solve(){
  std::sort(b.begin(), b.end(), customLess);

  rays.clear(); rays.resize(n);
  for (int i=0; i<n; i++){
    int j = b[i].first;
    rays[i] = R(P(0, b[i].second), P(p[j].first, p[j].second));
  }

  int i = 0;
  forever.clear(); forever.push_back(b[n-1].first);
  while (i < n-1){
    bool isForever = true;
    R ri = rays[i];
    int j = i+1;
    while (j<n){
      if (CGAL::do_intersect(ri, rays[j])) {isForever = false; break;}
      j++;
    }
    if (isForever){
      forever.push_back(b[i].first);
      i++;
    }else {
      i = j;
    }
  }
  
  std::sort(forever.begin(), forever.end());
  for (int f : forever){
    std::cout << f << " ";
  }
  std::cout << std::endl;

}

void testcase(){
  std::cin >> n;

  b.clear(); b.resize(n);
  p.clear(); p.resize(n);
  for (int i=0; i<n; i++){
    long y0, x1, y1;
    std::cin >> y0 >> x1 >> y1;
    b[i] = std::make_pair(i, y0);
    p[i] = std::make_pair(x1, y1);
  }

  solve();
}

int main(){
  int T; std::cin >> T;
  while (T--) testcase();
  return 0;
}