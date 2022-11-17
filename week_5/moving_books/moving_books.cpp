///1
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

#define trace(x) std::cerr << #x << " = " << x << std::endl;

int n, m;
std::vector<int> strength;
std::multiset<int> ws;

void pick_box(int& left){
  for (int i=0; i<n; i++){
    int s = strength[i];
    auto w = ws.lower_bound(s);
    if (w != ws.end()){
      ws.erase(w);
      left--;
    }else{
      break;
    }
  }
}


void testcase(){
  std::cin >> n >> m;

  strength.clear(); strength.resize(n);
  for (int i=0; i<n; i++) {int s; std::cin >> s; strength[i] = -s;}

  ws.clear();
  for (int i=0; i<m; i++) {int w; std::cin >> w; ws.insert(-w);};

  std::sort(strength.begin(), strength.end());

  if (*ws.begin() < strength[0]){
      std::cout << "impossible" << std::endl;
      return;
  }

  /*
  greedy: strongest person starts by picking heaviest box he/she can lift
  */
  int t = 0;
  int left = m;
  while (left > 0){
    pick_box(left);
    t++;
  }

  std::cout << 3*t - 1 << std::endl;

}


int main(){
  std::ios_base::sync_with_stdio(false);
  int T; std::cin >> T;
  while(T--) testcase();
  return 0;
}