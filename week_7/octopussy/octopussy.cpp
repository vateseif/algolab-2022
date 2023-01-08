#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <algorithm>

int n;
std::vector<bool> opened;
std::vector<std::pair<int, int>> bombs;

int turns2open(int idx){
  if (idx>=n) return 0;
  if (opened[idx]) return 0;
  opened[idx] = true;
  return 1+turns2open(2*idx+1)+turns2open(2*idx+2);
}

bool solve(){
  // start opening bombs with lowest time
  // if it's on the ground, it adds the counter by 1
  // if it's on top of other bombs they must have higher time so
  // we open all bombs beneath it and sum 1 to counter for each
  
  std::sort(bombs.begin(), bombs.end());

  int count = 0;
  for(int i=0; i<n; i++){
    count += turns2open(bombs[i].second);
    if (count > bombs[i].first) return false;
  }

  return true;
}

void testcase(){
  std::cin >> n;
  
  bombs.clear(); bombs.resize(n);
  opened.clear(); opened.resize(n, false);
  for(int i=0; i<n; i++){
    int ti; std::cin >> ti;
    bombs[i] = std::make_pair(ti, i);
  }

  std::string out = solve()? "yes" : "no";
  std::cout << out << std::endl;

  return;
}


int main(){
  std::ios_base::sync_with_stdio(false);
  int T; std::cin >> T;
  while (T--) testcase();
  return 0;
}