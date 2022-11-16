///1
#include <iostream>
#include <vector>
#include <algorithm>

#define trace(x) std::cerr << #x << " = " << x << std::endl;

int n, m;
std::vector<int> strength, weight, plan;

void pick_box(int& left){
  for (int i=0; i<n; i++){
    int s = strength[i];
    for (int j=plan[i]; j<m; j++){
      if (s >= weight[j]){
        weight[j] = std::numeric_limits<int>::max();
        plan[i] = j;
        left--;
        break;
      }
      plan[i] = m;
    }
  }
}

void testcase(){
  std::cin >> n >> m;

  strength.clear(); strength.resize(n);
  for (int i=0; i<n; i++){
    int s; std::cin >> s;
    strength[i] = s;
  }

  weight.clear(); weight.resize(m);
  for (int i=0; i<m; i++){
    int w; std::cin >> w;
    weight[i] = w;
  }

  std::sort(strength.begin(), strength.end(), std::greater<int>());
  std::sort(weight.begin(), weight.end(), std::greater<int>());

  if (weight[0] > strength[0]){
      std::cout << "impossible" << std::endl;
      return;
  }

  plan.clear(); plan.resize(n, 0);

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