#include <iostream>
#include <vector>
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


int n;
const int n_knight_moves = 8;
std::vector<std::vector<int>> cells;
std::vector<std::pair<int, int>> knight_move;


void init_knight_move(){
  knight_move.resize(n_knight_moves);
  knight_move[0] = std::make_pair(-1, -2);
  knight_move[1] = std::make_pair(-1, +2);
  knight_move[2] = std::make_pair(+1, -2);
  knight_move[3] = std::make_pair(+1, +2);
  knight_move[4] = std::make_pair(-2, -1);
  knight_move[5] = std::make_pair(-2, +1);
  knight_move[6] = std::make_pair(+2, -1);
  knight_move[7] = std::make_pair(+2, +1);
}

bool cellIsValid(int& i, int& j){
  if (i<0 || i>=n || j<0 || j>=n || !cells[i][j]) return false;
  return true;
}

void connect_node(edge_adder& adder, int& i, int& j){
  const int from = i*n + j;
  for (std::pair<int,int> move : knight_move){
    int ii = i+move.first;
    int jj = j+move.second;
    if (cellIsValid(ii, jj)){
      const int to = ii*n + jj;
      adder.add_edge(from, to, 1);
    }
  }
}

void testcase(){
  std::cin>>n;

  graph G(n*n);
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);
  edge_adder adder(G);

  int n_empty_cells = 0;
  cells.clear(); 
  cells.resize(n, std::vector<int>(n));
  for (int i=0;i<n;i++){
    for (int j=0;j<n;j++){
      int c;
      std::cin>>c;
      cells[i][j] = c;
      if (!c) n_empty_cells++;
    }
  }

  for (int i=0; i<n; i++){
    for (int j=0;j<n;j++){
      // connect nodes to cells that knights would target
      if ((i+j)%2 == 0){
        if (cellIsValid(i, j)){
          connect_node(adder, i, j);
          adder.add_edge(v_source, i*n + j, 1);
        }
      }else{ // connect right partition to target
        if (cellIsValid(i, j)) adder.add_edge(i*n + j, v_sink, 1);
      }
    }
  }

  // Find a min cut via maxflow
  int flow = boost::push_relabel_max_flow(G, v_source, v_sink);
  
  std::cout << n*n-n_empty_cells-flow << std::endl;

  return;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  init_knight_move();
  int T; std::cin>>T;
  while (T--) testcase();
  return 0;
} 