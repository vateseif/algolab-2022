#include <iostream>
#include <vector>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

#define trace(x) std::cerr << #x << " = " << x << std::endl;

// choose input type (input coefficients must fit)
typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

double roundGmpz(int& p, CGAL::Quotient<ET> optimal_value){
  trace(CGAL::to_double(optimal_value));
  double v = std::ceil(CGAL::to_double(optimal_value));
  while (v < optimal_value) v += 1;
  while (v-1 >= optimal_value) v -= 1;
  return p==1? -v : v; // the max problem (lp1) has opposite sign
}

Program lp1(int& a, int& b){
  // create an LP with Ax <= b, lower bound 0 and no upper bounds
  Program lp (CGAL::SMALLER, true, 0, false, 0); 
  // variable alias
  const int x = 0;
  const int y = 1;
  // constraints Ax <= b
  lp.set_a(x, 0, 1); lp.set_a(y, 0, 1); lp.set_b(0, 4);
  lp.set_a(x, 1, 4); lp.set_a(y, 1, 2); lp.set_b(1, a*b);
  lp.set_a(x, 2, -1); lp.set_a(y, 2, 1); lp.set_b(2, 1);
  // objective function
  lp.set_c(x, a); lp.set_c(y, -b); 
  
  return lp;
}

Program lp2(int& a, int& b){
  // create an LP with Ax <= b, lower bound 0 and no upper bounds
  Program lp (CGAL::SMALLER, false, 0, true, 0); 
  // variable alias
  const int x = 0;
  const int y = 1;
  const int z = 2;
  // constraints Ax <= b
  lp.set_a(x, 0, -1); lp.set_a(y, 0, -1); lp.set_a(z, 0, 0); lp.set_b(0, 4);
  lp.set_a(x, 1, -4); lp.set_a(y, 1, -2); lp.set_a(z, 1, -1); lp.set_b(1, a*b);
  lp.set_a(x, 2, 1); lp.set_a(y, 2, -1); lp.set_a(z, 2, 0); lp.set_b(2, 1);
  // objective function
  lp.set_c(x, a); lp.set_c(y, b); lp.set_c(z, 1); 

  return lp;
}

bool testcase(){
  int p, a, b;
  std::cin >> p;
  if (p==0) return false;
  std::cin >> a >> b;
  
  Program lp = p == 1? lp1(a,b) : lp2(a,b);
  Solution s = CGAL::solve_linear_program(lp, ET());
  
  if (s.is_infeasible()){
    std::cout << "no" << std::endl;
  } else if (s.is_unbounded()){
    std::cout << "unbounded" << std::endl;
  } else {
    CGAL::Quotient<ET> opt = s.objective_value(); 
    std::cout << roundGmpz(p, opt) << std::endl;
  }

  return true;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  while(testcase());
  return 0;
}