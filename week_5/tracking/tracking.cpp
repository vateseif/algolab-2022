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

std::vector<int> dmap;

void testcase(){
  int n, m, k, x, y;
  std::cin >> n >> m >> k >> x >> y;

  weighted_graph G(n*(k+1));
  weight_map weights = boost::get(boost::edge_weight, G);
  edge_desc e;
  
  for (int i=0; i<m; i++){
    int a, b, c, d;
    std::cin >> a >> b >> c >> d;
    if (d == 1){
      for (int l=0; l<k; l++){
        e = boost::add_edge(n*l+a, n*(l+1)+b, G).first; weights[e]=c;
        e = boost::add_edge(n*l+b, n*(l+1)+a, G).first; weights[e]=c;
      }
      e = boost::add_edge(n*k+a, n*k+b, G).first; weights[e]=c;
    }else{
      for (int l=0; l<k+1; l++){
        e = boost::add_edge(n*l+a, n*l+b, G).first; weights[e]=c;
      }
    }
  }

  dmap.clear();
  dmap.resize(n*(k+1));
  boost::dijkstra_shortest_paths(G, x,
    boost::distance_map(boost::make_iterator_property_map(
      dmap.begin(), boost::get(boost::vertex_index, G))));

  std::cout << dmap[n*k+y] << std::endl;
  
}


int main(){

  std::ios_base::sync_with_stdio(false);
  int T; std::cin >> T;
  while (T--) testcase();
  return 0;
}