#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

#define trace(x) std::cerr << #x << " = " << x << std::endl;

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Line_2       L;
typedef K::Point_2      P;
typedef K::Segment_2    S;
typedef K::Triangle_2   T;

std::vector<T> triangles;
std::vector<P> path;
std::vector<std::vector<int>> memo;

bool leg_in_map(const int& l, const int& b, const int& e){
  if (std::find(memo[l].begin()+b, memo[l].begin()+e, 1) != memo[l].begin()+e) return true;

  for (int t=b; t<=e-1; t++){ // TODO make memo mxn so you don't have to recheck when a leg l is not in triangle t
    if (memo[l][t]==0){  
      if (CGAL::do_intersect(path[l], triangles[t]) && CGAL::do_intersect(path[l+1], triangles[t])){
        memo[l][t] = 1;
        return true;
      }
      memo[l][t]=-1;
    }
  }
  
  return false;
}


void construct_triangle(const int& i){
  int x0, y0, x1, y1, x2, y2, x3, y3, x4, y4, x5, y5;
  std::cin >> x0 >> y0 >> x1 >> y1 >> x2 >> y2 >> x3 >> y3 >> x4 >> y4 >> x5 >> y5;

  L al(P(x0, y0), P(x1, y1));
  L bl(P(x2, y2), P(x3, y3));
  L cl(P(x4, y4), P(x5, y5));

  P a, b, c;

  auto ao = CGAL::intersection(al, bl);
  auto bo = CGAL::intersection(al, cl);
  auto co = CGAL::intersection(cl, bl);

  if (const P* ap = boost::get<P>(&*ao)){
    a = *ap;
  }else{
    throw std::invalid_argument("Weird intersection between edges of triangle");
  }
  
  if (const P* bp = boost::get<P>(&*bo)){
    b = *bp;
  }else{
    throw std::invalid_argument("Weird intersection between edges of triangle");
  }
  
  if (const P* cp = boost::get<P>(&*co)){
    c = *cp;
  }else{
    throw std::invalid_argument("Weird intersection between edges of triangle");
  }
      
  triangles[i] = T(a, b, c);
}

void testcase(){

  int m, n;
  std::cin >> m >> n;

  path.clear();
  path.resize(m);
  for (int i=0; i<m; i++){
    int x, y;
    std::cin >> x >> y;
    path[i] = P(x, y);
  }

  triangles.clear();
  triangles.resize(n);
  for (int i=0; i<n; i++){
    construct_triangle(i);
  }

  /*
  memo[i] is a vector containing the index of triangles fully containing leg i
  */
  memo.clear();
  memo.resize(m-1, std::vector<int>(n, 0));
  
  int min_k = n;
  int e=1;
  for (int b=0; b<n; b++){
    while (e-b<min_k && e<=n){
      // check if all legs are contained in triangles in range [b, e-1]
      bool legs_contained = true;
      for (int l=0; l<m-1; l++) if (!leg_in_map(l, b, e)){legs_contained = false; break;}
      if (legs_contained){min_k = e-b; break;}
      e++;
    }
  }

  std::cout << min_k << std::endl;

  return;
}


int main(){
  std::ios_base::sync_with_stdio(false);
  int TT; std::cin >> TT;
  while (TT--) testcase();
  return 0;
}
