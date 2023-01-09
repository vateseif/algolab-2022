///4
#include <iostream>
#include <limits>
#include <algorithm>
#include <map>
#include <queue>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>

// BGL includes 
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>

#define trace(x) std::cerr << #x << " = " << x << std::endl;
// BGL Graph definitions
// =====================
// Graph Type, OutEdgeList Type, VertexList Type, (un)directedS
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS    // Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
    >          graph;
typedef boost::graph_traits<graph>::vertex_descriptor    vertex_desc;    // Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).  
typedef boost::graph_traits<graph>::edge_iterator    edge_it;    // to iterate over all edges

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
typedef std::tuple<Index,Index,long> Edge;
typedef std::vector<Edge> EdgeV;
typedef std::pair<K::Point_2,Index> IPoint;
 
 
int n, k, f0;

int ncc(EdgeV& edges, long max_s, int k){
  graph G(n);

  for (EdgeV::const_iterator e = edges.begin(); e != edges.end(); ++e) {
    // determine components of endpoints
    Index c1 = std::get<0>(*e);
    Index c2 = std::get<1>(*e);
    if (c1 != c2 && std::get<2>(*e) < max_s) {
      // this edge connects two different components => part of the emst
      boost::add_edge(c1, c2, G);
    }
  }
  
  std::vector<int> component_map(n); // We MUST use such a vector as an Exterior Property Map: Vertex -> Component
  int nc = boost::connected_components(G,
  boost::make_iterator_property_map(component_map.begin(), boost::get(boost::vertex_index, G))); 

  std::vector<int> component_size(nc);
  for (int i=0; i<n; i++) component_size[component_map[i]]++;
  
  std::sort(component_size.begin(), component_size.end());
  auto it = std::lower_bound(component_size.begin(), component_size.end(), k);

  int comp_geq_k = std::distance(component_size.begin(), it);

  int f = nc-comp_geq_k;
  int r = comp_geq_k-1;
  for (int i=0; i<r; i++){
    if (component_size[i] < k){
      component_size[r] += component_size[i];
      if (component_size[r] >= k) {
        r--; 
        f++;
      }
    }
  }

  return f;
}


void testcase(){
  
  long s0;
  std:: cin >> n >> k >> f0 >> s0;
  
  std::vector<IPoint> points;
  points.reserve(n);
  for (int i=0; i<n; i++){
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


  
  int l=0;
  int r = edges.size();

  while(l<r){
    int mid = (l+r)/2;
    EdgeV::const_iterator e = edges.begin()+mid;
    long dd = std::get<2>(*e);
    if (ncc(edges, dd, k)<f0){
      r = mid;
    }else{
      l = mid+1;
    }
  }

  
  int f = ncc(edges, s0, k);

  std::cout << std::get<2>(*(edges.begin()+l-1)) << " " << f << std::endl;
  

  return;
}


int main(){
  std::ios_base::sync_with_stdio(false);
  int T; std::cin >> T;
  //T=1;
  while (T--) testcase();
  return 0;
}