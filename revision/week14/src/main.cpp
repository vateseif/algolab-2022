#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

typedef traits::vertex_descriptor                               vertex_desc;
typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator

// Custom edge adder class
class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, long capacity, long cost) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G); // new!
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;   // new assign cost
    w_map[rev_e] = -cost;   // new negative cost
  }
};

std::vector<int> out_e, in_w;

void testcase(){
  int e, w, m, d, p, l;
  std::cin >> e >> w >> m >> d >> p >> l;

  // Create graph, edge adder class and propery maps
  graph G(2*(e+w));
  edge_adder adder(G);
  auto c_map = boost::get(boost::edge_capacity, G);
  auto rc_map = boost::get(boost::edge_residual_capacity, G);
  // Add special vertices source and sink
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);
  const vertex_desc v_source2 = boost::add_vertex(G);
  const vertex_desc v_sink2 = boost::add_vertex(G);

  // connect non dangerous matches
  out_e.clear(); out_e.resize(e);
  in_w.clear(); in_w.resize(w);
  for (int i=0; i<m; i++){
    int u, v, r;
    std::cin >> u >> v >> r;
    adder.add_edge(u, e+v, 1, r);
    in_w[v]++;
    out_e[u]++;
  }

  // enforce l constraint on non-dangerous matches for east
  int sum_needed_matches = 0;
  for (int i=0; i<e; i++){
    int needed_matches = std::min(out_e[i], l);
    int remaining_matches = out_e[i] - needed_matches;
    adder.add_edge(v_source, i, needed_matches, 0);
    adder.add_edge(v_source2, i, remaining_matches, 0);
    sum_needed_matches += needed_matches;
  }
  adder.add_edge(v_source, v_source2, p-sum_needed_matches, 0);

  // enforce l constraint for matches of west
  sum_needed_matches = 0;
  for (int i=0; i<w; i++){
    int needed_matches = std::min(in_w[i], l);
    int remaining_matches = in_w[i] - needed_matches;
    adder.add_edge(e+i, v_sink, needed_matches, 0);
    adder.add_edge(e+i, v_sink2, remaining_matches, 0);
    sum_needed_matches += needed_matches;
  }
  adder.add_edge(v_sink2, v_sink, p-sum_needed_matches, 0);


  // create 2nd group of nodes connected to source2 and sink2 only
  for (int i=0; i<d; i++){
    int u, v, r;
    std::cin >> u >> v >> r;
    adder.add_edge((e+w)+u, (2*e+w)+v, 1, r);
    adder.add_edge(v_source2 ,(e+w)+u, 1, 0);
    adder.add_edge((2*e+w)+v, v_sink2, 1, 0);
  }

  // compute min cost
  boost::successive_shortest_path_nonnegative_weights(G, v_source, v_sink);
  int cost = boost::find_flow_cost(G);

  // compute flow
  int flow = 0;
  out_edge_it ebeg, eend;
  for(boost::tie(ebeg, eend) = boost::out_edges(boost::vertex(v_source,G), G); ebeg != eend; ++ebeg) {
    flow += c_map[*ebeg] - rc_map[*ebeg];     
  }


  std::string out = flow==p? std::to_string(cost) : "No schedule!";
  std::cout << out << std::endl; 

}



int main(){
  std::ios_base::sync_with_stdio(false);
  int T; std::cin >> T;
  while (T--) testcase();
  return 0;
}