#include <iostream>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef K::Point_2 P;

std::vector<P> existing;


bool testcase(){
  int n;
  std::cin >> n;
  if (!n) return false;

  existing.clear(); existing.resize(n);
  for (int i=0; i<n; i++){
    int x, y;
    std::cin >> x >> y;
    existing[i] = P(x, y);
  }
  Triangulation t;
  t.insert(existing.begin(), existing.end());


  int m;
  std::cin >> m;

  for (int i=0; i<m; i++){
    int x, y;
    std::cin >> x >> y;
    P p(x,y);
    Triangulation::Vertex_handle nearest = t.nearest_vertex(p);
    double d = CGAL::squared_distance(nearest->point(), p);
    std::cout << d << std::endl;
  }

  return true;
}



int main(){
  std::ios_base::sync_with_stdio(false);
  std::cout << std::setprecision(17);
  while(testcase());
  return 0;
}