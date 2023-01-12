///1
#include <iostream>
#include <vector>
#include <map>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>

#define trace(x) std::cerr << #x << " = " << x << std::endl;


typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef K::Point_2  P;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS    // Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
    >          graph;
typedef boost::graph_traits<graph>::vertex_descriptor    vertex_desc;    // Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).  
typedef boost::graph_traits<graph>::edge_iterator    edge_it;    // to iterate over all edges

int n;
std::vector<P> points;
std::map<P, int> point_idx;


int solve(long& rr){

  int l = 0;
  int r = n/2+1;
  while (l<r){
    int mid = (l+r)/2;

    // construct triangulation
    Triangulation t;
    t.insert(points.begin()+mid, points.end());

    // init graph
    graph G(n - mid);

    // construct graph with edgees where dist <= rr
    for (auto e=t.finite_edges_begin(); e!=t.finite_edges_end(); e++){
      if (t.segment(e).squared_length()<=rr){
        P p1 = e->first->vertex((e->second + 1) % 3)->point();
        P p2 = e->first->vertex((e->second + 2) % 3)->point();
        boost::add_edge(point_idx[p1]-mid, point_idx[p2]-mid, G); // - mid because we scale all
      }
    }

    // compute connected components
    std::vector<int> component_map(n); // We MUST use such a vector as an Exterior Property Map: Vertex -> Component
    int ncc = boost::connected_components(G,
     boost::make_iterator_property_map(component_map.begin(),
     boost::get(boost::vertex_index, G))); 

    // check whether there's a connected component with at least mid nodes
    bool found_k_points = false;
    std::vector<int> n_nodes_component(ncc, 0);
    for (int i = 0; i < n-mid; ++i) {
      n_nodes_component[component_map[i]]++;
      if (n_nodes_component[component_map[i]] >= mid){
        found_k_points = true;
        break;
      }
    }

    if (found_k_points){
      l = mid+1;
    }else{
      r = mid;
    }

  }

  return l-1;

}

void testcase(){
  long rr;
  std::cin >> n >> rr;
  rr*=rr;

  points.clear();
  points.resize(n);
  point_idx.clear();
  for (int i=0; i<n; i++){
    int x, y;
    std::cin >> x >> y;
    P p(x, y);
    points[i] = p;
    point_idx[p] = i;
  }

  std::cout << solve(rr) << std::endl;

  return;
}



int main(){
  std::ios_base::sync_with_stdio(false);
  int T; std::cin >> T;
  while (T--) testcase();
  return 0;
}