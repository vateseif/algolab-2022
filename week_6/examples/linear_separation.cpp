// example: decide whether two point sets R and B can be strictly
// separated by a line

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose input type (input coefficients must fit)
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

int main()
{
  // create an LP with Ax <= b and no lower/upper bounds
  Program lp (CGAL::SMALLER, false, 0, false, 0); 
  const int a = 0;
  const int b = 1;
  const int c = 2;
  
  // number of red and blue points
  int m; std::cin >> m;
  int n; std::cin >> n;

  // read the red points
  for (int i = 0; i < m; ++i) {
    int x; std::cin >> x;
    int y; std::cin >> y;
    // set up constraint a x + b y + c <= 0
    lp.set_a(a, i, x); 
    lp.set_a(b, i, y);
    lp.set_a(c, i, 1);
    lp.set_b(i, -1);
  }
  // read the blue points
  for (int i = 0; i < n; ++i) {
    int x; std::cin >> x;
    int y; std::cin >> y;
    // set up constraint a x + b y + c >= 0
    lp.set_a(a, m+i, -x); 
    lp.set_a(b, m+i, -y);
    lp.set_a(c, m+i, -1);
    lp.set_b(m+i, -1);
  }

  // solve the program, using ET as the exact type
  Solution s = CGAL::solve_linear_program(lp, ET());
  assert(s.solves_linear_program(lp));
  
  // output solution
  std::cout << s; 
}
