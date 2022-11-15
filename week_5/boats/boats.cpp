#include<iostream>
#include<vector>
#include<algorithm>
#include<limits>

#define tarce(x) std::cerr << #x << " = " << x << std::endl;

int n;
std::vector<std::vector<int>> rings;

// sort using a custom function object
struct {
    bool operator()(std::vector<int> a, std::vector<int> b) const
    {   
        return a[1] < b[1];
    }   
} sortRings;

bool greedy(int& delta, int& boats, int& i){
  const int ii = i;
  int min_e = std::numeric_limits<int>::max();
  for (int j=ii+1; j<n; j++){
    if (delta <= rings[j][1]){
      int e = std::max(rings[j][1], delta+rings[j][0]);
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
  rings.resize(n, std::vector<int>(2));
  for (int i=0; i<n; i++){
   int l, p;
   std::cin >> l >> p;
   rings[i] = {l, p};
  }

  std::sort(rings.begin(), rings.end(), sortRings);

  int delta = std::numeric_limits<int>::min();
  int boats = 0;
  int i = -1;
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