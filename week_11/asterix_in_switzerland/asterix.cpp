///2
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

#define trace(x) std::cerr << #x << " = " << x << std::endl;

// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

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


void testcase(){
  int n, m;
  std::cin >> n >> m;

  // init graph
  graph G(n);
  edge_adder adder(G);
  // Add special vertices source and sink
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);

  int sum_pos = 0;
  for (int i=0; i<n; i++){
    int b; std::cin >> b;
    if (b>=0){
      sum_pos += b;
      adder.add_edge(v_source, i, b);
    }else{
      adder.add_edge(i, v_sink, -b);
    }
  }

  for (int i=0; i<m; i++){
    int u, v, d;
    std::cin >> u >> v >> d;
    adder.add_edge(u, v, d);
  }

  int flow = boost::push_relabel_max_flow(G, v_source, v_sink);
  std::string out = sum_pos-flow>0? "yes" : "no";
  
  std::cout << out << std::endl;

  
  return;
}



int main(){
  std::ios_base::sync_with_stdio(false);
  int T; std::cin >> T;
  while (T--) testcase();
  return 0;
}