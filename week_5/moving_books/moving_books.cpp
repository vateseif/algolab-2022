///1
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

#define trace(x) std::cerr << #x << " = " << x << std::endl;

int n, m;
std::vector<int> strength, weight;
std::multiset<int, std::greater<int>> ws;

void pick_box(int& left){
  for (int i=0; i<n; i++){
    auto j = ws.lower_bound(strength[i]);
    if (j != ws.end()){
      ws.erase(j);
      left--;
    }else{
      break;
    }
  }
}

void testcase(){
  std::cin >> n >> m;

  strength.clear(); strength.resize(n);
  for (int i=0; i<n; i++) std::cin >> strength[i];

  weight.clear(); weight.resize(m);
  for (int i=0; i<m; i++) std::cin >> weight[i];

  std::sort(strength.begin(), strength.end(), std::greater<int>());
  std::sort(weight.begin(), weight.end(), std::greater<int>());

  if (weight[0] > strength[0]){
      std::cout << "impossible" << std::endl;
      return;
  }
  
  ws.clear();
  for (int w:weight) ws.insert(w);

  /*
  greedy: strongest person starts by picking heaviest box he/she can lift
  */
  int t = 0;
  int left = m;
  while (left > 0){
    pick_box(left);
    t++;
  }

  std::cout << 3*(t-1) + 2 << std::endl;

}


int main(){
  std::ios_base::sync_with_stdio(false);
  int T; std::cin >> T;
  while(T--) testcase();
  return 0;
}