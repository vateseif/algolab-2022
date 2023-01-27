///2
#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

#define trace(x) std::cerr << #x << " = " << x<< std::endl;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

void testcase(){
  int n, m, k, x, y;
  std::cin >> n >> m >> k >> x >> y;

  // construct multilayered (k+1 layers) graph of nodes
  weighted_graph G((k+1)*n);
  weight_map weights = boost::get(boost::edge_weight, G);
  // Add special vertices source and sink
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);

  // connect edges of graph, go down layer if edge along river, else add edge for all layers
  for (int i=0; i<m; i++){
    int a, b, c, d;
    std::cin >> a >> b >> c >> d;
    // connect different layers if along a river
    if (d==1){
      edge_desc e;
      for (int j=0; j<k; j++){
        e = boost::add_edge(n*j+a, n*(j+1)+b, G).first; weights[e]=c;
        // connect (a, b), (b,a) always going down in layers
        e = boost::add_edge(n*j+b, n*(j+1)+a, G).first; weights[e]=c;
      }
      e = boost::add_edge(n*k+a, n*k+b, G).first; weights[e]=c;
      e = boost::add_edge(n*k+b, n*k+a, G).first; weights[e]=c;
    }else{
      edge_desc e;
      for (int j=0; j<=k; j++){
        e = boost::add_edge(n*j+a, n*j+b, G).first; weights[e]=c;
        e = boost::add_edge(n*j+b, n*j+a, G).first; weights[e]=c;
      }
    }
  }

  // connect source to x of first layer
  edge_desc e;
  e = boost::add_edge(v_source, x, G).first; weights[e]=0;
  // connect sink to y of last layer
  e = boost::add_edge(k*n+y, v_sink, G).first; weights[e]=0;

  // compute shortest path from source to sink
  std::vector<int> dist_map((k+1)*n+2);

  
  boost::dijkstra_shortest_paths(G, v_source,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));

  std::cout << dist_map[v_sink] << std::endl;

  return;
}


int main(){
  std::ios_base::sync_with_stdio(false);
  int T; std::cin >> T;
  while (T--) testcase();
  return 0;
}