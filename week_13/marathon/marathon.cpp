///2
#include <iostream>
#include <vector>
#include <algorithm>
// BGL include
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

// BGL flow include *NEW*
#include <boost/graph/push_relabel_max_flow.hpp>

#define trace(x) std::cerr << #x << " = " << x << std::endl;

// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

//typedef traits::vertex_descriptor vertex_desc;
//typedef traits::edge_descriptor edge_desc;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

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

struct Street{
  int a, b, c, d;
  Street(){};

  Street(int ai, int bi, int ci, int di){
    a = ai; b = bi; c = ci; d = di;
  }
};

int MAX_VAL = std::numeric_limits<int>::max();
std::vector<Street> streets;
std::vector<std::vector<int>> capacities, costs;
std::vector<std::vector<bool>> added_edge;


std::vector<vertex_desc> compute_path(int start, int finish, std::vector<vertex_desc>& pred_map){
  int cur = finish;
  std::vector<vertex_desc> path;
  path.push_back(cur);
  while (start != cur) {
    cur = pred_map[cur];
    path.push_back(cur);
  }
  //std::reverse(path.begin(), path.end());
  return path;
}

void testcase(){
  int n, m, s, f;
  std::cin >> n >> m >> s >> f;
  

  // compute shortest path from s to all nodes
  weighted_graph G_dijkstra(n);
  weight_map weights = boost::get(boost::edge_weight, G_dijkstra);

  streets.clear(); streets.resize(m);
  costs.clear(); costs.resize(n, std::vector<int>(n, MAX_VAL));
  capacities.clear(); capacities.resize(n, std::vector<int>(n, 0));
  for (int i=0; i<m; i++){
    int a, b, c, d;
    std::cin >> a >> b >> c >> d;
    edge_desc e;
    e = boost::add_edge(a, b, G_dijkstra).first; weights[e]=d;
    e = boost::add_edge(b, a, G_dijkstra).first; weights[e]=d;
    streets[i] = Street(a, b, c, d);
    if (b < a) std::swap(b, a);
    if (d < costs[a][b]){
      costs[a][b] = d;
      capacities[a][b] = c;
    }else if (d == costs[a][b]){
      capacities[a][b] += c;
    }
  }

  // shortest path from f to all nodes
  std::vector<int>         f_dist_map(n);
  std::vector<vertex_desc> f_pred_map(n);

  boost::dijkstra_shortest_paths(G_dijkstra, f,
    boost::distance_map(boost::make_iterator_property_map(
      f_dist_map.begin(), boost::get(boost::vertex_index, G_dijkstra)))
    .predecessor_map(boost::make_iterator_property_map(
      f_pred_map.begin(), boost::get(boost::vertex_index, G_dijkstra))));

  // shortest path from s to all nodes
  std::vector<int>         s_dist_map(n);
  std::vector<vertex_desc> s_pred_map(n);

  boost::dijkstra_shortest_paths(G_dijkstra, s,
    boost::distance_map(boost::make_iterator_property_map(
      s_dist_map.begin(), boost::get(boost::vertex_index, G_dijkstra)))
    .predecessor_map(boost::make_iterator_property_map(
      s_pred_map.begin(), boost::get(boost::vertex_index, G_dijkstra))));

  int min_cost = s_dist_map[f]; // == s_dist_map[f];

  // init graph for max flow
  graph G_flow(n);
  edge_adder adder(G_flow);

  added_edge.clear();
  added_edge.resize(n, std::vector<bool>(n, false));

  // compute cost of path from all neighbors of s to f
  int count = 0;
  for (int i=0; i<n; i++){
    if (i!=f && f_dist_map[i] + s_dist_map[i] == min_cost){
      std::vector<vertex_desc> fpath = compute_path(f, i, f_pred_map);
      std::vector<vertex_desc> spath = compute_path(s, i, s_pred_map);
      std::reverse(spath.begin(), spath.end());
      for (int u=1; u<fpath.size(); u++){
        spath.push_back(fpath[u]);
      }
      for (int u=0; u<spath.size()-1; u++){
        if (!added_edge[spath[u]][spath[u+1]]){
          int u1 = spath[u];
          int v1 = spath[u+1];
          if (v1 < u1) std::swap(v1, u1);
          adder.add_edge(spath[u], spath[u+1], capacities[u1][v1]);
          added_edge[spath[u]][spath[u+1]] = true;
        }
      }
    }
  }

  long flow = boost::push_relabel_max_flow(G_flow, s, f);
  std::cout << flow << "\n";

  return;

}


int main(){
  std::ios_base::sync_with_stdio(false);
  int T; std::cin >> T;
  //T=5;
  while(T--) testcase();
  return 0;
}