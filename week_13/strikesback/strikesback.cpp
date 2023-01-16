///2
#include <iostream>
#include <vector>
#include <algorithm>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
// example: how to solve a simple explicit LP
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>

#define trace(x) std::cerr << #x << " = " << x << std::endl;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef K::Point_2 P;

// choose input type (input coefficients must fit)
typedef CGAL::Gmpq IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpq ET;
// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

int a, s, b, e;
std::vector<int> density;
std::vector<long> radii;
std::vector<P> asteroids, shooting, hunters;


char solve_lp(){
  // create an LP with Ax <= b, lower bound 0 and no upper bounds
  Program lp (CGAL::SMALLER, true, 0, false, 0); 

  // constraint that energy reaching asteroids is more than their density
  for (int j=0; j<a; j++){
    //long sum_dd = 0; // used to avoid fractions in constraint
    // loop through shootings and check that distance is smaller than max radius
    for (int i=0; i<s; i++){
      long dd = CGAL::squared_distance(shooting[i], asteroids[j]);
      if (dd < radii[i]){
        lp.set_a(i, j,  CGAL::Gmpq(-1, std::max((long)1, dd)));
        //sum_dd += std::max((long)1, dd);
      }else{
        lp.set_a(i, j,  0);
      }
    }
    int d_j = density[j]; // density of asteroid j
    //lp.set_b(j, -d_j*sum_dd);
    lp.set_b(j, -d_j);
  }

  // constraint that sum of energy is smaller than limit
  for (int i=0; i<s; i++){
    lp.set_a(i, a,  1);
  }
  lp.set_b(a, e);

  // solve the program, using ET as the exact type
  Solution s = CGAL::solve_linear_program(lp, ET());

  char out = s.is_optimal()? 'y' : 'n';
  
  return out;
}



void testcase(){
  std::cin >> a >> s >> b >> e;

  asteroids.clear();
  asteroids.resize(a);
  density.clear();
  density.resize(a);
  for (int i=0; i<a; i++){
    int x, y, d;
    std::cin >> x >> y >> d;
    asteroids[i] = P(x, y);
    density[i] = d;
  }

  shooting.clear();
  shooting.resize(s);
  for (int i=0; i<s; i++){
    int x, y;
    std::cin >> x >> y;
    shooting[i] = P(x, y);
  }


  hunters.clear();
  for (int i=0; i<b; i++){
    int x, y;
    std::cin >> x >> y;
    P p(x, y);
    hunters.push_back(p);
  }

  Triangulation th;
  th.insert(hunters.begin(), hunters.end());

  // compute maximum radius for each shooting
  radii.clear();
  radii.resize(s, std::numeric_limits<long>::max());
  if (b > 0){
    for (int i=0; i<s; i++){
      P sh = shooting[i];
      P nearest = th.nearest_vertex(sh)->point();
      long dd = CGAL::squared_distance(nearest, sh);
      radii[i] = dd;
    } 
  }
  

  std::cout << solve_lp() << std::endl;

  return;
}



int main(){
  std::ios_base::sync_with_stdio(false);
  int T; std::cin >> T;
  while(T--) testcase();  
  return 0;
}