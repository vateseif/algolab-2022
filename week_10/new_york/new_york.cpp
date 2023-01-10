///2
#include <iostream>
#include <vector>
#include <algorithm>
#include <deque>
#include <map>
#include <set>

#define trace(x) std::cerr << #x << " = " << x << std::endl;

int n, k;
size_t m;

std::vector<int> temp, critical; // route and temp for each island
std::vector<std::vector<int>> routes;
std::map<int, bool> critical_map;

void update_window(std::deque<int>& window, std::map<int, int>& window_temp){
  // check if front node is a critical node
  int min_h = window_temp.begin()->first;
  int max_h = window_temp.rbegin()->first;
  if (!critical_map[window.front()] && max_h - min_h <= k) {
    critical.push_back(window.front());
    critical_map[window.front()] = true; 
  }
  //if (max_h - min_h <= k) critical_set.insert(window.front());
  // remove counter of temperature of front node is the count becomes 0
  if (!(--window_temp[temp[window.front()]]))
    window_temp.erase(window_temp.find(temp[window.front()]));
  // remove front node
  window.pop_front();
}

void sliding_window(std::deque<int> window, std::map<int, int> window_temp,int v){
  // add new node to window
  window.push_back(v);
  window_temp[temp[v]]++;
  // check for critical window and remove oldest
  if (window.size()==m){
    update_window(window, window_temp);
  }
  // add new node
  for (int next_v : routes[v]){
    sliding_window(window, window_temp, next_v);
    //if (critical_map[v]) break;
  }
}


void testcase(){
  std::cin >> n >> m >> k;
  temp.clear();
  temp.resize(n);
  for (int i=0; i<n; i++){
    int hi; std::cin >> hi;
    temp[i] = hi;
  }

  routes.clear();
  routes.resize(n);
  for (int i=0; i<n-1; i++){
    int u, v;
    std::cin >> u >> v;
    routes[u].push_back(v);
  }
  
  critical.clear();
  critical_map.clear();
  std::deque<int> window;
  std::map<int, int> window_temp;
  sliding_window(window, window_temp, 0);

  
  if (critical.size()>0){
    std::sort(critical.begin(), critical.end());
    for (int s : critical) std::cout  << s << " ";
  } else{
    std::cout  << "Abort mission";
  }
  std::cout  << std::endl;

  return;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int T; std::cin >> T;
  while(T--) testcase();
  return 0;
}