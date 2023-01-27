#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

#define trace(x) std::cerr << #x << " = " << x << std::endl;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;

int m, n;
std::vector<P> legs;
std::vector<std::vector<bool>> memo; // store which triangles cover which node (int indexed by order)


bool check(std::vector<P>& pts, int& j){
  // return true if point j is contained in triangle. i.e. point is not in right turn of any edge
  return (!CGAL::right_turn(pts[0], pts[1], legs[j]) &&
  !CGAL::right_turn(pts[2], pts[3], legs[j]) && 
  !CGAL::right_turn(pts[4], pts[5], legs[j]));
}


int sliding_window(){
  int b = 0;
  int min_k = n; // min number of triangles needed
  int l_missing = m-1; // number of legs to be covered missing
  std::vector<int> window(m-1, 0); // number of triangles covering each leg in current b...e-1
  for (int e=0; e<n; e++){
    // insert triangle and check which legs it covers
    for (int j=0; j<m-1; j++){
      if (memo[e][j] && memo[e][j+1]){
        if (window[j]++==0){
          l_missing--;
        }
      }
    }

    while (l_missing == 0){
      //update min cost
      min_k = std::min(min_k, e-b+1);
      // remove earliest triangle
      for (int j=0; j<m-1; j++){
        if (memo[b][j] && memo[b][j+1]){
          if (--window[j]==0){
            l_missing++;
          }
        }
      }
      b++;
    }
  }

  return min_k;

}


void testcase(){
  std::cin >> m >> n;

  // store legs as sequence of points
  legs.clear(); legs.resize(m);
  for (int i=0; i<m; i++){
    int x, y;
    std::cin >> x >> y;
    legs[i] = P(x, y);
  }

  // compute triangles
  memo.clear();
  memo.resize(n, std::vector<bool>(m, false));
  for (int i=0; i<n; i++){
    int q0x, q1x, q2x, q3x, q4x, q5x;
    int q0y, q1y, q2y, q3y, q4y, q5y;
    std::cin >> q0x >> q0y >> q1x >> q1y >> q2x >> q2y;
    std::cin >> q3x >> q3y >> q4x >> q4y >> q5x >> q5y;
    P q0(q0x, q0y), q1(q1x, q1y), q2(q2x, q2y);
    P q3(q3x, q3y), q4(q4x, q4y), q5(q5x, q5y);
    // swap points of needed for orientation test
    if (CGAL::right_turn(q0,q1,q2)) std::swap(q0, q1);
    if (CGAL::right_turn(q2,q3,q4)) std::swap(q2, q3);
    if (CGAL::right_turn(q4,q5,q0)) std::swap(q4, q5);
    std::vector<P> pts = {q0,q1,q2,q3,q4,q5};
    // check which leg is contained in which triangle
    for (int j=0; j<m; j++){
      memo[i][j] = check(pts, j);
    }
  }

  // loop through with sliding window
  int k = sliding_window();

  std::cout << k << std::endl;
  return;
}



int main(){
  std::ios_base::sync_with_stdio(false);
  int T; std::cin >> T;
  while (T--) testcase();
  return 0;
}