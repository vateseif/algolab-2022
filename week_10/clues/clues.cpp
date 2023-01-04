///1
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef K::Point_2 P;
typedef Triangulation::Vertex_handle VertexHandle;
typedef std::map<VertexHandle, int> OpenedVertexMap;

Triangulation t;
std::vector<P> stations;
OpenedVertexMap vertex_map;

bool check_interference(int r){
  for (VertexHandle v_handle = t.all_vertices_begin(); v_handle!=t.all_vertices_end(); v_handle++){
    Triangulation::Edge_circulator c = t.incident_edges(v_handle);
    do {
    if (!t.is_infinite(c)) continue;
    VertexHandle neighbor = c->first->vertex((c->second + 2) % 3);
    int color_vertex = vertex_map[v_handle];
    int color_neighbor = vertex_map[neighbor];
    } while (++c != t.incident_edges(v_handle));
    
  }

  for (Triangulation::Face_iterator f=t.finite_faces_begin(); f!=t.finite_faces_end(); f++){
    for (int i=0; i<3; i++){
      int vi = t.cw(i);
      int vj = t.ccw(i);
      int i_vi = CGAL::squared_distance(f->vertex(i)->point(), f->vertex(vi)->point());
      int i_vj = CGAL::squared_distance(f->vertex(i)->point(), f->vertex(vj)->point());
      if (i_vi <= r && i_vj <= r) return true;
    }
  }
  return false;
}

void testcase(){
  int n, m, r;
  std::cin >> n >> m >> r;

  t.clear();
  vertex_map.clear();
  for (int i=0; i<n; i++){
    int x, y;
    std::cin >> x >> y;
    P p(x, y);
    t.insert(p);
  }
  
  t.insert(stations.begin(), stations.end());

  for (int i=0; i<m; i++){
    int xa, ya, xb, yb;
    std::cin >> xa >> ya >> xb >> yb;

    //std::string out = check_interference(r)? "n" : "y"; 
    //std::cout << out;
  }

  std::cout << std::endl;

  return;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int T; 
  std::cin >> T;
  while (T--) testcase();
  return 0;
}