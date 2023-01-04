///3
#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

#define trace(x) std::cerr << #x << " = " << x << std::endl;

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

int w, h;
std::vector<std::vector<bool>> garden;
std::vector<std::pair<int, int>> moves;

void init_moves(){
  moves.clear();
  moves.resize(4);
  moves[0] = std::make_pair(-1, 0);
  moves[1] = std::make_pair(0, -1);
  moves[2] = std::make_pair(1, 0);
  moves[3] = std::make_pair(0, 1);
}

bool valid_tile(int i, int j){
  if (i<0 || i>h-1 || j<0 || j>w-1 || !garden[i][j]) return false;
  return true;
}

int ij2tile(int i, int j){
  return i*w+j;
}

void testcase(){
  std::cin >> w >> h;

  int n_tiles = 0;
  garden.clear();
  garden.resize(h, std::vector<bool>(w, false));
  for (int i=0; i<h; i++){
    for (int j=0; j<w; j++){
      char tile; std::cin>>tile;
      if (tile == '.'){
        garden[i][j] = true;
        n_tiles++;
      }
    }
  }

  graph G(h*w);
  edge_adder adder(G);
  // Add special vertices source and sink
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);

  for (int i=0; i<h; i++){
    for (int j=0; j<w; j++){
      if (valid_tile(i, j)){
        if ((i+j)%2==1) {
          adder.add_edge(ij2tile(i,j), v_sink, 1);
          continue;
        }
        adder.add_edge(v_source, ij2tile(i,j), 1);
        for (std::pair<int, int> m : moves){
          int i2 = i+m.first;
          int j2 = j+m.second;
          if (valid_tile(i2, j2)){
            adder.add_edge(ij2tile(i,j), ij2tile(i2,j2), 1);
          }
        }
      }
    }
  }

  int flow = boost::push_relabel_max_flow(G, v_source, v_sink);
  std::string out = (2*flow==n_tiles)? "yes" : "no";
  std::cout << out << std::endl;

  return;
}


int main(){
  std::ios_base::sync_with_stdio(false);
  init_moves();
  int T; std::cin >> T;
  while(T--) testcase();
  return 0;
}