#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>

// BGL flow include *NEW*
#include <boost/graph/push_relabel_max_flow.hpp>

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


std::vector<std::vector<int>> limbs;

void testcase(){
  int n, m;
  std::cin >> n >> m;
  // store cost of limbs for each figure
  limbs.clear();
  limbs.resize(n, std::vector<int>(n, 0));
  for (int i=0; i<m; i++){
    int a, b, c;
    std::cin >> a >> b >> c;
    limbs[a][b] += c;
  }

  // construct graph
  graph G(n);
  edge_adder adder(G);

  // add limb connection with cost
  for (int i=0; i<n; i++){
    for (int j=0; j<n; j++){
      if (limbs[i][j]>0){
        adder.add_edge(i, j, limbs[i][j]);
      }
    }
  }

  // compute min cut in circle
  int min_cost = boost::push_relabel_max_flow(G, n-1, 0);
  for (int i=0; i<n-1; i++){
    int cost = boost::push_relabel_max_flow(G, i, i+1);
    min_cost = std::min(min_cost, cost);
  }

  std::cout << min_cost << std::endl;

}



int main(){
  std::ios_base::sync_with_stdio(false);
  int T; std::cin >> T;
  while (T--) testcase();
  return 0;
}