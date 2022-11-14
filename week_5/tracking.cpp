///3
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

int n, x;

int solve(weighted_graph& G, int target, int k_missing){
  std::vector<int> dmap(n);
  boost::dijkstra_shortest_paths(G, target,
    boost::distance_map(boost::make_iterator_property_map(
      dmap.begin(), boost::get(boost::vertex_index, G))));
  if (k_missing == 0) return dmap[x];
  int min_cost = std::numeric_limits<int>::max();
  for (auto e : river_edges){
    int p_c = std::min(solve(G, e[0], k_missing-1) + dmap[e[1]],
                       solve(G, e[1], k_missing-1) + dmap[e[0]]);
    min_cost = std::min(min_cost, p_c+e[2]);
  }
  return min_cost;
}


void testcase(){
  int m, k, y;
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

  std::cout << solve(G, y, k) << std::endl;
  
}


int main(){

  std::ios_base::sync_with_stdio(false);
  int T; std::cin >> T;
  while (T--) testcase();
  return 0;
}