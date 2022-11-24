///1
#include <iostream>
#include <vector>
#include <cstdlib>
#include <climits>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

#define trace(x) std::cerr << #x << " = " << x << std::endl;

typedef long IT;
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

int n, m, h, w;
std::vector<std::pair<int, int>> po, pn;  // old and new posters (x, y)

double roundUp(CGAL::Quotient<ET> r){
  double out = std::ceil(CGAL::to_double(r));
  while (out < r) out +=1;
  while (out-1 >= r) out-=1;
  return out;
}


void objective(Program& lp){
  for (int i=0; i<n; i++){
    lp.set_c(i, -2*(h+w));
  }
}

void constraints(Program& lp){
  int n_c = 0; // counter of constraints
  // new constraints do not overlap
  for (int i=0; i<n-1; i++){
    for (int j=i+1; j<n; j++){
      long dx = std::abs(pn[i].first - pn[j].first);
      long dy = std::abs(pn[i].second - pn[j].second);
      long d, b;
      if (dx*h>dy*w){
        d = dx;
        b = w;
      }else{
        d = dy;
        b = h;
      }
      lp.set_a(i, n_c, b);
      lp.set_a(j, n_c, b);
      lp.set_b(n_c, 2*d);
      n_c++;
    }
  }
  // old and new constraints do not overlap
  for (int i=0; i<n; i++){
    for (int j=0; j<m; j++){
      long dx = std::abs(pn[i].first - po[j].first);
      long dy = std::abs(pn[i].second - po[j].second);
      long d, b;
      if (dx*h>dy*w){
        d = dx;
        b = w;
      }else{
        d = dy;
        b = h;
      }
      lp.set_a(i, n_c, b);
      lp.set_b(n_c, 2*d-b);
      n_c++;
    }
  }
}

void testcase(){
  std::cin >> n >> m >> h >> w;

  pn.clear(); pn.resize(n);
  for (int i=0; i<n; i++){
    int x, y;
    std::cin >> x >> y;
    pn[i] = std::make_pair(x, y);
  }

  po.clear(); po.resize(m);
  for (int i=0; i<m; i++){
    int x, y;
    std::cin >> x >> y;
    po[i] = std::make_pair(x, y);
  }
  // construct lp
  Program lp (CGAL::SMALLER, true, 1, false, 0); 
  constraints(lp);
  objective(lp);
  
  //solve
  Solution s = CGAL::solve_linear_program(lp, ET());
  std::cout << roundUp(-s.objective_value()) << std::endl;
}


int main(){
  std::ios_base::sync_with_stdio(false);
  std::cout << std::setprecision(10);
  int T; std::cin>>T;
  while(T--)testcase();
  return 0;
}