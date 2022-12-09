///1
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

#define trace(x) std::cerr << #x << " = " << x << std::endl;

struct FaceDataStructure{
  long max_d;
  std::vector<long> edges_lenght;
};

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<FaceDataStructure,K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds>  Triangulation;

typedef K::Point_2 P;
typedef Triangulation::Face_handle FaceHandle;
typedef std::unordered_map<FaceHandle, long> MaxDistanceMap;
typedef std::unordered_map<FaceHandle, std::unordered_map<int, long>> EdgeLengthMap;

Triangulation t;
std::vector<P> infected;
MaxDistanceMap distance, opened;
EdgeLengthMap edge_length;

void init(){

  for (FaceHandle f:t.finite_face_handles()){
    FaceDataStructure fds;
    fds.max_d = 0;
    fds.edges_lenght.resize(3, 0);
    for (int i=0;i<3;i++){
      int k = f->cw(i);
      int j = f->ccw(i);
      P a = f->vertex(k)->point();
      P b = f->vertex(j)->point();
      long ab_distance = CGAL::squared_distance(a, b);
      fds.edges_lenght[i] = ab_distance;
    }
    f->info() = fds;
  }
}

bool find_exit(FaceHandle& face, const long& d){
  opened[face] = 1;
  if (t.is_infinite(face)) return true;
  
  FaceDataStructure fds = face->info();
  if (fds.max_d >= d) return true;
  for (int i=0;i<3;i++){
    FaceHandle neighbor_face = face->neighbor(i);
    if (opened[neighbor_face]) continue;
    long ab_distance = fds.edges_lenght[i];
    if (ab_distance >= 4*d && find_exit(neighbor_face, d)){
      face->info().max_d = d;
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

  init();
  edge_length.clear();
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