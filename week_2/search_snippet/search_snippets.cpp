///1
#include <iostream>
#include <vector>
#include <limits>
#include <cmath>
#include <algorithm>


std::vector<int> word_number; // number of times each word occurs in document
std::vector<std::vector<int>> word_location; // stores m locations of the n words

void testcase(){
  int n; std::cin >> n;

  word_number.clear();
  word_location.clear();
  word_number.resize(n);
  word_location.resize(n);
  for  (int i=0;i<n; i++){
    int mi; std::cin>>mi;
    word_number[i] = mi;
    word_location[i].resize(mi);
  }
  
  int a = 0;
  int b = std::pow(2, 30);
  
  for (int i=0; i<n; i++){
    int mi = word_number[i];
    for (int j=0; j<mi; j++){
      int pij; std::cin >> pij;
      word_location[i][j] = pij;
      if (i>0){
        auto it_closest = std::upper_bound(word_location[i-1].begin(), word_location[i-1].end(), pij);
        if (it_closest == word_location[i-1].end()){
          it_closest--;
          int p_previous = *it_closest;
          if (pij - p_previous < b-a){
            a = p_previous;
            b = pij;
          }
        }else{
          if (*it_closest - pij < b-a){
            a = pij;
            b = *it_closest;
          }
          if (it_closest==word_location[i-1].begin()) continue;
          it_closest--;
          if (pij - *it_closest < b-a){
            a = *it_closest;
            b = pij;
          }
        }
      }
    }
  }
  std::cout << b-a+1 << std::endl;
  
  return;
}


int main(){
  std::ios_base::sync_with_stdio(false);
  int T; std::cin>>T;
  while(T--) testcase();
  return 0;
}