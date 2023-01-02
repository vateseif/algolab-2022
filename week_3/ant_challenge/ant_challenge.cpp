///1
#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>

#define trace(x) std::cerr << #x << " = " << x << std::endl;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;
typedef boost::graph_traits<weighted_graph>::edge_iterator              edge_it;



int MAX_WEIGHT = std::numeric_limits<int>::max();

void testcase(){
  int n, e, s, a, b;
  std::cin>>n>>e>>s>>a>>b;

  // overall graph
  weighted_graph G(n);
  weight_map weights = boost::get(boost::edge_weight, G);
  // private network for each specie
  std::vector<weighted_graph> networks(s, weighted_graph(n));
  std::vector<weight_map> networks_weights(s, boost::get(boost::edge_weight, G));

  // add weighted edges to graph
  for (int j=0; j<e; j++){
    int t1, t2;
    std::cin>>t1>>t2;
    for (int i=0; i<s; i++){
      int w; std::cin>>w;
      edge_desc e;
      e = boost::add_edge(t1, t2, networks[i]).first; networks_weights[i][e]=w;
    }
  }

  for (int i=0; i<s; i++){
    int h; std::cin>>h;
    std::vector<edge_desc> mst; // vector to store MST edges (not a property map!)
    boost::kruskal_minimum_spanning_tree(networks[i], std::back_inserter(mst));
    
    for (std::vector<edge_desc>::iterator it = mst.begin(); it != mst.end(); ++it) {
      int u = boost::source(*it, networks[i]);
      int v = boost::target(*it, networks[i]);
      edge_desc e;
      if(!edge(u,v,G).second) { // edge not yet in G
          e = boost::add_edge(u, v, G).first;
          weights[e] = networks_weights[i][*it];
        } else {
          e = edge(u,v,G).first;
          weights[e] = std::min(weights[e], networks_weights[i][*it]);
        }
    }  
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