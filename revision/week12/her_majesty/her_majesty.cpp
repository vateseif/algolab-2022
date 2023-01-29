///2
#include <iostream>
#include <vector>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

#define trace(x) std::cerr << #x << " = " << x << std::endl;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

int n, m, a, s, c, d;
std::vector<int> agents, shelters;
std::vector<std::vector<int>> dist_map;

bool exist_matching(int t){
  // construct graph
  graph G(a+c*s);

  // add edge if distance < t-d
  for (int i=0; i<a; i++){
    for (int j=0; j<s; j++){
      for (int k=1; k<=c; k++){
        if (dist_map[i][shelters[j]] <= t-k*d){
          boost::add_edge(i, a+(k-1)*s+j, G);
        }
      }
    }
  }
   
  // check if all agents math with a shelter
  std::vector<vertex_desc> mate_map(a+c*s);  // exterior property map

  boost::edmonds_maximum_cardinality_matching(G,
    boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));
  int matching_size = boost::matching_size(G,
    boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));

  return matching_size==a;
}


void testcase(){
  std::cin >> n >> m >> a >> s >> c >> d;

  // construct graph
  weighted_graph G(n);
  edge_desc e;
  weight_map weights = boost::get(boost::edge_weight, G);


  // add edges
  for (int i=0; i<m; i++){
    char w;
    int x, y, z;
    std::cin >> w >> x >> y >> z;
    e = boost::add_edge(x, y, G).first; weights[e]=z;
    if (w=='L'){
      e = boost::add_edge(y, x, G).first; weights[e]=z;
    }
  }

  // load agents location
  agents.clear(); agents.resize(a);
  for (int i=0; i<a; i++) std::cin >> agents[i];

  // load shelter locations
  shelters.clear(); shelters.resize(s);
  for (int i=0; i<s; i++) std::cin >> shelters[i];


  // compute shortest path for each agent
  dist_map.clear(); dist_map.resize(a, std::vector<int>(n));
  for (int i=0; i<a; i++){
    boost::dijkstra_shortest_paths(G, agents[i],
    boost::distance_map(boost::make_iterator_property_map(
      dist_map[i].begin(), boost::get(boost::vertex_index, G))));
  }

  int l = 0;
  int r = std::numeric_limits<int>::max();
  //trace(max_t);
  while (l<r){
    int mid = (l+r)/2;
    if (exist_matching(mid)){
      r = mid;
    }else{
      l = mid+1;
    }
  }

  std::cout << l << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int T; std::cin >> T;
  while (T--) testcase();
  return 0;
}