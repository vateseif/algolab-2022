// STL includes
#include <iostream>
#include <vector>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/properties.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS> graph;
typedef boost::default_color_type                                         color;
const color black = boost::color_traits<color>::black(); // visited by BFS
const color white = boost::color_traits<color>::white(); // not visited by BFS

bool is_universal(const graph &G, int u) { // Is u universal in G?
  int n = boost::num_vertices(G);
  std::vector<color> vertex_color(n); // exterior property map

  boost::breadth_first_search(G, u,
    boost::color_map(boost::make_iterator_property_map(
      vertex_color.begin(), boost::get(boost::vertex_index, G))));

  // u is universal iff no vertex is white (i.e., all vertices are reachable from u)
  return (std::find(vertex_color.begin(), vertex_color.end(), white) == vertex_color.end());
}

void testcase() {
  int n, m;
  std::cin >> n >> m;
  graph G(n);

  for (int i = 0; i < m; ++i) {
    int u, v;
    std::cin >> u >> v;
    boost::add_edge(u, v, G);
  }

  for (int i = 0; i < n; ++i) {
    if (is_universal(G, i)) std::cout << i << " ";
  }
  std::cout << "\n";
}

int main()
{
    int T;
    std::cin >> T;

    while(T--) testcase();

    return 0;
}
