///
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <deque>

#define trace(x) std::cerr << #x << " = " << x << std::endl;

struct Query{
  int name_idx, max_age, id;
  Query(){};
  Query(int idx, int age, int i){
    name_idx = idx; max_age = age; id = i;
  }
};

int luca;
std::vector<int> ages, ancestor;
std::vector<std::string> idx_name;
std::unordered_map<std::string, int> name_idx;
std::vector<std::vector<int>> offsprings;
std::unordered_map<int, std::vector<Query>> queries;
std::deque<std::pair<int, int>> tree;

int n, q;

void compute_results(std::vector<int>& results, int idx){
  tree.push_front(std::make_pair(idx, ages[idx]));
  // compute queries of current specie with binary search
  for (Query qu : queries[idx]){
    auto res_idx = std::upper_bound(tree.begin(), tree.end(), qu.max_age, [](int ma, std::pair<int, int> b) {
      return ma < b.second;   
    });
    results[qu.id] = (--res_idx)->first;
  }
  for (int idx_off : offsprings[idx]){
    compute_results(results, idx_off);
  }
  tree.pop_front();
}

void testcase(){
  
  std::cin >> n >> q;

  int max_age = -1;
  int max_age_idx = -1;
  name_idx.clear();
  ages.clear(); ages.resize(n);
  idx_name.clear(); idx_name.resize(n);
  for (int i=0; i<n; i++){
    std::string name;
    int age;
    std::cin >> name >> age;
    name_idx[name] = i;
    idx_name[i] = name;
    ages[i] = age;
    if (age > max_age){
      max_age = age;
      max_age_idx = i;
    }
  }

  ancestor.clear(); ancestor.resize(n, -1);
  offsprings.clear(); offsprings.resize(n);
  for (int i=0; i<n-1; i++){
    std::string off, anc;
    std::cin >> off >> anc;
    ancestor[name_idx[off]] = name_idx[anc];
    offsprings[name_idx[anc]].push_back(name_idx[off]);
  }


  queries.clear();
  for (int i=0; i<q; i++){
    std::string name;
    int max_age;
    std::cin >> name >> max_age;
    int idx = name_idx[name];
    queries[idx].push_back(Query(idx, max_age, i));
  }

  tree.clear();
  std::vector<int> results(q);
  compute_results(results, max_age_idx);

  for (int n_idx : results){
    std::cout << idx_name[n_idx] << " ";
  }

  std::cout << std::endl;

  return;
}


int main(){
  std::ios_base::sync_with_stdio(false);
  int T; std::cin >> T;
  while (T--)testcase();
  return 0;
}