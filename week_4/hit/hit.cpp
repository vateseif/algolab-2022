///1
#include <iostream>
#include <string>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

#define trace(x) std::cerr << #x << " = " << x << std::endl;

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2    P;
typedef K::Ray_2      R;
typedef K::Segment_2  S;


bool testcase(){
  int n; std::cin>>n;
  if (n==0) return false;

  long x, y, a, b;
  std::cin >> x >> y >> a >> b;
  R ray(P(x, y), P(a, b));

  bool intersects = false;
  for (int i=0;i<n;i++){
    long r, s, t, u;
    std::cin >> r >> s >> t >> u;
    if (!intersects){
      S seg(P(r, s), P(t, u));
      if (CGAL::do_intersect(ray, seg)) intersects = true;
    }
  }
  std::string out = intersects? "yes" : "no"; 
  std::cout << out << std::endl;
  return true;
}


int main(){
  std::ios_base::sync_with_stdio(false);
  while (testcase());
  return 0;
}