///1
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

#define trace(x) std::cerr << #x << " = " << x << std::endl;


struct Move{
  long d, t;
  Move(){};
  Move(long di, long ti){
    d = di; t = ti;
  }
};


int n, m;
long D, T;
std::vector<Move> moves;
std::vector<long> gulps;


bool split_list(int start, int stop, std::vector<Move>& S){
  for (int s = 0; s < 1<<(stop-start); ++s) { // Iterate through all subsets
    long sum_d = 0;
    long sum_t = 0;
    for (int i = 0; i < (stop-start); ++i) {
      if (s & 1<<i){
        sum_d += moves[start+i].d;
        sum_t += moves[start+i].t;
        if (sum_d >= D && sum_t < T) return true;
      }
    }
    if (sum_t < T) S.push_back(Move(sum_d, sum_t));
  }
  return false;
}

bool solve(){
  int n1 = n/2;
  // vector containing the subset sums
  std::vector<Move> S1, S2;
  // compute subset sums of d and t
  if (split_list(0, n1, S1)) return true;
  if (split_list(n1, n, S2)) return true;

  // sort S2 decreasingly by time
  std::sort(S2.begin(), S2.end(), [](Move a, Move b) {
        return a.t < b.t;   
    });
  // if movement with less time and more d than take it
  for (unsigned int i=1; i<S2.size(); i++) S2[i].d = std::max(S2[i].d, S2[i-1].d);


  
  // for each sum in S1 find sum in S2 that is strictly smaller than T
  for (unsigned int i=0; i<S1.size(); i++){
    // find max allowable time for movement in S2
    long t_max = T - S1[i].t;
    auto it = std::lower_bound(S2.begin(), S2.end(), t_max, [](Move a, long t_m) {
        return a.t < t_m;   
    });
    // if all times bigger then skip
    if (it == S2.begin()) continue;
    if ((--it)->d + S1[i].d >= D) return true;
  }
  

  return false;
}

std::string testcase(){
  std::cin >> n >> m >> D >> T;

  moves.clear(); moves.resize(n);
  for (int i=0; i<n; i++){
    long di, ti; std::cin >> di >> ti;
    moves[i] = Move(di, ti);
  }


  gulps.clear(); gulps.resize(m);
  for (int i=0; i<m; i++) std::cin >> gulps[i];

  if (solve()) return "0";
  if (m==0) return "Panoramix captured";
  
  int l=0;
  int r=m;
  while (l<r){
    int mid = (l+r)/2;
    // update moves with the mid-th gulp
    for (int i=0; i<n; i++) moves[i].d += gulps[mid];
    if (solve()){
      r = mid;
    }else{
      l = mid + 1;
      trace("here");
    }
    // remove the mid-th gulp
    for (int i=0; i<n; i++) moves[i].d -= gulps[mid];
  }
  
  if (l==m) return "Panoramix captured";
  return std::to_string(l+1);
}


int main(){
  std::ios_base::sync_with_stdio(false);
  int T; std::cin>>T;
  while (T--) std::cout << testcase() << std::endl;
  return 0;
}