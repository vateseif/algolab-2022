#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

int main()
{
  K::Point_2 p(2,1), q(1,0), r(-1,-1);
  K::Line_2 l(p,q);
  K::FT d = CGAL::squared_distance(r,l);
  std::cout << d << "\n";
}
