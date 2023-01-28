///2
#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
// example: how to solve a simple explicit LP
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

#define trace(x) std::cerr << #x << " = " << x << std::endl;

// choose input type (input coefficients must fit)
typedef long IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

int n, m;
long h, w;
std::vector<std::pair<long, long>> fixed, variable;


double ceil_to_double(const CGAL::Quotient<ET> x){
 double a = std::floor(CGAL::to_double(x));
 while (a < x) a += 1;
 while (a-1 >= x) a -= 1;
 return a;
}



void solve_lp(){
  // create an LP with Ax <= b, lower bound 1 and no upper bounds
  Program lp (CGAL::SMALLER, true, 1, false, 0); 

  // objective cost: maximize perimeter
  for (int i=0; i<n; i++){
    lp.set_c(i, -2*(h+w));
  }

  // enforce variable posters do not overlap with each other
  int nc = 0; // counter of constraints
  for (int i=0; i<n-1; i++){
    for (int j=i+1; j<n; j++){
      long dx = std::abs(variable[i].first - variable[j].first);
      long dy = std::abs(variable[i].second - variable[j].second);
      if (dx*h >= dy*w){
        lp.set_a(i, nc,  w);
        lp.set_a(j, nc,  w);
        lp.set_b(nc, 2*dx);
      }else{
        lp.set_a(i, nc,  h);
        lp.set_a(j, nc,  h);
        lp.set_b(nc, 2*dy);
      }
      nc++;
    }
  }
  
  
  // enforce variable poster doesn't overlap with closest fixed poster
  for (int i=0; i<n; i++){
    long min_dx = 2*std::pow(2,32);
    long min_dy = 2*std::pow(2,32);
    for (int j=0; j<m; j++){
      long dx = std::abs(variable[i].first - fixed[j].first);
      long dy = std::abs(variable[i].second - fixed[j].second);
      if (2*dy*w-h*h*w > 2*dx*h-h*w*w){
        min_dy = std::min(min_dy, dy);
      }else{
        min_dx = std::min(min_dx, dx);
      }
    }
    if (min_dy*w <= min_dx*h){
      lp.set_a(i, nc, h);
      lp.set_b(nc, 2*min_dy-h);
    }else{
      lp.set_a(i, nc, w);
      lp.set_b(nc, 2*min_dx-w);
    }
    nc++;
  }
  
  // solve the program, using ET as the exact type
  Solution s = CGAL::solve_linear_program(lp, ET());
  std::cout << ceil_to_double(-s.objective_value()) << std::endl;
}


void testcase(){
  std::cin >> n >> m >> h >> w;

  variable.clear();
  variable.resize(n);
  for (int i=0; i<n; i++){
    long x,y;
    std::cin >> x >> y;
    variable[i] = std::make_pair(x, y);
  }

  fixed.clear(); fixed.resize(m);
  for (int i=0; i<m; i++){
    long x, y;
    std::cin >> x >> y;
    fixed[i] = std::make_pair(x, y);
  }

  solve_lp();
}



int main(){
  std::ios_base::sync_with_stdio(false);
  std::cout << std::setprecision(17);
  int T; std::cin >> T;
  while (T--) testcase();
  return 0;
}