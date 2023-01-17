///2
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>
#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <limits>

#define trace(x) std::cerr << #x << " = " << x << std::endl;

// Epic kernel is enough, no constructions needed, provided the squared distance
// fits into a double (!)
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Exact_predicates_exact_constructions_kernel EK;
// we want to store an index with each vertex
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Triangulation;
typedef std::tuple<Index,Index,K::FT> Edge;
typedef std::vector<Edge> EdgeV;
typedef K::Point_2    P;
typedef std::pair<P,Index> IPoint;

std::vector<IPoint> trees;


void testcase(){
  int n, h;
  long s;
  std::cin >> n >> s >> h;


  trees.clear(); trees.reserve(n);
  for(int i=0; i<n; i++){
    int x, y;
    std::cin >> x >> y;
    trees.emplace_back(P(x, y), i);
  }

  Triangulation t;
  t.insert(trees.begin(), trees.end());


  EdgeV edges;
  edges.reserve(3*n); // there can be no more in a planar graph
  for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
    Index i1 = e->first->vertex((e->second+1)%3)->info();
    Index i2 = e->first->vertex((e->second+2)%3)->info();
    // ensure smaller index comes first
    if (i1 > i2) std::swap(i1, i2);
    edges.emplace_back(i1, i2, t.segment(e).squared_length());
  }
  std::sort(edges.begin(), edges.end(),
      [](const Edge& e1, const Edge& e2) -> bool {
        return std::get<2>(e1) < std::get<2>(e2);
            });


  //a2 query
  int a2 = 0;
  long min_edge = std::numeric_limits<long>::max();
  for (EdgeV::const_iterator e = edges.begin(); e != edges.end(); ++e) {
    // determine components of endpoints
    // this edge connects two different components => part of the emst
    if (std::get<2>(*e) > min_edge) break;
    min_edge = std::get<2>(*e);
    a2++;
  }
  
  
  // a3 query
  int a3 = 0;
  EK::FT min_rr = std::numeric_limits<long>::max();
  for (Triangulation::Face_iterator f = t.finite_faces_begin(); f != t.finite_faces_end(); ++f) {
    P p = f->vertex(0)->point();
    P q = f->vertex(1)->point();
    P r = f->vertex(2)->point();
    EK::Point_2 ep(p.x(), p.y()), eq(q.x(), q.y()), er(r.x(), r.y());
    EK::Circle_2 c(ep, eq, er);
    EK::FT rr = c.squared_radius();
    if (rr < min_rr){
      a3 = 1;
      min_rr = rr;
    }else if(rr == min_rr){
      a3++;
    }
  } 

  std::cout << a2  << " ";
  std::cout << a3  << " ";
  std::cout << h  << " ";
  std::cout << h  << std::endl;


  return;
}




int main(){
  std::ios_base::sync_with_stdio(false);
  int T; std::cin >> T;
  while(T--) testcase();
  return 0;
}