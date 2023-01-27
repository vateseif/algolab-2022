#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#define trace(x) std::cerr << #x  << " = " << x << std::endl;

struct Bomb{
  int id;
  long t;
  Bomb(){};
  Bomb(int idi, long ti){
    id = idi; t = ti;
  }
};

int n;
std::vector<Bomb> bombs;
std::vector<long> times;
std::vector<bool> defused;

void defuse(int id, long& t){
  if (defused[id]) return;
  if (id < (n-1)/2){
    defuse(2*id+1, t);
    defuse(2*id+2, t);
  }
  t++;
  defused[id] = true;
}

bool greedy(){
  long t = 0;
  for (Bomb b : bombs){
    defuse(b.id, t);
    if (t>b.t) return false;
  }
  return true;
}


void testcase(){
  std::cin >> n;

  times.clear(); times.resize(n);
  bombs.clear(); bombs.resize(n);
  for (int i=0; i<n; i++){
    long ti; std::cin >> ti;
    bombs[i] = Bomb(i, ti);
    times[i] = ti;
  }

  // idea: defuse bomb with smallest time, or the bombs that are beneath it
  std::sort(bombs.begin(), bombs.end(), [](Bomb a, Bomb b) {
      return a.t < b.t;   
  });

  defused.clear(); defused.resize(n, false);
  std::string out = greedy()? "yes" : "no";
  std::cout << out << std::endl;

}




int main(){
  std::ios_base::sync_with_stdio(false);
  int T; std::cin >> T;
  while (T--) testcase();
  return 0;
}