#include <iostream>
#include <vector>
#include <unordered_map>

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/biconnected_components.hpp>


namespace boost{
    struct edge_component_t
    {
        enum
        {
            num = 555
        };
        typedef boost::edge_property_tag kind;
    } edge_component;
}


typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
            boost::no_property,
            boost::property<boost::edge_component_t, std::size_t>>        graph;

typedef boost::graph_traits<graph>::edge_iterator                         edge_it;
typedef boost::graph_traits<graph>::edge_descriptor                       edge_desc;
typedef boost::property_map<graph, boost::edge_component_t>::type         comp_map;




int main(){
  // init
  std::ios_base::sync_with_stdio(false);
  int T; std::cin >> T;
  
  for (int t=0; t<T; t++){
    int n; std::cin >> n;
    int m; std::cin >> m;
    
    graph g(n);
    for (int i=0; i<m; i++){
      int u; std::cin >> u;
      int v; std::cin >> v;
      boost::add_edge(u, v, g);
    }
    
    // property map, for each edge its component is stored
    comp_map edge_component = boost::get(boost::edge_component, g);
    // solve
    int n_comp = biconnected_components(g, edge_component);
    
    
    // for each components up to 2 of its edges are stored in a vector
    std::vector<std::vector<edge_desc>> comp_edges_map(n_comp);
    edge_it ei, ei_end;
    for (boost::tie(ei, ei_end) = edges(g); ei != ei_end; ++ei){
        int comp = edge_component[*ei];
        // store up to 2 edges for each component
        if (comp_edges_map[comp].size() <= 2){
            comp_edges_map[comp].push_back(*ei);
        }
    }
    
    int k = 0;
    // map of edges in lexicographic order
    std::unordered_map<int, std::vector<int>> lexi_edges;
    for (int c=0; c < n_comp; c++){
      std::vector<edge_desc> c_edges = comp_edges_map[c];
      if (c_edges.size()  == 1){
        k++;
        int u = boost::source(c_edges[0], g);
        int v = boost::target(c_edges[0], g);
        if (u < v){
          lexi_edges[u].push_back(v);
        }
        else{
          lexi_edges[v].push_back(u);
        }
      }
    }
    
    std::cout << k << std::endl;
    
    for (int j=0; j<n; j++){
      std::vector<int> neighbors = lexi_edges[j];
      if (neighbors.size() > 0) {
        std::sort(neighbors.begin(), neighbors.end());
        for (int v: neighbors){
          std::cout << j << " " << v << std::endl;
        }
      }
    }
    
  }
  
  
  return 0;
}