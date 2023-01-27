///1
#include <iostream>
#include <vector>
#include <algorithm>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

#define trace(x) std::cerr << #x << " = " << x << std::endl;

typedef CGAL::Exact_predicates_exact_constructions_kernel K;

struct Biker{
  int id;
  K::FT y0, x1, y1;
  Biker(){};
  Biker(int idi, long y0i, long x1i, long y1i){
    id=idi; y0=y0i; x1=x1i; y1 =y1i;
  }
};

int n;
std::vector<bool> forever;
std::vector<Biker> up_bikers, down_bikers, mixed_bikers;

void solve(){
  // bikers goinf up that remove each other
  Biker b_min = up_bikers[0];
  for (int i=0; i<n; i++){
    Biker bi = up_bikers[i];
    if ((bi.y1-bi.y0)*b_min.x1 <= (b_min.y1-b_min.y0)*bi.x1){
      forever[bi.id] = true;
      mixed_bikers.push_back(bi);
      b_min = bi;
    }
  }
  // bikers going down that remove each other
  Biker b_max = down_bikers[0];
  for (int i=0; i<n; i++){
    Biker bi = down_bikers[i];
    if ((bi.y1-bi.y0)*b_min.x1 >= (b_min.y1-b_min.y0)*bi.x1){
      forever[bi.id] = true;
      mixed_bikers.push_back(bi);
      b_min = bi;
    }
  }

  // mixed bikers that remove each other, remaining ride forever
  std::sort(mixed_bikers.begin(), mixed_bikers.end(), [](Biker a, Biker b) {
      return a.y0 > b.y0;   
  });
  b_min = mixed_bikers[0];
  for (int i=0; i<n; i++){
    Biker bi = up_bikers[i];
    if (CGAL::abs(bi.y1-bi.y0)*b_min.x1 <= CGAL::abs(b_min.y1-b_min.y0)*bi.x1 &&
          (bi.y1>=bi.y0 != b_min.y1>=b_min.y0)){
      forever[b_min.id] = false;
      b_min = bi;
    }
  }

}

void testcase(){
  std::cin>>n;

  up_bikers.clear(); 
  down_bikers.clear();
  for (int i=0; i<n; i++){
    long y0, x1, y1;
    std::cin >> y0 >> x1 >> y1;
    if (y1>=y0) up_bikers.push_back(Biker(i, y0, x1, y1));
    if (y1<y0) down_bikers.push_back(Biker(i, y0, x1, y1));
  }

  // sort bikers going up decreasigly by y0
  std::sort(up_bikers.begin(), up_bikers.end(), [](Biker a, Biker b) {
      return a.y0 > b.y0;   
  });
  // sort bikers going down increasigly by y0
  std::sort(down_bikers.begin(), down_bikers.end(), [](Biker a, Biker b) {
      return a.y0 < b.y0;   
  });

  //
  forever.clear();
  forever.resize(n, false);
  mixed_bikers.clear();
  solve();

  for (int i=0; i<n;i++) {if (forever[i]) std::cout << i << " ";}
  
  std::cout << std::endl;

  return;
}


int main(){
  std::ios_base::sync_with_stdio(false);
  int T; std::cin >> T;
  while(T--) testcase();
  return 0;
}