///2
#include <iostream>
#include <vector>
#include <algorithm>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

#define trace(x) std::cerr << #x << " = " << x << std::endl;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef K::Point_2 P;

struct Player {
  P position;
  long r;
  int id;
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
    p.id = i;
    players[i] = p;
  }
  // compute squared radius of lamp area
  long h; std::cin >> h;
  long lamp_r_squared = h * h;

  // load lamps positions
  lamps.clear(); lamps.resize(n); 
  for (int i=0; i<n; i++){
    int x, y;
    std::cin >> x >> y;
    P p(x, y);
    lamps[i] = p;
  }

  int l=0;
  int r=n-1;

  while (l<=r){
    int mid = (l+r)/2;
    Triangulation t;
    t.insert(lamps.begin()+l, lamps.begin()+mid+1);

    std::vector<Player> players_left;
    for(Player p : players){
      P nearest_lamp = t.nearest_vertex(p.position)->point();
      K::FT dd = CGAL::squared_distance(p.position, nearest_lamp);
      if (p.r*p.r + lamp_r_squared + 2*h*p.r <= dd){
        players_left.push_back(p);
      }
    }
    if (players_left.size()==0){
      r = mid;
      if (r==l) break;
    }else{
      players = players_left;
      l = mid + 1;
    }
  }
  
  for (Player p : players) 
    std::cout << p.id << " ";

  return;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int T; std::cin>>T;
  while(T--) {testcase(); std::cout << std::endl;};
  return 0;
}