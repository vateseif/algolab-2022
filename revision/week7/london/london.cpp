///5
#include <iostream>
#include <vector>
#include <string>
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

int N = 26;
std::vector<std::vector<int>> front, counter;

void testcase(){
  int h, w;
  std::cin >> h >> w;

  graph G(N*N + N);
  edge_adder adder(G);
  // Add special vertices source and sink
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);

  // load word and count letter repetions
  std::vector<int> note_counter(N,0);
  std::string note; std::cin >> note;
  for (char l : note){
    int l_id = l-'A';
    note_counter[l_id]++;
  }
  // connect words of note to sink
  for (unsigned int i=0; i<note_counter.size(); i++){
    if (note_counter[i]>0){
      adder.add_edge(N*N+i , v_sink, note_counter[i]);
    }
  } 

  // load front page
  front.clear(); front.resize(h, std::vector<int>(w, 0));
  for (int i=0; i<h; i++){
    std::string word;
    std::cin >> word;
    for (int j=0; j<w; j++){
      front[i][j] = word[j] - 'A';
    }
  }

  // load back page and connect word pair to respective node
  counter.clear(); counter.resize(N, std::vector<int>(N, 0));
  for (int i=0; i<h; i++){
    std::string word;
    std::cin >> word;
    for (int j=0; j<w; j++){
      int l_back = word[j] - 'A';
      int l_front = front[i][w-j-1];
      if (l_back<l_front) std::swap(l_back, l_front);
      counter[l_front][l_back]++;
    }
  }
  
  for (int i=0; i<N; i++){
    for (int j=0; j<N; j++){
      if (counter[i][j]>0){
        int u = i*N + j;
        adder.add_edge(v_source, u, counter[i][j]);
        adder.add_edge(u, N*N+i, counter[i][j]);
        adder.add_edge(u, N*N+j, counter[i][j]);
      }
    }
  }

  unsigned int flow = boost::push_relabel_max_flow(G, v_source, v_sink);
  std::string out = flow==note.size()? "Yes" : "No";
  std::cout << out << std::endl;
}




int main(){
  std::ios_base::sync_with_stdio(false);
  int T; std::cin >> T;
  while (T--) testcase(); 
  return 0;
}