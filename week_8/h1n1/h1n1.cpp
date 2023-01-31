///
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

#define trace(x) std::cerr << #x << " = " << x << std::endl;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<long,K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;
typedef K::Point_2 P;

std::vector<P> infected;
std::vector<long> memo; // stores max escape radius for each face of triangulation
Triangulation t;

void compute_max_radius(){
  std::queue<std::pair<long, Triangulation::All_faces_iterator>> Q;
  
  for (auto f=t.all_faces_begin(); f!=t.all_faces_end(); f++){
    if (t.is_infinite(f)){
      Q.emplace(std::numeric_limits<long>::max(), f);
    }
    f->info() = 0;
  }

  while (!Q.empty()){
    auto cf = Q.front(); Q.pop();
    long cost = cf.first;
    auto fu = cf.second;

    if (fu->info() >= cost){
      continue;
    }

    fu->info() = cost;

    for (int i=0; i<3; i++){
      auto fv = fu->neighbor(i);
      if (t.is_infinite(fv)) continue;
      long uv_edge = CGAL::squared_distance(fu->vertex(t.cw(i))->point(),
                                            fu->vertex(t.ccw(i))->point());
      long min_cost = std::min(cost, uv_edge);
      if (fv->info() >= min_cost) continue;
      Q.emplace(min_cost, fv);
    }
  }
}


bool testcase(){
  int n; std::cin >> n;
  if (!n) return false;

  // load infected positions
  infected.clear(); infected.resize(n);
  for (int i=0; i<n; i++){
    long x, y; std::cin >> x >> y;
    infected[i] = P(x, y);
  }

  // construct triangulation
  t.clear();
  t.insert(infected.begin(), infected.end());

  // compute max escape radius for each face
  compute_max_radius();

  int m; std::cin >> m;
  for (int i=0; i<m; i++){
    long x, y, d;
    std::cin >> x >> y >> d;
    P p(x, y);
    
    // nearest infected
    P nearest = t.nearest_vertex(p)->point();
    if (CGAL::squared_distance(p, nearest) < d){
      std::cout << "n";
      continue;
    }
    
    // locate point in face
    auto face = t.locate(p);
    if (4*d <= face->info()){
      std::cout << "y";
    }else{
      std::cout << "n";
    }
  }

  std::cout << std::endl;
  
  return true;
}




int main(){
  std::ios_base::sync_with_stdio(false);
  while (testcase());
  return 0;
}