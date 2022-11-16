///1
#include<iostream>
#include<vector>
#include<algorithm>
#include<limits>

#define trace(x) std::cerr << #x << " = " << x << std::endl;

int n;
std::vector<int> l, p;                    // store boat lengths and ring position
std::vector<std::pair<int, int>> rings;   // store boat lengths and ring position


bool greedy(int& delta, int& boats, int& i){
  /*
  Find earliest boat finish that does not collide with finish location delta of previous boat.
  */
  const int ii = i;
  int min_e = std::numeric_limits<int>::max();
  for (int j=ii+1; j<n; j++){
    if (p[j] > min_e) break;  // current earlieast finish < ring node j <= earliest finish node j
    if (delta <= p[j]){       // ensures boat j can be placed within ring
      int e = std::max(p[j], delta+l[j]);
      if (min_e > e){
        min_e = e;
        i = j;
      } 
    }
  }
  if (min_e == std::numeric_limits<int>::max()) return false;
  delta = min_e;
  boats++;
  return true;
}

void testcase(){
  std::cin >> n;
  rings.clear();
  rings.resize(n);
  for (int i=0; i<n; i++){
   int li, pi;
   std::cin >> li >> pi;
   rings[i] = std::make_pair(pi, li);
  }
  std::sort(rings.begin(), rings.end()); // pair is sorted lexicographically so p must go first
  
  l.clear(); l.resize(n);
  p.clear(); p.resize(n);
  for (int i=0; i<n; i++){
    p[i] = rings[i].first;
    l[i] = rings[i].second;
  }

  /*
  greedy algorithm: for each boat, move it to the most left possible (no collision and within ring location)
  then take local optimum of the earliest boat finish
  */
  int i = -1;
  int boats = 0;
  int delta = std::numeric_limits<int>::min();
  while(i < n-1){
    if (!greedy(delta, boats, i)) break;
  }

  std::cout << boats << std::endl;
}


int main(){
  std::ios_base::sync_with_stdio(false);
  int T; std::cin >> T;
  while(T--) testcase();
  return 0;
}