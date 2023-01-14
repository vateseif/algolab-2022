///5
#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <cmath>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::graph_traits<graph>::vertex_descriptor                       vertex_desc;

#define trace(x) std::cerr << #x << " = " << x << std::endl;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

int MAX_T = 5*std::pow(10,7); // max time for an agent to get to shelter

int n, m, a, s, c, d;
std::vector<int> agents; // position of agents and shelters
std::vector<std::vector<int>> dist_matrix;

bool match_agent_shelter(int t_max){
  // construct bipartite graph matrching agents to shelters
  graph G(a+c*s);
  
  // if distance <= t_max make connection
  for (int ag=0; ag<a; ag++){
    for (int sh=0; sh<s; sh++){
      if (dist_matrix[ag][sh]<=t_max){
        for (int i=0; i<c; i++){
          if (dist_matrix[ag][sh]<=t_max - i*d)
            boost::add_edge(ag, a+i*s+sh, G);
        }
      }
    }
  }

  std::vector<vertex_desc> mate_map(a+c*s);  // exterior property map

  boost::edmonds_maximum_cardinality_matching(G,
    boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));
  int matching_size = boost::matching_size(G,
    boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));

  if (matching_size==a) return true;

  return false;
}

void testcase(){
  
  std::cin >> n >> m >> a >> s >> c >> d;

  
  weighted_graph G(n+s); // poisitions plus the inside of a shelter
  weight_map weights = boost::get(boost::edge_weight, G);

  
  for (int i=0; i<m; i++){
    char w;
    int x, y, z;
    std::cin >> w >> x >> y >> z;
    edge_desc e;
    e = boost::add_edge(x, y, G).first; weights[e]=z;
    if (w=='L'){
      e = boost::add_edge(y, x, G).first; weights[e]=z;
    }
  }
  
  agents.clear(); agents.resize(a);
  for (int i=0; i<a; i++){
    int p;
    std::cin >> p;
    agents[i] = p;
  }

  
  //shelters.clear(); shelters.resize(s);
  for (int i=0; i<s; i++){
    int p;
    std::cin>>p;
    edge_desc e;
    e = boost::add_edge(p, n+i, G).first; weights[e]=d;
  }
  

  // run dijkstra for agents
  
  dist_matrix.clear();
  dist_matrix.resize(a, std::vector<int>(s));
  for (int j=0; j<a; j++){
    std::vector<int> dist_map(n+s);
    boost::dijkstra_shortest_paths(G, agents[j],
      boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));
  
    for (int i=0; i<s; i++){
      dist_matrix[j][i] = dist_map[n+i];  
    }
  }
  
  int l=0;
  int r = MAX_T;
  while(l<r){
    int mid = (l+r)/2;
    if (match_agent_shelter(mid)){
      r = mid;
    }else{
      l = mid + 1;
    }
  }

  std::cout << l << std::endl;
  

  return;
}



int main(){
  std::ios_base::sync_with_stdio(false);
  int T; std::cin>>T;
  while(T--) testcase();
  return 0;
}