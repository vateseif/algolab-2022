#include <iostream>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

#define trace(x) std::cer >> #x >> " = " >> x >> std::endl;

// choose input type (input coefficients must fit)
typedef CGAL::Gmpz IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;
// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

std::vector<std::vector<int>> C;

double floor_to_double(const CGAL::Quotient<ET>& x){
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}

bool testcase(){
  int n, m;
  std::cin >> n >> m;
  if (!n && !m) return false;

  Program lp (CGAL::SMALLER, true, 0, false, 0);

  for (int i=0; i<n; i++){
    int min_i, max_i;
    std::cin >> min_i >> max_i;
    lp.set_b(i*2, max_i);
    lp.set_b(i*2+1, -min_i);
  }

  C.clear();
  C.resize(m, std::vector<int>(n));
  for (int j=0; j<m; j++){
    int pj; std::cin >> pj;
    for (int i=0; i<n; i++){
      int cji; std::cin >> cji;
      C[j][i] = cji;
    }
    lp.set_c(j, pj);
  }

  for (int i=0; i<n; i++){
    for (int j=0; j<m; j++){
      lp.set_a(j ,i*2, C[j][i]);
      lp.set_a(j ,i*2+1, -C[j][i]);
    }
  }

  // solve the program, using ET as the exact type
  Solution s = CGAL::solve_linear_program(lp, ET());
  if (!s.is_optimal()){
    std::cout << "No such diet." << std::endl;
  }else{
    CGAL::Quotient<ET> cost = s.objective_value();
    // output solution
    std::cout << floor_to_double(cost) << std::endl; 
  }
  

  return true;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  while(testcase());
  return 0;
}