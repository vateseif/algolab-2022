#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
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
int voc_size;
std::unordered_map<char, int> letter_idx; 
std::unordered_map<int, int>  note_letters_count;
std::string vocabulary = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

void init_vocabulary(){
  voc_size = vocabulary.size();
  for (int i=0;i<voc_size;i++){
    letter_idx[vocabulary[i]] = i;
  }
}


void testcase(){
  int h, w, n;
  std::string note;
  std::cin >> h >> w >> note;
  n = note.size();
  
  note_letters_count.clear();
  for (int i=0; i<n; i++){
    int i_idx = letter_idx[note[i]];
    note_letters_count[i_idx]++;
  }
  // construct graph
  graph G(voc_size+h*w);
  edge_adder adder(G);
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);

  long sum_counts = 0;
  for (int j=0; j<voc_size; j++){
    int letter_count = note_letters_count[j];
    adder.add_edge(j, v_sink, letter_count);
    sum_counts += letter_count;
  }
  
  for (int i=0;i<h;i++){
    std::string text;
    std::cin >> text;
    for (int j=0; j<w; j++){
      int j_letter = letter_idx[text[j]];
      adder.add_edge(voc_size+i*w+j, j_letter, 1);
      adder.add_edge(v_source, voc_size+i*w+j, 1);
    }
  }

  for (int i=0;i<h;i++){
    std::string text;
    std::cin >> text;
    for (int j=0; j<w; j++){
      int j_letter = letter_idx[text[w-1-j]];
      adder.add_edge(voc_size+i*w+j, j_letter, 1);
    }
  }
  long max_flow = boost::push_relabel_max_flow(G, v_source, v_sink);
  std::string out = max_flow==sum_counts ? "Yes" : "No";
  std::cout << out << std::endl;

  return;
}



int main(){
  std::ios_base::sync_with_stdio(false);
  int T; std::cin >> T;
  init_vocabulary();
  while(T--) testcase();
  return 0;
}