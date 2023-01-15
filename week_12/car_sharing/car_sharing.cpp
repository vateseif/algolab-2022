#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>


#define trace(x) std::cerr << #x << " = " << x << std::endl;

// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator
typedef traits::vertex_descriptor vertex_desc;


// Custom edge adder class
class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, long capacity, long cost) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G); // new!
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;   // new assign cost
    w_map[rev_e] = -cost;   // new negative cost
  }
};

struct Request{
  int s, t, d, a, p;
  Request(){};

  Request(int si, int ti, int di, int ai, int pi){
    s=si; t=ti; d=di; a=ai; p=pi;
  }
};

int MAX_L = 100; // max cars per station
int MAX_S = MAX_L * 10; // max number of cars over all stations
int MAX_T = 100000; // max time
int MAX_P = 100; // max profit per request



std::vector<int> L; // store l_i number of initial cars to station i
std::vector<std::set<int>> departures; // store unique departure times per station
std::vector<Request> requests; // store all the requests
std::vector<std::vector<int>> M; // given station i and request j returns the corresponding graph node


void testcase(){
  int n, s;
  std::cin >> n >> s;

  // store initial cars
  L.clear();
  L.resize(s);
  for (int i=0; i<s; i++){
    int l; std::cin >> l;
    L[i] = l;
  }

  // init requests
  departures.clear();
  departures.resize(s, {0, MAX_T});
  
  // store requests and departures
  requests.clear();
  requests.resize(n);
  for (int i=0; i<n; i++){
    int si, ti, di, ai, pi;
    std::cin >> si >> ti >> di >> ai >> pi;
    requests[i] = Request(si-1, ti-1, di, ai, pi);
    departures[si-1].insert(di);
  }
  
  
  // count all nodes
  int count = 0;
  M.clear(); M.resize(s);
  for (int i=0; i<s; i++){
    for (int j=0; j<departures[i].size(); j++){
      M[i].push_back(count);
      count++;
    }
  } 
  
  
  // init graph with total nodes + source + sink
  // Create graph, edge adder class and propery maps
  graph G(count+2);
  edge_adder adder(G);
  auto c_map = boost::get(boost::edge_capacity, G);
  auto rc_map = boost::get(boost::edge_residual_capacity, G); 
  const int v_source = count;  
  const int v_sink = count + 1;

  // connect first and last node to source and sink
  for (int i=0; i<s; i++){
    adder.add_edge(v_source, M[i][0], L[i], 0);
    adder.add_edge(M[i].back(), v_sink, MAX_S, 0);
    int tt = 0;
    int t_last = 0;
    for (auto& dep_time : departures[i]){
      if (tt!=0){
        adder.add_edge(M[i][tt-1], M[i][tt], MAX_S, (dep_time-t_last)*MAX_P);
      }
      tt++;
      t_last = dep_time;
    }
  }


  // connect nodes per request
  for (int i=0; i<n; i++){
    Request r = requests[i];
    // earliest feasible departure after arrival
    auto earliest_departure = departures[r.t].lower_bound(r.a);
    int u = M[r.s][std::distance(departures[r.s].begin(), departures[r.s].find(r.d))];
    int v = M[r.t][std::distance(departures[r.t].begin(), earliest_departure)];
    adder.add_edge(u, v, 1, (*earliest_departure - r.d)*MAX_P - r.p);
  }


  boost::successive_shortest_path_nonnegative_weights(G, v_source, v_sink);
  int cost2 = boost::find_flow_cost(G);
  
  int flow = 0;
  out_edge_it e, eend;
  for(boost::tie(e, eend) = boost::out_edges(boost::vertex(v_source,G), G); e != eend; ++e) {
    flow += c_map[*e] - rc_map[*e];     
  }

  std::cout << MAX_T*flow*MAX_P - cost2 << std::endl;


  return;
}



int main(){
  std::ios_base::sync_with_stdio(false);
  int T; std::cin>>T;
  //T=1;
  while(T--) testcase();
  return 0;
}