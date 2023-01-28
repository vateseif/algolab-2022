///2
#include <iostream>
#include <vector>
#include <string>
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


struct Stable{
  int f, c, k, l, m;
  Stable(){};
  Stable(int fi,int ci, int ki, int li, int mi){
    f=fi; c=ci; k=ki; l=li; m=mi;
  }
};



int n;
std::vector<int> A, B, SA, SB, memo_a;
std::vector<Stable> stables;

bool solve_lp(int a, int p){
  // create an LP with Ax <= b, lower bound 0 and no upper bounds
  Program lp (CGAL::SMALLER, true, 0, true, 1); 
  // set the coefficients 
  const int H1 = 0; 
  const int H2 = 1;
  const int H3 = 2;
  // impose constraint that all filth has to go below ci for each stable
  for (int i=0; i<n; i++){
    lp.set_a(H1, i, -(stables[i].k + a*a)); 
    lp.set_a(H2, i, -(stables[i].l + p*p));
    lp.set_a(H3, i, -(stables[i].m + a*p)); 
    lp.set_b(i, stables[i].c - stables[i].f);
  }

  // return whether a feasible solution exists
  Solution s = CGAL::solve_linear_program(lp, ET());
  return s.is_optimal();
}


void testcase(){
  std::cin >> n;

  stables.clear(); stables.resize(n);
  for (int i=0; i<n; i++){
    int fi, ci, ki, li, mi;
    std::cin >> fi >> ci >> ki >> li >> mi;
    stables[i] = Stable(fi, ci, ki, li, mi);
  }

  A.clear(); A.resize(24);
  SA.clear(); SA.resize(25, 0);
  for (int i=0; i<24; i++){
    int ai; std::cin >> ai;
    A[i] = ai;
  }
  for (int i=1; i<25; i++) SA[i] = SA[i-1] + A[i-1];

  B.clear(); B.resize(24);
  SB.clear(); SB.resize(25, 0);
  for (int i=0; i<24; i++){
    int bi; std::cin >> bi;
    B[i] = bi;
  }
  for (int i=1; i<25; i++) SB[i] = SB[i-1] + B[i-1];

  int l = 0;
  int r = 49;
  memo_a.clear(); memo_a.resize(25, -1);// store for each a the max p that didn't solve lp
  while (l<r){
    int mid = (l+r)/2;
    int diff = std::max(0, mid-24);
    int max = std::min(mid, 24);
    bool solutionFound = false;
    for (int i=diff; i<=max; i++){
      if (mid-i > memo_a[i]){
        if (solve_lp(SA[i], SB[mid-i])) {
          solutionFound=true; 
          break;
        }else{
          memo_a[i] = mid-i;
        }
      }
    }
    if (solutionFound){
      r = mid;
    }else{
      l = mid+1;
    }
  }
  
  std::string out = l==49? "Impossible!" : std::to_string(l);

  std::cout << out << std::endl;
  return;
}




int  main(){
  std::ios_base::sync_with_stdio(false);
  int T; std::cin>>T;
  while(T--) testcase(); 
  return 0;
}