///1
#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>

#define trace(x) std::cerr << #x << " = " << x << std::endl;

int n;
std::vector<std::pair<long, long>> boats;

void dp(int& i, int& counter, long& r, long& r_prev){
  if (i==n) return;
  std::pair<long, long> boat = boats[i++];
  // if right side of previous boat covers pi we pick boat with smallest r
  if (r > boat.first){
    long r_new = std::max(boat.first, r_prev+boat.second);
    r = std::min(r, r_new);
    dp(i, counter, r, r_prev);
  }else{
    r_prev = r;
    r = std::max(boat.first, r+boat.second);
    counter++;
    dp(i, counter, r, r_prev);  
  }
}


void testcase(){
  std::cin >> n;

  // load boats
  boats.clear(); boats.resize(n);
  for (int i=0; i<n; i++){
    long li, pi;
    std::cin >> li >> pi;
    boats[i] = std::make_pair(pi, li);
  }

  // sort boats increasingly on location of p
  std::sort(boats.begin(), boats.end());

  int i = 0;
  int counter = 0;
  long r = std::numeric_limits<long>::min();
  long r_prev = std::numeric_limits<long>::min();

  dp(i, counter, r, r_prev);
  std::cout << counter << std::endl;
}



int main(){
  std::ios_base::sync_with_stdio(false);
  int T; std::cin >> T;
  while (T--) testcase();
  return 0;
}