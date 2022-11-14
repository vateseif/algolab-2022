
#include <iostream>
#include <vector>
#include <limits>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

#define trace(x) std::cerr << #x << " = " << x << std::endl;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >        weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type   weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor              edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor            vertex_desc;

std::vector<std::vector<int>> river_edges;
std::vector<int> dmap_source, dmap_target;


void testcase(){
  int n, m, k, x, y;
  std::cin >> n >> m >> k >> x >> y;

  weighted_graph G(n);
  weight_map weights = boost::get(boost::edge_weight, G);
  river_edges.clear();
  edge_desc e;
  for (int i=0; i<m; i++){
    int a, b, c, d;
    std::cin >> a >> b >> c >> d;
    e = boost::add_edge(a, b, G).first; weights[e]=c;
    if (d == 1) river_edges.push_back({a, b, c});
  }

  // compute SP from source and from target
  dmap_source.clear(); dmap_source.resize(n);
  dmap_target.clear(); dmap_target.resize(n);
  
  boost::dijkstra_shortest_paths(G, x,
    boost::distance_map(boost::make_iterator_property_map(
      dmap_source.begin(), boost::get(boost::vertex_index, G))));
  boost::dijkstra_shortest_paths(G, y,
    boost::distance_map(boost::make_iterator_property_map(
      dmap_target.begin(), boost::get(boost::vertex_index, G))));


  /*
  Find SP from x and from y to river edges then take one with lowest cost.
  Works only for the case k=1. 
  */
  int min_cost = std::numeric_limits<int>::max();
  for (auto e : river_edges){
    int p_c = std::min(dmap_source[e[0]] + dmap_target[e[1]],
                       dmap_source[e[1]] + dmap_target[e[0]]);
    min_cost = std::min(p_c+e[2], min_cost);
  }
  std::cout << min_cost << std::endl;
}


int main(){

  std::ios_base::sync_with_stdio(false);
  int T; std::cin >> T;
  while (T--) testcase();
  return 0;
}