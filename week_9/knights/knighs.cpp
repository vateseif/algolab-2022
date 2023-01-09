///1
#include <iostream>
#include <vector>
// BGL include
#include <boost/graph/adjacency_list.hpp>

// BGL flow include *NEW*
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

int m, n, k, c;
std::vector<std::pair<int, int>> moves;

void init_moves(){
  moves.clear(); moves.resize(4);
  moves[0] = std::make_pair(-1, 0);
  moves[1] = std::make_pair(1, 0);
  moves[2] = std::make_pair(0, -1);
  moves[3] = std::make_pair(0, 1);
  
}

int ij2n(int i, int j, int io){
  return io*n*m+i*m+j;
}

bool is_valid(int i, int j){
  if (i<0 || i>=n || j<0 || j>=m) return false;
  return true;
}

int exit_ways(int i, int j){
  if (i==0 && (j==m-1 || j==0)) return 2;
  if (i==n-1 && (j==m-1 || j==0)) return 2;
  if (j==0 && (i==n-1 || i==0)) return 2;
  if (j==m-1 && (i==n-1 || i==0)) return 2;
  return 1;
}


bool border_node(int i, int j){
  if (i==n-1 || i==0 || j==m-1 || j==0) return true;
  return false;
}


void testcase(){
  std::cin >> m >> n >> k >> c;

  // init graph
  graph G(2*n*m);
  edge_adder adder(G);
  // Add special vertices source and sink
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);

  for (int i=0; i<n; i++){
    for (int j=0; j<m; j++){
      adder.add_edge(ij2n(i,j,0), ij2n(i,j,1), c);
      if (border_node(i,j)) adder.add_edge(ij2n(i,j,1), v_sink, exit_ways(i,j));
      for (std::pair<int, int> m : moves){
        int i2 = i+m.first;
        int j2 = j+m.second;
        if (is_valid(i2, j2)){
          adder.add_edge(ij2n(i,j,1), ij2n(i2,j2,0), 1);
        }
      }
    }
  }


  for (int l=0; l<k; l++){
    int j, i;
    std::cin >> j >> i;
    adder.add_edge(v_source, ij2n(i,j,0), 1);
  }
  
  long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
  std::cout << flow << "\n";

  return;
}


int main(){
  std::ios_base::sync_with_stdio(false);
  int T; std::cin >> T;
  init_moves();
  while (T--) testcase();
  return 0;
}