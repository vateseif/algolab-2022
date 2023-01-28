///1
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



int m, n, k, c;
std::vector<std::pair<int, int>> moves;


void init_moves(){
  moves.clear();
  moves.resize(4);

  moves[0] = std::make_pair(0, 1);
  moves[1] = std::make_pair(1, 0);
  moves[2] = std::make_pair(0, -1);
  moves[3] = std::make_pair(-1, 0);
}


int ij2node(int i, int j, int out){
  // return node number given row, column and input/output
  return out*(n*m)+i*m+j;
}

bool valid_node(int i, int j){
  if (i<0 || i >= n || j<0 || j>= m) return false;
  return true;
}

int n_exits(int i, int j){
  int exits = 0;
  if (i==0 || i==n-1) exits++;
  if (j==0 || j==m-1) exits++;
  return exits;
}

void construct_graph(edge_adder& adder,  const vertex_desc& v_sink){
  for (int i=0; i<n; i++){
    for (int j=0; j<m; j++){
      // connect to sink ifon edge
      int exits = n_exits(i, j);
      adder.add_edge(ij2node(i, j, 1), v_sink, exits);
      // impose intersection constraint of C knights
      adder.add_edge(ij2node(i, j, 0), ij2node(i, j, 1), c);
      // connect to neighbor intersections
      for (std::pair<int, int> move : moves){
        int i2 = i + move.first;
        int j2 = j + move.second;
        if (valid_node(i2, j2)){
          // connnect neighbor intersetions (impose hallway usable only once)
          adder.add_edge(ij2node(i, j, 1), ij2node(i2, j2, 0), 1);
        }
      }
    }
  }
}


void testcase(){
  std::cin >> m >> n >> k >> c;

  // init graph (each intersection has 2 nodes)
  graph G(2*(n*m));
  edge_adder adder(G);
  // Add special vertices source and sink
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);

  // loop through knights location and connect source
  for (int i=0; i<k; i++){
    int col, row;
    std::cin >> col >> row;
    adder.add_edge(v_source, ij2node(row, col, 0), 1);
  }

  // construct graph
  construct_graph(adder, v_sink);

  long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
  std::cout << flow << "\n";

  return;
}



int main(){
  std::ios_base::sync_with_stdio(false);
  init_moves();
  int T; std:: cin >> T;
  while (T--) testcase();
  return 0;
}