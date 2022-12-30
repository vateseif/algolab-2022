///3
#include<iostream>
#include<vector>
#include<algorithm>

typedef std::vector<long> VL; // vector of longs
typedef std::vector<VL>   VVL; // vector of VL

typedef std::vector<int> VI; // vector of ints
typedef std::vector<VI>   VVI; // vector of VI

void solve(VVL &board, VVL &memo, VVI &neighbors, int node, int &k, int k_next){
  
  // map [neighbour node: cost]
  VI next_nodes = neighbors[node];
  
  // max score of picking a neighbor
  VL next_scores;
  long max_score = -1;
  for (int j : next_nodes){
    // score to next node
    long p = board[node][j];
    // check if max score at [j][l-1] is not in memory
    if (memo[j][k_next-1] == -1){
      solve(board, memo, neighbors, j, k, k_next-1);
    }
    long score = p + memo[j][k_next-1];
    max_score = std::max(score, max_score);
  }
  // if node has no neighbors go back to node 0
  if (max_score == -1){
    if (memo[0][k_next] == -1){
      solve(board, memo, neighbors, 0, k, k_next);
    }
    max_score = memo[0][k_next];
  }
  // store in memory
  memo[node][k_next] = max_score;

}

int main(){
  
  //init
  std::ios_base::sync_with_stdio(false);
  int T; std::cin >> T;
  for (int t=0; t<T; t++){
    // get params
    int n; std::cin >> n;
    int m; std::cin >> m;
    long x; std::cin >> x;
    int k; std::cin >> k;
    // init vecotr ML with size of n ndoes
    VVL board(n, VL(n, -1));
    VVI neighbors(n);
    for (int i=0; i<m; i++){
      int u; std::cin >> u;
      int v; std::cin >> v;
      long p; std::cin >> p;
      // load/update connections with max score
      if (board[u][v] == -1){
        neighbors[u].push_back(v);
      }
      if (p >= board[u][v]){
        board[u][v] = p;
      }
    }
    
    /* init memory bank:
    memo is a n x k matrix of longs.
    memo[i][j] indicates the max score achievable starting at node i with j moves
    */
    VVL memo(n, VL(k+1, -1));
    for (int i=0; i<n; i++){
      memo[i][0] = 0;
    }
    
    int min_k = k+1;
    for (int l=1; l<=k; l++){
      if (memo[0][l] == -1){
        solve(board, memo, neighbors, 0, k, l);  
      }
      if (x <= memo[0][l]){
        min_k = l;
        break;
      }
    }
    
    
    if (min_k > k){
      std::cout << "Impossible" << std::endl;
    }
    else{
      std::cout << min_k << std::endl;
    }

  }

  return 0;
}