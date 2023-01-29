///3
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>
#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>

#define trace(x) std::cerr << #x << " = " << x << std::endl;

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

Index n, m;
long s;
int k;
EdgeV bt_edges;
std::vector<IPoint> points;


void testcase(){
  std::cin >> n >> m >> s >> k;


  // store tree location
  points.clear();
  points.reserve(n);
  for (Index i=0; i<n; i++){
    int x, y;
    std::cin >> x >> y;
    points.emplace_back(K::Point_2(x,y), i);
  }

  // construct triangulation of trees
  Delaunay t;
  t.insert(points.begin(), points.end());

  // find shortest tree for each bone and store as edge
  bt_edges.clear();
  bt_edges.reserve(m);
  std::vector<int> component_map(n+m, 0);
  for (Index i=0; i<m; i++){
    int x, y;
    std::cin >> x >> y;
    Delaunay::Vertex_handle nearest = t.nearest_vertex(K::Point_2(x, y));
    long dd = CGAL::squared_distance(nearest->point(), K::Point_2(x, y));
    bt_edges.emplace_back(nearest->info(), n+i, K::FT(4*dd));
    component_map[nearest->info()]++;
  }

  // find and sort edges of tree triangulation
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


  //
  boost::disjoint_sets_with_storage<> uf(n);
  Index n_components = n;
  // ... and process edges in order of increasing length
  for (EdgeV::const_iterator e = edges.begin(); e != edges.end(); ++e) {
    // determine components of endpoints
    long dd = std::get<2>(*e);
    Index c1 = uf.find_set(std::get<0>(*e));
    Index c2 = uf.find_set(std::get<1>(*e));
    if (dd <= s && c1 != c2) {
      // this edge connects two different components => part of the emst
      uf.link(c1, c2);
      if (--n_components == 1) break;
    }
  }

  std::vector<int> bone_count(n, 0);
  for (EdgeV::const_iterator e = bt_edges.begin(); e != bt_edges.end(); ++e){
    long dd = std::get<2>(*e);
    if (dd <= s){
      Index c = uf.find_set(std::get<0>(*e));
      bone_count[c]++;
    }
    edges.push_back(*e);
  }

  int a = *std::max_element(bone_count.begin(), bone_count.end());
  std::cout << a << " ";

  std::sort(edges.begin(), edges.end(),
      [](const Edge& e1, const Edge& e2) -> bool {
        return std::get<2>(e1) < std::get<2>(e2);
    });

  long b = 4*s;
  bone_count.clear();
  bone_count.resize(n+m, 0);
  boost::disjoint_sets_with_storage<> uf2(n+m);
  for (EdgeV::const_iterator e = edges.begin(); e != edges.end(); ++e) {
    Index i1 = std::get<0>(*e);
    Index i2 = std::get<1>(*e);
    Index c1 = uf2.find_set(i1);
    Index c2 = uf2.find_set(i2);
    if (c1!=c2){
      int bc1 = bone_count[c1];
      int bc2 = bone_count[c2];
      uf2.link(c1, c2);
      Index c = uf2.find_set(i1);
      if (i2 >= n){
        bone_count[c] = bc1+1;
      }else{
        bone_count[c] = bc1+bc2;
      }
      
      if (bone_count[c]>=k){
        b = std::get<2>(*e);
        break;
      }
    }
  }

  std::cout << b << std::endl;

  return;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int T; std::cin >> T;
  while (T--) testcase();
}