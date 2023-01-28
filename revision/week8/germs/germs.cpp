#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <cmath>
#include <unordered_map>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Delaunay_triangulation_2.h>

#define trace(x) std::cerr << #x << " = " << x << std::endl;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt EK;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef K::Point_2 P;


long MAX_VAL = std::numeric_limits<long>::max();

std::vector<P> bacteria;
std::vector<long> death_dist;
std::unordered_map<P, int> bacteria_idx;

double dd_to_time(const EK::FT& dd){
  EK::FT d = CGAL::sqrt(dd/4);
  EK::FT tt = (2*d-1)/2;
  EK::FT ti = CGAL::sqrt(tt);
  double a = std::floor(CGAL::to_double(ti));
  while (a < ti) a += 1;
  while (a-1 >= ti) a -= 1;
  return a;
}


bool testcase(){
  int n; std::cin >> n;
  if (!n) return false;


  long l, b, r, t;
  std::cin >> l >> b >> r >> t;


  // store bacteria and compute min distance to edges
  bacteria_idx.clear();
  bacteria.clear(); bacteria.resize(n);
  death_dist.clear(); death_dist.resize(n, MAX_VAL);
  for (int i=0; i<n; i++){
    long x, y;
    std::cin >> x >> y;
    P p(x,y);
    bacteria[i] = p;
    bacteria_idx[p] = i;
    long min_dx = std::min(x-l, r-x);
    long min_dy = std::min(y-b, t-y);
    long min_d = std::min(min_dx, min_dy);
    death_dist[i] = 4*min_d*min_d;
  }

  Triangulation tr;
  tr.insert(bacteria.begin(), bacteria.end());


  for (auto u = tr.finite_vertices_begin(); u!=tr.finite_vertices_end(); u++){
    // find all infinite edges incident to v
    Triangulation::Edge_circulator c = tr.incident_edges(u);
    if (c!=0){
      do {
      if (!tr.is_infinite(c)) {
        long dd = tr.segment(c).squared_length();
        death_dist[bacteria_idx[u->point()]] = std::min(dd, death_dist[bacteria_idx[u->point()]]);
      }
      } while (++c != tr.incident_edges(u));
    }
  }

  std::sort(death_dist.begin(), death_dist.end());

  std::cout << dd_to_time(death_dist[0]) << " ";
  std::cout << dd_to_time(death_dist[n/2]) << " ";
  std::cout << dd_to_time(death_dist.back()) << " "; 
  std::cout << std::endl;
  return true;
}


int main(){
  std::ios_base::sync_with_stdio(false);
  while (testcase());
  return 0;
}