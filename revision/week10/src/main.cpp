///3
#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>
#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>

#define trace(x) std::cerr << #x << " = " << x<< std::endl;

// Epic kernel is enough, no constructions needed, provided the squared distance
// fits into a double (!)
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
// we want to store an index with each vertex
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;

// As edges are not explicitly represented in the triangulation, we extract them
// from the triangulation to be able to sort and process them. We store the
// indices of the two endpoints, first the smaller, second the larger, and third
// the squared length of the edge. The i-th entry, for i=0,... of a tuple t can
// be accessed using std::get<i>(t).
typedef std::tuple<Index,Index,K::FT> Edge;
typedef std::vector<Edge> EdgeV;
typedef std::pair<K::Point_2,Index> IPoint;
  
int n, k, f0;
long s0;
std::vector<IPoint> points;


int ncc(long s, const EdgeV& edges){
  // setup and initialize union-find data structure
  boost::disjoint_sets_with_storage<> uf(n);
  int f = n;
  // ... and process edges in order of increasing length
  for (EdgeV::const_iterator e = edges.begin(); e != edges.end(); ++e) {
    // squared length of edge
    long dd = std::get<2>(*e);
    // determine components of endpoints
    Index c1 = uf.find_set(std::get<0>(*e));
    Index c2 = uf.find_set(std::get<1>(*e));
    if (dd < s && c1 != c2) {
      // this edge connects two different components => part of the emst
      uf.link(c1, c2);
      f--;
    }
  }
  return f;
}



void testcase(){
  std::cin >> n >> k >> f0 >> s0;

  points.clear();
  points.reserve(n);
  for (Index i = 0; i < n; ++i) {
    int x, y;
    std::cin >> x >> y;
    points.emplace_back(K::Point_2(x, y), i);
  }

  Delaunay t;
  t.insert(points.begin(), points.end());

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

  
  int f = ncc(s0, edges);
  
  //binary search on max edge length
  int l = 0;
  int r = edges.size();
  while (l<r){
    int mid = (l+r)/2;
    int n_c = ncc(std::get<2>(edges[mid]), edges);
    if (n_c < f0){
      r = mid;
    }else{
      l = mid+1;
    }
  }
  long s = std::get<2>(edges[l-1]);
  
  std::cout << s << " " << f << std::endl;


}


int main(){
  std::ios_base::sync_with_stdio(false);
  int T; std::cin >> T;
  //T=2;
  while (T--) testcase();
  return 0;
}