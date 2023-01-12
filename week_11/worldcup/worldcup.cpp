///3
#include <iostream>
#include <vector>
#include <unordered_map>

// example: how to solve a simple explicit LP
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>


#define trace(x) std::cerr << #x << " = " << x << std::endl;

// choose input type (input coefficients must fit)
typedef long IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef K::Point_2 P;

int n, m, c;


std::vector<int> supply, alcohol, ulimit, demand;
std::vector<std::vector<int>> revenue, lines_crossed;

std::vector<P> stadium, warehouse;
std::unordered_map<P, int> stadium_idx, warehouse_idx;

double floor_to_double(const CGAL::Quotient<ET>& x)
{
 double a = std::floor(CGAL::to_double(x));
 while (a > x) a -= 1;
 while (a+1 <= x) a += 1;
 return a;
}

void solve(){
  // create an LP with Ax <= b, lower bound 0 and no upper bounds
  Program lp (CGAL::SMALLER, true, 0, false, 0); 

  // decisions variables:
  // x_ij: from warehouse i to stadium j
  // x_0...x_m-1: from warehous 0 to all m stadiums

  // contraints to not exceed warehouse supply (n constraints)
  for (int i=0; i<n; i++){
    for (int j=0; j<m; j++){
      lp.set_a(i*m+j, i, 1);
    }
    lp.set_b(i, supply[i]);
  }

  // constraints to not exceed stadiums alcohol (m constraints)
  for (int j=0; j<m; j++){
    for (int i=0; i<n; i++){
      lp.set_a(i*m+j, n+j, alcohol[i]);
    }
    lp.set_b(n+j, ulimit[j]*100);
  }


  // constraints to match exactly stadium demands (2*m consraints)
  for (int j=0; j<m; j++){
    for (int i=0; i<n; i++){
      lp.set_a(i*m+j, n+m+j, 1);
      lp.set_a(i*m+j, n+2*m+j, -1);
    }
    lp.set_b(n+m+j, demand[j]);
    lp.set_b(n+2*m+j, -demand[j]);
  }

  for (int i=0; i<n; i++){
    for (int j=0; j<m; j++){
      lp.set_c(i*m+j, -100*revenue[i][j]+lines_crossed[i][j]);
    }
  }

  // solve the program, using ET as the exact type
  Solution s = CGAL::solve_linear_program(lp, ET());

  if (s.is_optimal()){
    std::cout << floor_to_double(-s.objective_value()/100);
  }else{
    std::cout << "RIOT!";
  }

  std::cout << std::endl;
  return;

}


void testcase(){
  std::cin >> n >> m >> c;


  supply.clear();
  supply.resize(n);
  alcohol.clear();
  alcohol.resize(n);
  warehouse.clear();
  warehouse.resize(n);
  warehouse_idx.clear();
  for (int i=0; i<n; i++){
    int x, y, s, a;
    std::cin >> x >> y >> s >> a;
    P p(x, y);
    supply[i] = s;
    alcohol[i] = a;
    warehouse[i] = p;
    warehouse_idx[p] = i;
  }


  ulimit.clear();
  ulimit.resize(m);
  demand.clear();
  demand.resize(m);
  stadium.clear();
  stadium.resize(m);
  stadium_idx.clear();
  for (int i=0; i<m; i++){
    int x, y, d, u;
    std::cin >> x >> y >> d >> u;
    P p(x, y);
    demand[i] = d;
    ulimit[i] = u;
    stadium[i] = p;
    stadium_idx[p] = i;
  }


  revenue.clear();
  revenue.resize(n, std::vector<int>(m, 0));
  for (int i=0; i<n; i++){
    for (int j=0; j<m; j++){
      int r; std::cin>>r;
      revenue[i][j] = r;
    }
  }

  // create triangulation 
  Triangulation ts, tw;
  ts.insert(stadium.begin(), stadium.end());
  tw.insert(warehouse.begin(), warehouse.end());

  lines_crossed.clear();
  lines_crossed.resize(n, std::vector<int>(m, 0));
  for (int i=0; i<c; i++){
    int x, y;
    long rr;
    std::cin >> x >> y >> rr; 
    rr*=rr;
    P center(x, y);
    P nearest_s = ts.nearest_vertex(center)->point();
    P nearest_w = tw.nearest_vertex(center)->point();
    long dns = CGAL::squared_distance(center, nearest_s);
    long dnw = CGAL::squared_distance(center, nearest_w);
    if (dnw < rr || dns < rr){
      for (P w : warehouse){
        long dw = CGAL::squared_distance(center, w);
        for (P s : stadium){
          long ds = CGAL::squared_distance(center, s);
          if ((ds<rr)!=(dw<rr)){
            lines_crossed[warehouse_idx[w]][stadium_idx[s]]++;
          }
        }
      }
    }

  }

  solve();

  return;
}



int main(){
  std::ios_base::sync_with_stdio(false);
  std::cout << std::setprecision(17); 
  int T; std::cin >> T;
  while (T--) testcase();  
  return 0;
}