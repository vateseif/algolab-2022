// ALGOLAB BGL Tutorial 3
// Code demonstrating 
// - MinCostMaxFlow with arbitrary edge costs using cycle_canceling
// - MinCostMaxFlow with non-negative edge costs using successive_shortest_path_nonnegative_weights

// Includes
// ========
#include <iostream>
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


int main() {
    const int N=7;
    const int v_source = 0;
    const int v_farm1 = 1;
    const int v_farm2 = 2;
    const int v_farm3 = 3;
    const int v_shop1 = 4;
    const int v_shop2 = 5;
    const int v_target = 6;

    // Create graph, edge adder class and propery maps
    graph G(N);
    edge_adder adder(G);  
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto rc_map = boost::get(boost::edge_residual_capacity, G);
    
    // Add the edges
    adder.add_edge(v_source, v_farm1, 3, 0);
    adder.add_edge(v_source, v_farm2, 1, 0);
    adder.add_edge(v_source, v_farm3, 2, 0);

    adder.add_edge(v_farm1, v_shop1, 1, 1);
    adder.add_edge(v_farm1, v_shop2, 1, 5);
    adder.add_edge(v_farm2, v_shop1, 1, 2);
    adder.add_edge(v_farm2, v_shop2, 1, 2);
    adder.add_edge(v_farm3, v_shop1, 1, 3);
    adder.add_edge(v_farm3, v_shop2, 2, 2);

    adder.add_edge(v_shop1, v_target, 3, 0);
    adder.add_edge(v_shop2, v_target, 3, 0);

    // Run the algorithm

    // Option 1: Min Cost Max Flow with cycle_canceling
    int flow1 = boost::push_relabel_max_flow(G, v_source, v_target);
    boost::cycle_canceling(G);
    int cost1 = boost::find_flow_cost(G);
    std::cout << "-----------------------" << "\n";
    std::cout << "Minimum Cost Maximum Flow with cycle_canceling()" << "\n";
    std::cout << "flow " << flow1 << "\n"; // 5
    std::cout << "cost " << cost1 << "\n"; // 12

    // Option 2: Min Cost Max Flow with successive_shortest_path_nonnegative_weights  
    boost::successive_shortest_path_nonnegative_weights(G, v_source, v_target);
    int cost2 = boost::find_flow_cost(G);
    std::cout << "-----------------------" << std::endl;
    std::cout << "Minimum Cost Maximum Flow with successive_shortest_path_nonnegative_weights()" << "\n";
    std::cout << "cost " << cost2 << "\n"; // 12
    // Iterate over all edges leaving the source to sum up the flow values.
    int s_flow = 0;
    out_edge_it e, eend;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(v_source,G), G); e != eend; ++e) {
        std::cout << "edge from " << boost::source(*e, G) << " to " << boost::target(*e, G) 
            << " with capacity " << c_map[*e] << " and residual capacity " << rc_map[*e] << "\n";
        s_flow += c_map[*e] - rc_map[*e];     
    }
    std::cout << "s-out flow " << s_flow << "\n"; // 5
    // Or equivalently, you can do the summation at the sink, but with reversed edge.
    int t_flow = 0;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(v_target,G), G); e != eend; ++e) {
        std::cout << "edge from " << boost::target(*e, G) << " to " << boost::source(*e, G) 
            << " with capacity " << c_map[r_map[*e]] << " and residual capacity " << rc_map[r_map[*e]] << "\n";
        t_flow += rc_map[*e] - c_map[*e];  
    }
    std::cout << "t-in flow " << t_flow << "\n"; // 5
    std::cout << "-----------------------" << "\n";

    return 0;
}
