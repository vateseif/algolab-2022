///4
#include <iostream>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <CGAL/Object.h>
#include <boost/pending/disjoint_sets.hpp>
#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>

// Epic kernel is enough, no constructions needed, provided the squared distance
// fits into a double (!)
typedef CGAL::Exact_predicates_exact_constructions_kernel K;
// we want to store an index with each vertex
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;

// As edges are not explicitly represented in the triangulation, we extract them
// from the triangulation to be able to sort and process them. We store the
// indices of the two endpoints, first the smaller, second the larger, and third
// the squared length of the edge. The i-th entry, for i=0,... of a tuple t can
// be accessed using std::get<i>(t).
typedef std::tuple<Index,Index,K::FT> Edge;
typedef std::vector<Edge> EdgeV;
typedef std::pair<K::Point_2,Index> IPoint;
  
  
std::vector<IPoint> points;
std::vector<std::pair<K::FT, int>> bounds;



void testcase(){
  Index n, h;
  K::FT s;
  std::cin >> n >> s >> h;
  
  // load tree location
  points.clear();
  points.reserve(n);
  for (Index i = 0; i < n; ++i) {
    int x, y;
    std::cin >> x >> y;
    points.emplace_back(K::Point_2(x, y), i);
  }

  Delaunay t;
  t.insert(points.begin(), points.end());

  // extract edges and sort by (squared) length
  // This step takes O(n log n) time (for the sorting).
  EdgeV edges;
  edges.reserve(3*n); // there can be no more in a planar graph
  for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
    Index i1 = e->first->vertex((e->second+1)%3)->info();
    Index i2 = e->first->vertex((e->second+2)%3)->info();
    // ensure smaller index comes first
    if (i1 > i2) std::swap(i1, i2);
    edges.emplace_back(i1, i2, t.segment(e).squared_length());
  }
  std::sort(edges.begin(), edges.end(),
      [](const Edge& e1, const Edge& e2) -> bool {
        return std::get<2>(e1) < std::get<2>(e2);
            });

  int a2 = 0;
  K::FT min_d = std::get<2>(edges[0]);
  for (EdgeV::const_iterator e = edges.begin(); e != edges.end(); ++e) {
    K::FT dd = std::get<2>(*e);
    if (dd > min_d) break;
    a2++;
  }


  int a3 = 0;
  K::FT min_r = std::numeric_limits<long>::max();
  for (auto it = t.finite_faces_begin(); it!=t.finite_faces_end(); it++){
    K::Point_2 center = t.dual(it);
    K::FT rr = CGAL::squared_distance(center, it->vertex(0)->point());
    if (rr<min_r){
      min_r = rr;
      a3 = 1;
    }else if (rr==min_r){
      a3++;
    }
  }

  int as = 0;
  bounds.clear();
  for (auto it=t.finite_edges_begin(); it!=t.finite_edges_end(); it++){
    K::FT it_dd = t.segment(it).squared_length();
    //if (it_dd > 4*s) continue;
    auto obj = t.dual(it);
    K::Point_2 u = it->first->vertex((it->second + 1) % 3)->point();
    K::Point_2 v = it->first->vertex((it->second + 2) % 3)->point();
    if (const K::Segment_2* segment = CGAL::object_cast<K::Segment_2>(&obj)){
      K::Point_2 su = segment->source();
      K::Point_2 sv = segment->target();
      K::FT dd1 = CGAL::squared_distance(v, su);
      K::FT dd2 = CGAL::squared_distance(v, sv);
      K::FT max_d = std::max(dd1, dd2);
      K::FT min_d = std::min(dd1, dd2);
      if (CGAL::do_intersect(t.segment(it), *segment)){
        bounds.emplace_back(it_dd/4, 0);
        bounds.emplace_back(max_d, 1);
        if (max_d >= s && it_dd <= 4*s){
          as++;
        }
      }else{
        bounds.emplace_back(min_d, 0);
        bounds.emplace_back(max_d, 1);
        if (max_d >= s && min_d <= s && it_dd <= 4*s){
          as++;
        }
      }
    }else if (const K::Ray_2* ray = CGAL::object_cast<K::Ray_2>(&obj)){
      K::FT dd = CGAL::squared_distance(u, ray->source());
      if (CGAL::do_intersect(t.segment(it), *ray)){
        bounds.emplace_back(it_dd/4, 0);
        if (it_dd <= 4*s) as++;
      }else {
        bounds.emplace_back(dd, 0);
        if (dd <= s && it_dd <= 4*s){
          as++;
        }
      }
    }
  }
  
  std::sort(bounds.begin(), bounds.end());
  
  int counter = 0;
  int max_counter = 0;
  for (unsigned int i=0; i<bounds.size(); i++){
    if (bounds[i].second==0){
      counter++;
      max_counter = std::max(counter, max_counter);
    }else{
      counter--;
    }
  }

  std::cout << a2 << " ";
  std::cout << a3 << " ";
  std::cout << as << " ";
  std::cout << max_counter << std::endl;
  
  return;
}



int main(){
  std::ios_base::sync_with_stdio(false);
  int T; std::cin>>T;
  while (T--) testcase();
  return 0;
}