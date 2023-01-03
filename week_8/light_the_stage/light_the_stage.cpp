///2
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

#define trace(x) std::cerr << #x << " = " << x << std::endl;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef K::Point_2 P;
typedef std::map<P, int> lampIndexMap;

struct Player {
  P position;
  long r;
  // This constructor is convenient for a concise initialisation. It can also
  // be omitted and the member variables be set manually.
  //Player(P pos, long radius) : position(pos), r(radius) {}
};

// create triangulation of lamp positions
Triangulation t;
std::vector<P> lamps;
std::vector<int> winners;
std::vector<Player> players;

void testcase(){
  int m, n;
  std::cin >> m >> n;

  // store players as Player
  players.clear(); players.resize(m);
  for (int i=0; i<m; i++){
    int x, y;
    long r;
    std:: cin >> x >> y >> r;
    Player p;
    p.r = r;
    p.position = P(x, y);
    players[i] = p;
  }
  // compute squared radius of lamp area
  long h; std::cin >> h;
  long lamp_r_squared = h * h;

  // load lamps positions
  lamps.clear(); lamps.resize(n); 
  lampIndexMap lamps_idx; 
  for (int i=0; i<n; i++){
    int x, y;
    std::cin >> x >> y;
    P p(x, y);
    lamps_idx[p] = i;
    lamps[i] = p;
  }
  
  t.clear();
  t.insert(lamps.begin(), lamps.end());
  
  bool winnerNotFound = true;
  while (winnerNotFound){
    int i = 0;
    int last_lamp_idx = -1;
    for (Player p : players){
      P nearest_lamp = t.nearest_vertex(p.position)->point();
      long squared_d = CGAL::squared_distance(nearest_lamp, p.position);
      if (p.r*p.r + lamp_r_squared + 2*h*p.r <= squared_d){
        std::cout << i << " ";
        last_lamp_idx = n;
        winnerNotFound = false;
      }else{
        last_lamp_idx = std::max(last_lamp_idx, lamps_idx[nearest_lamp]);
      }
      i++;
    }
    for (Triangulation::Vertex_handle v_handle=t.all_vertices_begin(); v_handle!=t.all_vertices_end(); ++v_handle){
      P v_point = v_handle->point();
      if (lamps_idx[v_point] >= last_lamp_idx) t.remove(v_handle);
    }
  }

  return;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int T; std::cin>>T;
  while(T--) {testcase(); std::cout << std::endl;};
  return 0;
}