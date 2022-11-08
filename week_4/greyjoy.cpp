///1
#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>

typedef std::vector<int>  VI;
typedef std::vector<VI>   VVI;
typedef std::vector<std::unordered_map<int, VI>> VMVI;
typedef std::unordered_set<int> SI;


// TODO: remove memo_indices and just loop over each value in the map

void sliding_window(VMVI &memo, SI &memo_indices, VI &soldiers, VI &waterway, int &k, int &l, int &max_islands){

  int w_size = waterway.size();
  int counter = 0;
  int j=0;
  for (int i=0; i<w_size; i++){
    int s = waterway[i]; // starting island in sliding window
    int e; // ending island in sliding window
    while (j < w_size){
      e = waterway[j];
      counter += soldiers[e];
      if (counter > k) break;
      if (counter==k) max_islands = std::max(max_islands, j-i+1);
      if (s == 0){
        if (memo[0][counter].size()==0) memo[0][counter]={-2, -2};
        if (memo[1][counter].size()==0) memo[1][counter]={-2, -2};
        if (j > memo[0][counter][0]){
            memo[1][counter] = memo[0][counter];
            memo[0][counter] = {j, l};
            memo_indices.insert(counter);
          }else if (j == memo[0][counter][0]){
            if (memo[0][counter][1] != -1) memo[0][counter][1] = -1;
            memo_indices.insert(counter);
          }else if (j > memo[1][counter][0]){
            memo[1][counter] = {j, l};
            memo_indices.insert(counter);
          }else if (j == memo[1][counter][0]){
            if (memo[1][counter][1] != -1) memo[1][counter][1] = -1;
            memo_indices.insert(counter);
          }
      }
      j++;
    }
    if (counter <= k) break;
    counter -= soldiers[s] + soldiers[e];
  }

  return;
}

int main(){
  
  std::ios_base::sync_with_stdio(false);
  int T; std::cin >> T;
  
  for (int t=0; t<T; t++){

    int n; std::cin >> n; // number of islands
    int k; std::cin >> k; // total soldiers
    int w; std::cin >> w; // number of waterways

    VI soldiers(n); // soldiers needed for each island

    for (int i=0; i<n; i++){
        int c; std::cin >> c; // number of men needed for island i
        soldiers[i] = c;
    }

    VVI waterways(w); // vector of islands in each waterway
    for (int j=0; j<w; j++){
        int l; std::cin >> l; // length of waterway j
        for (int i=0; i<l; i++){
            int a; std::cin >> a; // island a belonging to waterway j
            waterways[j].push_back(a);
        }
    }

    int s0 = soldiers[0];
    int max_islands = 0;
    /*
    memo stores max number of islands starting at 0 with j soldiers needed.
    i = 2 (max and second max value)
    j = k (total soldiers)
    memo[i][j] = <islands starting at 0, waterways> 
    */
    VMVI memo(2);
    SI memo_indices;
    for (int l=0; l<w; l++){
      sliding_window(memo, memo_indices, soldiers, waterways[l], k, l, max_islands);
    }
 
    
    for (auto i=memo_indices.begin(); i!=memo_indices.end(); i++){
      if (memo[0][k+s0-*i].size()==0) continue;
      int x1 = memo[0][*i][0];
      int x2;
      if (memo[0][*i][1] != -1 && memo[0][k+s0-*i][1] != -1 && memo[0][*i][1] == memo[0][k+s0-*i][1]){
        x2 = memo[1][k+s0-*i][0];
      }else{
        x2 = memo[0][k+s0-*i][0];
      }
      if (x2==-2) continue;
      max_islands = std::max(max_islands, x1+x2+1);
    }
    
    std::cout << max_islands << std::endl;
    
  }
  
  return 0;
}