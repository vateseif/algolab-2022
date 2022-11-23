#include <iostream>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

#define trace(x) std::cerr << #x << " = " << x << std::endl;

typedef int IT;
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

int n, d;

double floorGmpz(CGAL::Quotient<ET> r){
  double out = std::floor(CGAL::to_double(r));
  while (out > r) out -=1;
  while (out+1 <= r) out+=1;
  return out;
}

void objective(Program& lp){
  const int r = d;
  lp.set_c(r, -1);
}

void constraints(Program& lp){
  const int r = d; // radius
  // loop over n sides of triangle
  for (int i=0; i<n; i++){
    // A matrix
    int norm_squared = 0;
    for (int j=0; j<d; j++){
      int aij; std::cin >> aij;
      lp.set_a(j, i, aij);
      norm_squared += aij*aij;
    }
    int norm = std::sqrt(norm_squared);
    lp.set_a(r, i, norm);
    // b vector
    int bi; std::cin >> bi;
    lp.set_b(i, bi);
    // lower bound
    lp.set_l(r, true, 0);
  }
}

bool testcase(){
  std::cin >> n;
  if (n==0) return false;
  std::cin >> d;
  // construct lp
  Program lp (CGAL::SMALLER, false, 0, false, 0); 
  constraints(lp);
  objective(lp);
  //solve
  Solution s = CGAL::solve_linear_program(lp, ET());
  // output
  if (s.is_infeasible()){
    std::cout << "none" << std::endl;
  } else if (s.is_unbounded()){
    std::cout << "inf" << std::endl;
  } else {
    std::cout << floorGmpz(-s.objective_value()) << std::endl;
  }
  return true;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  while(testcase());
  return 0;
}