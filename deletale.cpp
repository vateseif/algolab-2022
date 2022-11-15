#include<limits>
#include<iostream>

int main(){
  int a = std::numeric_limits<int>::min();
  std::cout << a;
  return 0;
}