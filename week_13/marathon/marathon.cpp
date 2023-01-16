///2
#include <iostream>
#include <vector>
#include <algorithm>
// BGL include
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

// BGL flow include *NEW*
#include <boost/graph/push_relabel_max_flow.hpp>

#define trace(x) std::cerr << #x << " = " << x << std::endl;

// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

//typedef traits::vertex_descriptor vertex_desc;
//typedef traits::edge_descriptor edge_desc;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

// Custom edge adder class, highly recommended
class edge_adder {
  graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const auto e = boost::add_edge(from, to, G).first;
    const auto rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

struct Street{
  int a, b, c, d;
  Street(){};

  Street(int ai, int bi, int ci, int di){
    a = ai; b = bi; c = ci; d = di;
  }
};

std::vector<Street> streets;


void compute_dist_map(weighted_graph& G, std::vector<int>& dist_map, int start){
  boost::dijkstra_shortest_paths(G, start,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));
}


void testcase(){
  int n, m, s, f;
  std::cin >> n >> m >> s >> f;
  

  // compute shortest path from s to all nodes
  weighted_graph G_dijkstra(n);
  weight_map weights = boost::get(boost::edge_weight, G_dijkstra);

  streets.clear(); streets.resize(m);
  for (int i=0; i<m; i++){
    int a, b, c, d;
    std::cin >> a >> b >> c >> d;
    edge_desc e;
    e = boost::add_edge(a, b, G_dijkstra).first; weights[e]=d;
    e = boost::add_edge(b, a, G_dijkstra).first; weights[e]=d;
    streets[i] = Street(a, b, c, d);
  }

  // shortest path from f to all nodes
  std::vector<int>         f_dist_map(n);
  compute_dist_map(G_dijkstra, f_dist_map, f);

  // shortest path from s to all nodes
  std::vector<int>         s_dist_map(n);
  compute_dist_map(G_dijkstra, s_dist_map, s);
  

  int min_cost = s_dist_map[f];

  // init graph for max flow
  graph G_flow(n);
  edge_adder adder(G_flow);


  // loop through all egdes to see if on a shortest path
  for (Street st : streets){
    if (s_dist_map[st.a] + f_dist_map[st.b] + st.d == min_cost){
      adder.add_edge(st.a, st.b, st.c);
    } else if (s_dist_map[st.b] + f_dist_map[st.a] + st.d == min_cost){
      adder.add_edge(st.b, st.a, st.c);
    }
  }

  long flow = boost::push_relabel_max_flow(G_flow, s, f);
  std::cout << flow << "\n";

  return;

}


int main(){
  std::ios_base::sync_with_stdio(false);
  int T; std::cin >> T;
  while(T--) testcase();
  return 0;
}