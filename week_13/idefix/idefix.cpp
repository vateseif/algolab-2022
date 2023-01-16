///4
#include <cmath>
#include <limits>
#include <unordered_map>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>
#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>

// BGL includes 
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>

#define trace(x) std::cerr << #x << " = " << x << std::endl;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
//typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;


// Epic kernel is enough, no constructions needed, provided the squared distance
// fits into a double (!)
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
// we want to store an index with each vertex
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Triangulation;

// As edges are not explicitly represented in the triangulation, we extract them
// from the triangulation to be able to sort and process them. We store the
// indices of the two endpoints, first the smaller, second the larger, and third
// the squared length of the edge. The i-th entry, for i=0,... of a tuple t can
// be accessed using std::get<i>(t).
typedef std::tuple<Index,Index,K::FT> Edge;
typedef std::vector<Edge> EdgeV;
typedef K::Point_2  P;
typedef std::pair<P,Index> IPoint;
  

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS    // Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
    >          graph;
typedef boost::graph_traits<graph>::vertex_descriptor    vertex_desc;    // Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).  
typedef boost::graph_traits<graph>::edge_descriptor       edge_desc;    // Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).  
typedef boost::graph_traits<graph>::edge_iterator    edge_it;    // to iterate over all edges

int n, m;
std::vector<IPoint> trees;
std::vector<P> bones;
std::unordered_map<P, int> bone_count;
std::vector<std::pair<int, long>> closest_tree; // index of tree and squared
EdgeV edges;


std::pair<int, graph> compute_cc(Triangulation& t, long& max_rr){
  // init graph for connected components
  graph G(n);

  for (EdgeV::const_iterator e = edges.begin(); e != edges.end(); ++e) {
    // determine components of endpoints
    int c1 = std::get<0>(*e);
    int c2 = std::get<1>(*e);
    long seg_ll = std::get<2>(*e);
    if (c1 != c2 && seg_ll <= max_rr) {
      boost::add_edge(c1, c2, G);
    }else if (seg_ll > max_rr){
      break;
    }
  }
  // compute connected components
  std::vector<int> component_map(n); // We MUST use such a vector as an Exterior Property Map: Vertex -> Component
  int ncc = boost::connected_components(G, boost::make_iterator_property_map(component_map.begin(), 
            boost::get(boost::vertex_index, G))); 

  // compute number of bones that can  be inspected for each connected component
  std::vector<int> feasible_bones(ncc, 0);

  int max_n_bones = -1;
  for (int j=0; j<m; j++){
    if (4*closest_tree[j].second <= max_rr){
      max_n_bones = std::max(max_n_bones, ++feasible_bones[component_map[closest_tree[j].first]]);
    }
  }
  
  return std::make_pair(max_n_bones, G);
}

int compute_cc(Triangulation& t, graph& G, long& max_rr, long& prev_rr){
  
  if (max_rr > prev_rr){
    for (EdgeV::const_iterator e = edges.begin(); e != edges.end(); ++e) {
      // determine components of endpoints
      int c1 = std::get<0>(*e);
      int c2 = std::get<1>(*e);
      long seg_len = std::get<2>(*e);
      if (c1 != c2 && seg_len > prev_rr && seg_len <= max_rr) {
        boost::add_edge(c1, c2, G);
      }else if (seg_len > max_rr){
        break;
      }
    }
  }else{
    for (EdgeV::const_iterator e = edges.begin(); e != edges.end(); ++e) {
      // determine components of endpoints
      int c1 = std::get<0>(*e);
      int c2 = std::get<1>(*e);
      long seg_len = std::get<2>(*e);
      if (c1 != c2 && seg_len <= prev_rr && seg_len > max_rr) {
        boost::remove_edge(c1, c2, G);
      }else if (seg_len > prev_rr){
        break;
      }
    }
  }
  
  
  // compute connected components
  std::vector<int> component_map(n); // We MUST use such a vector as an Exterior Property Map: Vertex -> Component
  int ncc = boost::connected_components(G, boost::make_iterator_property_map(component_map.begin(), 
            boost::get(boost::vertex_index, G))); 

  // compute number of bones that can  be inspected for each connected component
  std::vector<int> feasible_bones(ncc, 0);

  int max_n_bones = -1;
  for (int j=0; j<m; j++){
    if (4*closest_tree[j].second <= max_rr){
      max_n_bones = std::max(max_n_bones, ++feasible_bones[component_map[closest_tree[j].first]]);
    }
  }
  
  return max_n_bones;
}

void testcase(){
  int k;
  long s;

  std::cin >> n >> m >> s >> k;


  trees.clear();
  trees.reserve(n);
  //tree_idx.clear();
  for (int i=0; i<n; i++){
    int x, y;
    std::cin >> x >> y;
    P p(x, y);
    trees.emplace_back(P(x, y), i);
  }
  
  Triangulation t;
  t.insert(trees.begin(), trees.end());
  
  
  edges.clear();
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



  closest_tree.clear();
  closest_tree.resize(m);
  long min_dd = std::numeric_limits<long>::max();
  long max_dd = std::numeric_limits<long>::min();
  //bone_count.clear();
  for (int i=0; i<m; i++){
    int x, y;
    std::cin >> x >> y;
    P p(x, y);
    auto nearest = t.nearest_vertex(p);
    long dd = CGAL::squared_distance(nearest->point(), p);
    closest_tree[i] = std::make_pair(nearest->info(), dd);
    min_dd = std::min(min_dd, dd);
    max_dd = std::max(max_dd, dd);
  }


  std::pair<int, graph> aG = compute_cc(t, s);
  int a = aG.first;
  graph G = aG.second;
  
  // run bisection to determine smallest b
  long l = 0;
  // find upper bound 
  long r = std::pow(2, 51);
  //if (a<k){
    //l = s;
    //r = 2*s;
  //}
  
  /*
  for (long i=1; i<std::pow(2, 51)/s; i++){
    long is = i*s;
    if (compute_cc(t, is) >= k) {
      l = (i-1)*s;
      r = i*s;
      break;
    }
  }
  */
  
  
  long prev_mid = 0;
  while (l<r){
    long mid = (l+r)/2;
    if (compute_cc(t, G, mid, prev_mid) < k){
      l = mid + 1;
    }else{
      r = mid;
    }
    prev_mid = mid;
  }
  long b = l;

  std::cout << a << " " << b << std::endl;


  return;
}



int main(){
  std::ios_base::sync_with_stdio(false);
  int  T; std::cin >> T;
  while(T--) testcase();  
  return 0;
}