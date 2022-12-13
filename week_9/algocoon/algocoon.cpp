#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <queue>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>

#define trace(x) std::cerr << #x << " = " << x << std::endl;

typedef  boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
  boost::property<boost::edge_capacity_t, long,
    boost::property<boost::edge_residual_capacity_t, long,
      boost::property<boost::edge_reverse_t, traits::edge_descriptor> > > >  graph;
// Interior Property Maps
typedef traits::vertex_descriptor vertex_desc;
typedef  boost::graph_traits<graph>::edge_descriptor      edge_desc;
typedef  boost::graph_traits<graph>::out_edge_iterator      out_edge_it;

int n;
std::vector<std::vector<long>> cutting_cost;


class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

void testcase(){
  int m;
  std::cin>>n>>m;
  
  cutting_cost.clear();
  cutting_cost.resize(n, std::vector<long>(n, 0));
  for (int i=0;i<m;i++){
    int a, b, c;
    std::cin>>a>>b>>c;
    cutting_cost[a][b] += c;
  }

  graph G(n);
  edge_adder adder(G);
  for (int a=0;a<n;a++){
    for (int b=0;b<n;b++){
      if (!cutting_cost[a][b]) continue;
      adder.add_edge(a, b, cutting_cost[a][b]);
    }
  }
  
  long min_cost = std::numeric_limits<long>::max();
  for (int a=1;a<n;a++){
    long r_flow = boost::push_relabel_max_flow(G, a, 0);
    long flow = boost::push_relabel_max_flow(G, 0, a);
    min_cost = std::min(min_cost, std::min(flow, r_flow));
  }

  std::cout << min_cost << std::endl;

}


int main(){
  std::ios_base::sync_with_stdio(false);
  int T;
  std::cin>>T;
  while(T--) testcase();
  return 0;
}