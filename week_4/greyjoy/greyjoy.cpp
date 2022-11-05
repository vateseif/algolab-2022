///1
#include <iostream>
#include <vector>

typedef std::vector<int>  VI;
typedef std::vector<VI>   VVI;


void sliding_window(VI &soldiers, VI &waterway, VI &memo, int &max_islands, int &k){

  int w_size = waterway.size();
  int counter = 0;
  int j = 0;
  for (int i=0; i<w_size; i++){
    int s = waterway[i];  // start node sliding window
    int e; // end node sliding window
    while (j<w_size){
      e = waterway[j];
      counter += soldiers[e];
      if (counter == k) max_islands = std::max(max_islands, j-i);
      if (i == 0) memo.push_back(counter);
      if (counter >= k) break;
      j++;
    }
    if (counter < k) break;
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

    int max_islands = -1;
    VVI memo(w);
    for (int l=0; l<w; l++){
      sliding_window(soldiers, waterways[l], memo[l], max_islands, k);
      

    }
    
    int s0 = soldiers[0];
    
    for (int w1=0; w1<w-1; w1++){
      for (int w2=w1+1; w2<w; w2++){
        for (int i1=0; i1<memo[w1].size(); i1++){
          for (int i2=max_islands-i1+1; i2<memo[w2].size(); i2++){
            int s1 = memo[w1][i1];
            int s2 = memo[w2][i2];
            if (s1+s2-s0 == k) max_islands = std::max(max_islands, i1+i2);
            if (s1+s2-s0 >= k) break;
          }
        }
      }
    }
    
    std::cout << max_islands + 1 << std::endl;

  }
  
  
  
  return 0;
}