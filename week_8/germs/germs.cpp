#include <iostream>
#include <vector>
#include <tuple>
#include <cmath>
#include <map>
#include <algorithm>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Delaunay_triangulation_2.h>

#define trace(x) std::cerr << #x << " = " << x << std::endl;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef K::Point_2                    P;

std::vector<P> germs;
std::map<P, int> germs_idx;
std::vector<K::FT> tt_collision;


bool testcase(){
  int n; std::cin >> n;
  if (!n) return false;
  
  int l, b, r, t;
  std::cin >> l >> b >> r >> t;

  germs.clear();
  germs_idx.clear();
  germs.resize(n);
  tt_collision.clear();
  tt_collision.resize(n);
  for(int i=0; i<n; i++){
    int x, y;
    std::cin>>x>>y;
    P p(x, y);
    germs[i] = p;
    germs_idx[p] = i;
    int dx = std::min(std::abs(x-l), std::abs(r-x));
    int dy = std::min(std::abs(y-b), std::abs(t-y));
    int d = std::min(dx, dy);
    tt_collision[i] = d? d-0.5 : 0;
  }

  Triangulation trg;
  trg.insert(germs.begin(), germs.end());

  for (auto u = trg.finite_vertices_begin(); u!=trg.finite_vertices_end(); u++){
    // find all infinite edges incident to v
    Triangulation::Edge_circulator c = trg.incident_edges(u);
    K::FT min_dd_germ = std::numeric_limits<long>::max();
    if (c!=0){
      do {
      if (!trg.is_infinite(c)) {
        min_dd_germ = std::min(min_dd_germ, trg.segment(c).squared_length());
      }
      } while (++c != trg.incident_edges(u));
    }
    K::FT tt = min_dd_germ>=1? (std::sqrt(min_dd_germ)-1)/2 : 0;
    tt_collision[germs_idx[u->point()]] = std::min(tt_collision[germs_idx[u->point()]], tt);
  }

  std::sort(tt_collision.begin(), tt_collision.end());
  
  K::FT f = std::sqrt(tt_collision[0]);
  K::FT l1 = std::sqrt(tt_collision[n-1]);
  K::FT m = std::sqrt(tt_collision[std::floor(n/2.0)]);

  std::cout << std::ceil(f) << " ";
  std::cout << std::ceil(m) << " ";
  std::cout << std::ceil(l1) << std::endl;
  
  return true;
}


int main(){
  std::ios_base::sync_with_stdio(false);
  while (testcase());
  return 0;
}