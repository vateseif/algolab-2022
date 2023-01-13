///3
#include <iostream>
#include <vector>
#include <algorithm> 
#include <deque>
#include <map>

#define trace(x) std::cerr << #x << " = " << x << std::endl;

int n, k;
size_t m;
std::vector<int> temp, critical;
std::vector<std::vector<int>> edges;
std::map<int, bool> critical_map;


int update_window(std::deque<int>& window, std::map<int, int>& window_temp){
  int front = window.front();
  // check if front is a critical point
  if (!critical_map[front]){
    int min_h = window_temp.begin()->first;
    int max_h = window_temp.rbegin()->first;
    if (max_h-min_h<=k) {
      critical_map[front] = true;
      critical.push_back(front);
    }
  }
  // remove front element
  window.pop_front();
  // update temperature count (remove in case temp becomes 0)
  if (!(--window_temp[temp[front]]))
    window_temp.erase(window_temp.find(temp[front]));
    
  return front;
}

void reset_window(int front, std::deque<int>& window, std::map<int, int>& window_temp){
  if (front!=-1){
    // add back to front the original node
    window.push_front(front);
    window_temp[temp[front]]++;
  }
  // remove latest node added at the back
  // update temperature count (remove in case temp becomes 0)
  if (!(--window_temp[temp[window.back()]]))
    window_temp.erase(window_temp.find(temp[window.back()]));
  // remove node
  window.pop_back();
}


void dp(int u, std::deque<int>& window, std::map<int, int>& window_temp){
  // store front element of window to be readded
  int front = -1;
  // insert new element
  window.push_back(u);
  window_temp[temp[u]]++;
  // remove top and check if critical point
  if (window.size()==m){
    front = update_window(window, window_temp);
  }
  // carry on with neighbors
  for (int v : edges[u]){
    dp(v, window, window_temp);
  }
  // reset window: remove newest element and readd oldeest
  reset_window(front, window, window_temp);
}

void testcase(){
  std::cin >> n >> m >> k;

  temp.clear();
  temp.resize(n);
  for (int i=0; i<n; i++){
    int t; std::cin >> t;
    temp[i] = t;
  }

  edges.clear();
  edges.resize(n);
  for (int i=0; i<n-1; i++){
    int u, v;
    std::cin >> u >> v;
    edges[u].push_back(v);
  }

  critical.clear();
  critical_map.clear();
  std::deque<int> window;
  std::map<int, int> window_temp;
  
  dp(0, window, window_temp);


  if (critical.size()>0){
    std::sort(critical.begin(), critical.end());
    for (int u : critical) {
      std::cout << u << " ";
    }
  }else{
    std::cout << "Abort mission";
  }
  
  std::cout << std::endl;

  return;
}



int main(){
  std::ios_base::sync_with_stdio(false);
  int T; std::cin >> T;
  while(T--) testcase();
  return 0;
}