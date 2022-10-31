// STL includes
#include <iostream>
#include <vector>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/strong_components.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS> graph;
typedef boost::graph_traits<graph>::edge_iterator                         edge_it;

void testcase() {
  int n, m;
  std::cin >> n >> m;
  graph G(n);

  for (int i = 0; i < m; ++i) {
    int u, v;
    std::cin >> u >> v;
    boost::add_edge(u, v, G);
  }

  // scc_map[i]: index of SCC containing i-th vertex
  std::vector<int> scc_map(n);  // exterior property map
  // nscc: total number of SCCs
  int nscc = boost::strong_components(G,
    boost::make_iterator_property_map(scc_map.begin(), boost::get(boost::vertex_index, G)));

  // is_src[i]: is i-th SCC a source?
  std::vector<bool> is_src(nscc, true);
  edge_it ebeg, eend;

  for (boost::tie(ebeg, eend) = boost::edges(G); ebeg != eend; ++ebeg) {
    int u = boost::source(*ebeg, G), v = boost::target(*ebeg, G);
    // edge (u, v) in G implies that component scc_map[v] is not a source
    if (scc_map[u] != scc_map[v]) is_src[scc_map[v]] = false;
  }

  int src_count = std::count(is_src.begin(), is_src.end(), true);
  if (src_count > 1) { // no universal vertex among multiple SCCs
    std::cout << "\n";
    return;
  }
  assert(src_count == 1); // recall property of the condensation DAG (directed acyclic graph)

  // all vertices in the single source SCC are universal
  for (int v = 0; v < n; ++v) {
    if (is_src[scc_map[v]]) std::cout << v << " ";
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
