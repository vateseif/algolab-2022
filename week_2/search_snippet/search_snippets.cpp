///1
#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>

#define trace(x) std::cerr << #x << " = " << x << std::endl;

std::vector<int> word_number, word_counter; // number of times each word occurs in document
std::vector<std::pair<int, int>> document;


void testcase(){
  int n; std::cin >> n;
  int N=0; // total number of word positions given
  word_number.clear();
  word_number.resize(n);
  for  (int i=0;i<n; i++){
    int mi; std::cin>>mi;
    word_number[i] = mi;
    N+=mi;
  }
  
  document.clear();
  document.resize(N);
  int c = 0;
  for (int i=0; i<n; i++){
    int mi = word_number[i];
    for (int j=0; j<mi; j++){
      int pij; std::cin >> pij;
      document[c] = std::make_pair(pij, i);
      c++;
    }
  }

  std::sort(document.begin(), document.end());

  int back = 0;
  int missing_words = n;
  int min_d = std::numeric_limits<int>::max();
  word_counter.clear(); word_counter.resize(n, 0);
  for (int i=0; i<N; i++){
    if (!word_counter[document[i].second]++){
      missing_words--;
    }
    while (missing_words==0){
      min_d = std::min(min_d, document[i].first-document[back].first+1);
      if (!--word_counter[document[back++].second]){
        missing_words++;
      }  
    }
  }
  
  std::cout << min_d << std::endl;
  return;
}


int main(){
  std::ios_base::sync_with_stdio(false);
  int T; std::cin>>T;
  while(T--) testcase();
  return 0;
}