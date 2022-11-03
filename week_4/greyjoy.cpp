#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>

int main(){
  
  std::ios_base::sync_with_stdio(false);
  int T; std::cin >> T;

  for (int t=0; t<T; t++){

    int n; std::cin >> n; // number of islands
    int k; std::cin >> k; // total soldiers
    int w; std::cin >> w; // number of waterways

    std::vector<int> soldiers_island(n); // soldiers needed for each island

    for (int i=0; i<n; i++){
        int c; std::cin >> c; // number of men needed for island i
        soldiers_island.push_back(c);
    }

    std::vector<std::vector<int>> islands_waterway(w); // vector of islands in each waterway
    for (int j=0; j<w; j++){
        int l; std::cin >> l; // length of waterway j
        for (int i=0; i<l; i++){
            int a; std::cin >> a; // island a belonging to waterway j
            islands_waterway[j] = a;
        }           
    }   

  }
  
  
  
  return 0;
}