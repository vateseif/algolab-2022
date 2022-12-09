///1
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

#define trace(x) std::cerr << #x << " = " << x << std::endl;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef K::Point_2 P;
typedef Triangulation::Face_handle FaceHandle;
typedef std::unordered_map<FaceHandle, long> MaxDistanceMap;

Triangulation t;
std::vector<P> infected;
MaxDistanceMap distance, opened;


bool find_exit(FaceHandle& face, const long& d){
  opened[face] = 1;
  if (t.is_infinite(face)) return true;
  if (distance[face] >= d) return true;
  for (int i=0;i<3;i++){
    FaceHandle neighbor_face = face->neighbor(i);
    if (opened[neighbor_face]) continue;
    P a = face->vertex(face->ccw(i))->point();
    P b = face->vertex(face->cw(i))->point();
    long ab_distance = CGAL::squared_distance(a, b);

    if (ab_distance >= 4*d && find_exit(neighbor_face, d)){
      distance[face] = d;
      return true;
    }
  }
  return false;
}


bool solve(long& x, long& y, const long& d){
  P point(x, y);
  Triangulation::Vertex_handle nearest = t.nearest_vertex(point);
  double d_nearest = CGAL::squared_distance(point, nearest->point());
  if (d_nearest < d) return false;
  
  FaceHandle face = t.locate(point);
  opened.clear();
  
  return find_exit(face, d);

}

bool testcase(){
  int n, m;
  std::cin >> n;
  if (!n) return false;

  infected.clear(); infected.resize(n);
  for (int i=0;i<n;i++){
    long x, y;
    std::cin >> x >> y;
    infected[i] = P(x, y);
  }

  t.clear();
  t.insert(infected.begin(), infected.end());
  distance.clear();
  std::cin >> m;
  for (int i=0;i<m;i++){
    long x, y, d;
    std::cin >> x >> y >> d;
    std::string out = solve(x, y, d)? "y":"n";
    std::cout << out;
  }
  
  std::cout << std::endl;
  return true;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  
  while (testcase());
  return 0;
}