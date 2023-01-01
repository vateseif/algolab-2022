///1
#include <iostream>
#include <vector>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;


#define trace(x) std::cerr << #x << " = " << x << std::endl;

void testcase(){
  int n, e, s, a, b;
  std::cin>>n>>e>>s>>a>>b;

  weighted_graph G(n);
  weight_map weights = boost::get(boost::edge_weight, G);

  // add weighted edges to graph
  for (int j=0; j<e; j++){
    int t1, t2;
    std::cin>>t1>>t2;
    for (int i=0; i<s; i++){
      int w; std::cin>>w;
      edge_desc e;
      e = boost::add_edge(t1, t2, G).first; weights[e]=w;
      e = boost::add_edge(t2, t1, G).first; weights[e]=w;
    }
  }

  // store hives
  for (int i=0; i<s; i++){
    int h; std::cin>>h;
  }

  std::vector<int> dist_map(n);

  boost::dijkstra_shortest_paths(G, a,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));

  std::cout << dist_map[b] << std::endl;
  return;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int T; std::cin >> T;
  while (T--) testcase();
  return 0;
}