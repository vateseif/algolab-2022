///1
#include <iostream>
#include <vector>
#include <string>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>
// debug
#define trace(x) std::cerr << #x << " = " << x << std::endl;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::graph_traits<graph>::vertex_descriptor                       vertex_desc;

std::vector<std::vector<bool>> garden;

int ij2node(int& w, int i, int j){
  return i*w+j;
}

void testcase(){
  int w, h;
  std::cin >> w >> h;
  // init graph
  graph G(h*w);
  
  garden.clear();
  garden.resize(h, std::vector<bool>(w, false));
  int n_tiles = 0;
  for (int i=0; i<h; i++){
    for (int j=0; j<w; j++){
      char tile; std::cin >> tile;
      if (tile=='.'){
        garden[i][j] = true;
        n_tiles++;
      }
    }
  }

  for (int i=0; i<h; i++){
    for (int j=0; j<w; j++){
      if (j<w-1){
        if (garden[i][j] && garden[i][j+1]){
          boost::add_edge(ij2node(w, i, j), ij2node(w, i, j+1), G);
        }
      }
      if (i<h-1){
        if (garden[i][j] && garden[i+1][j]){
          boost::add_edge(ij2node(w, i, j), ij2node(w, i+1, j), G);
        }
      }
    }
  }

  std::vector<vertex_desc> mate_map(h*w);  // exterior property map

  boost::edmonds_maximum_cardinality_matching(G,
    boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));
  int matching_size = boost::matching_size(G,
    boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));

  std::string out = 2*matching_size==n_tiles? "yes" : "no";
  std::cout << out << std::endl;

  return;
}


int main(){
  std::ios_base::sync_with_stdio(false);
  int T; std::cin >> T;
  while(T--) testcase();
  return 0;
}