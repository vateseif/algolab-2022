#include <iostream>
#include <string>
#include <limits>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

#define trace(x) std::cerr << #x << " = " << x << std::endl;

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

int n, m;

void testcase(){
  std::cin >> n >> m;
  // construct graph
  graph G(n+m);
  edge_adder adder(G);
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);

  /*
  idea: solve as circulation problem as in lecture.
  nodes: 
    players:  0...n-1 are the nodes connecting to the target (edge capacity 1)
    games:    n...n+m-1 are the nodes connecting to the source (edge capacity 1)
  connections:
    c=0:      connect game to both a and b
    c=1||2:   connect game to either a or b
  output:
    max_flow has to be equal to the sum of scores and number of games m
  */

  for (int i=0;i<m;i++){
    int a, b, c;
    std::cin>> a >> b >> c;
    if (c==1){
      adder.add_edge(n+i, a, 1);
    } else if (c==2){
      adder.add_edge(n+i, b, 1);
    } else{
      adder.add_edge(n+i, a, 1);
      adder.add_edge(n+i, b, 1);
    }
    adder.add_edge(v_source, n+i, 1);
  }

  long sum_score = 0;
  for (int i=0; i<n; i++){
    long si;
    std::cin >> si;
    adder.add_edge(i, v_sink, si);
    sum_score += si;
  }
  long max_flow = boost::push_relabel_max_flow(G, v_source, v_sink);


  std::string out = (max_flow==sum_score && max_flow==m) ? "yes" : "no";
  std::cout << out << std::endl;

  return;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int T; std::cin>>T;
  while(T--)testcase();
  return 0;
}