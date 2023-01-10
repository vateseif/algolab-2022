///2
#include <iostream>
#include <vector>
#include <stack>
#include <map>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>


#define trace(x) std::cerr << #x << " = " << x << std::endl;


typedef std::pair<int, bool> nodeInfo; // component and color
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_vertex_base_with_info_2<nodeInfo, K> Vb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;

typedef K::Point_2 P;
typedef Triangulation::Vertex_handle VH;


int n, m;
std::vector<P> stations;

bool trg_interference(Triangulation& t, long& r){
  for (auto e=t.finite_edges_begin(); e!=t.finite_edges_end(); e++){
    if (t.segment(e).squared_length() <= r*r) return true;
  }
  return false;
}

bool interference(Triangulation& t, long& r){

  // set default nodeInfo for each node
  for (auto u = t.finite_vertices_begin(); u!=t.finite_vertices_end(); u++){
    u->info() = std::make_pair(0, false);
  }

  // init triangulation for the 2 different coloring
  Triangulation t1, t2;
  // loop through connected components via dfs
  // a connection occurs if distance is smaller than r
  int n_components = 0;
  for (auto u_src = t.finite_vertices_begin(); u_src!=t.finite_vertices_end(); u_src++){
    if (u_src->info().first==0){  
      u_src->info().first = ++n_components;
      std::stack<VH> Q;
      Q.push(u_src);
      while (!Q.empty()){
        VH u = Q.top(); Q.pop();
        // find all infinite edges incident to v
        Triangulation::Edge_circulator c = t.incident_edges(u);
        do {
        if (!t.is_infinite(c)) {
          VH v =  c->first->vertex((c->second + 1) % 3);
          long dd = CGAL::squared_distance(u->point(), v->point());
          if (dd <=r*r){
            if (v->info().first==0){
              v->info() = std::make_pair(u->info().first, !u->info().second);
              Q.push(v);
            }
          }
        }
        } while (++c != t.incident_edges(u));
      }
    }
    if (u_src->info().second) t1.insert(u_src->point());
    else t2.insert(u_src->point());
  }

  return (trg_interference(t1, r) || trg_interference(t2, r));
}

char radios_can_communicate(Triangulation& t, P& ai, P& bi, long& r){
  VH nearest_ai = t.nearest_vertex(ai);
  VH nearest_bi = t.nearest_vertex(bi);
  if (CGAL::squared_distance(ai, nearest_ai->point()) > r*r)
    return 'n';
  if (CGAL::squared_distance(bi, nearest_bi->point()) > r*r)
    return 'n';
  if (nearest_ai->info().first != nearest_bi->info().first)
    return 'n';

  return 'y';
}


void testcase(){
  long r;
  std::cin >> n >> m >> r;

  stations.clear();
  stations.resize(n);
  for (int i=0; i<n; i++){
    int x, y;
    std::cin >> x >> y;
    stations[i] = P(x, y);
  }

  Triangulation t;
  t.insert(stations.begin(), stations.end());
  
  bool succes = !interference(t, r);

  for(int i=0; i<m; i++){
    int x1, y1, x2, y2;
    std::cin >> x1 >> y1 >> x2 >> y2;
    if (!succes) {
      std::cout << 'n';
      continue;
    }
    P ai(x1, y1), bi(x2, y2);
    if (CGAL::squared_distance(ai, bi) <= r*r){
      std::cout << 'y';
    }else{
      std::cout << radios_can_communicate(t, ai, bi, r);
    }
  }

  std::cout << std::endl;

  return;
}


int main(){
  std::ios_base::sync_with_stdio(false);
  int T; std::cin >> T;
  while(T--)testcase();
  return 0;
}
