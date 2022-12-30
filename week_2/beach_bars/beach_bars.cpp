#include<iostream>
#include<vector>
#include<algorithm>
#include<set>
#include<math.h>

int main(){
  
  // init
  std::ios_base::sync_with_stdio(false);
  int T; std::cin >> T;
  
  for (int t=0; t<T; t++){
    
    int n; std::cin >> n;
    std::vector<int> s;
    for (int i=0; i<n; i++){
      int a; std::cin >> a;
      s.push_back(a);
    }
    std::sort(s.begin(), s.end(), std::less<int>());
    std::vector<int> optimal_x;

    int max_para = 0;
    int minmax_dis = 1e7;
    
    
    for (auto it1=s.begin(); it1!=s.end(); it1++){
      int k = 0;
      int gap = 0;
      std::vector<int> x;
      
      for (auto it2=it1; it2!=s.end(); it2++){
        if(*it2 - *it1 - 200 > 0){
          break;
        }
        k++;
        gap = *it2 - *it1;
      }
      

      int max_dis_r = floor(static_cast<float> (gap) / 2.0);
      int max_dis_l = ceil(static_cast<float> (gap) / 2.0);
      
      if (max_dis_r != max_dis_l){
        x.push_back(*it1 + max_dis_r);
        x.push_back(*it1 + max_dis_l);
      }
      else{
        x.push_back(*it1 + max_dis_l);
      }
      
      if (k == max_para){
        if (max_dis_l == minmax_dis){
          for (int j:x){
            optimal_x.push_back(j);
          }
          
        }
        if (max_dis_l < minmax_dis){
          minmax_dis = max_dis_l;
          optimal_x.clear();
          for (int j:x){
            optimal_x.push_back(j);
          }
        }
      } 
      
      if (k > max_para) {
        max_para = k;
        minmax_dis = max_dis_l;
        optimal_x.clear();
        for (int j:x){
          optimal_x.push_back(j);
        }
      }
    }
    std::sort(optimal_x.begin(), optimal_x.end(), std::less<int>());
    
    std::cout << max_para << " " << minmax_dis << std::endl;
    
    for (auto it = optimal_x.begin(); it != optimal_x.end(); it++){
      std::cout << *it << " ";
    }
    std::cout << std::endl;
    
  }
  
  
  return 0;
}