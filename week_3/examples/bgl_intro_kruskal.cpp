// STL includes
#include <iostream>
#include <vector>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

void kruskal(const weighted_graph &G) {
  std::vector<edge_desc> mst;    // vector to store MST edges (not a property map!)

  boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));

  for (std::vector<edge_desc>::iterator it = mst.begin(); it != mst.end(); ++it) {
    std::cout << boost::source(*it, G) << " " << boost::target(*it, G) << "\n";
  }
}

int main()
{
  weighted_graph G(4);
  weight_map weights = boost::get(boost::edge_weight, G);

  edge_desc e;
  e = boost::add_edge(0, 1, G).first; weights[e]=0;
  e = boost::add_edge(1, 2, G).first; weights[e]=2;
  e = boost::add_edge(2, 3, G).first; weights[e]=1;
  e = boost::add_edge(3, 0, G).first; weights[e]=3;
  e = boost::add_edge(0, 2, G).first; weights[e]=3;

  kruskal(G);

  return 0;
}
