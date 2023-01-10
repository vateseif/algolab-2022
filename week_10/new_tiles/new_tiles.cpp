#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::graph_traits<graph>::vertex_descriptor                       vertex_desc;

int h, w;
std::vector<std::vector<int>> grid;
std::vector<std::pair<int, int>> moves;

void init_moves(){
  moves.clear();
  moves.resize(4);

  moves[0] = std::make_pair(-1, -1);
  moves[1] = std::make_pair(-1, 1);
  moves[2] = std::make_pair(1, -1);
  moves[3] = std::make_pair(1, 1);
}

int ij2n(int i, int j){
  return i*w + j;
}

bool valid_node(int i, int j){
  if (i<0 || i>=h || j<0 || j>=w || !grid[i][j]) return false;
  return true;
}

bool valid_connection(int i1, int j1, int i2, int j2){
  int min_i = std::min(i1, i2);
  int max_j = std::max(j1, j2);
  if (!grid[min_i][max_j] || !grid[min_i+1][max_j-1]) return false;
  return true;
}

void testcase(){
  
  std::cin >> h, w;

  grid.clear();
  grid.resize(h, std::vector<int>(w));
  for(int i=0; i<h; i++){
    for (int j=0; j<w; j++){
      std::cin >> grid[i][j];
    }
  }

  graph G(h*w);

  for (int i=0; i<h; i+=2){
    for (int j=0; j<w; j++){
      if ((i+j)%2==0 && valid_node(i, j)){
        for (std::pair<int, int> m : moves){
          int i2 = i + m.first;
          int j2 = j + m.second;
          if (valid_node(i2, j2) && valid_connection(i,j,i2,j2)){
            boost::add_edge(ij2n(i,j), ij2n(i2,j2), G);
          }
        }
      }
    }
  }


  std::vector<vertex_desc> mate_map(h*w);  // exterior property map

  boost::edmonds_maximum_cardinality_matching(G,
    boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));
  int matching_size = boost::matching_size(G,
    boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));

  std::cout << matching_size << std::endl;

  return;
}


int main(){
  std::ios_base::sync_with_stdio(false);
  int T; std::cin >> T;
  while (T--) testcase();  
  return 0;
}