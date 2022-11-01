#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>


typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS>     graph;
typedef boost::graph_traits< graph >::vertex_descriptor vertex_desc;


int main(){
  
  std::ios_base::sync_with_stdio(false);
  int T; std::cin >> T;
  
  for (int t=0; t<T; t++){
    
    int n; std::cin >> n; // num of students
    int c; std::cin >> c; // num of hobbies
    int f; std::cin >> f;
    
    graph g(n);
    std::vector<std::vector<int>> student_hobbies(n);
    std::unordered_map<std::string, int> hobbies_idx(c);
    int idx = 1;
    for(int i=0; i<n; i++){
        for (int j=0; j<c; j++){
            std::string hobby; std::cin >> hobby;
            if (hobbies_idx[hobby] == 0){
              hobbies_idx[hobby] = idx;
              idx++;
            }
            student_hobbies[i].push_back(hobbies_idx[hobby]);
        }
    } 

    for (int u=0; u<n-1; u++){
        std::vector<int> u_hobbies = student_hobbies[u]; // hobbies of student u
        std::sort(u_hobbies.begin(), u_hobbies.end());
        for (int v=u+1; v<n; v++){
            std::vector<int> v_hobbies = student_hobbies[v]; // hobbies of student v
            std::sort(v_hobbies.begin(), v_hobbies.end());
            std::vector<int> uv_intersect; // intersecting hobbies
            std::set_intersection(u_hobbies.begin(), u_hobbies.end(),
                                  v_hobbies.begin(), v_hobbies.end(),
                                  std::back_inserter(uv_intersect));
            if (uv_intersect.size() > f){
              boost::add_edge(u, v, g);
            }
        }
    }

    std::vector<vertex_desc> mate_map(n);
    boost::edmonds_maximum_cardinality_matching(g, &mate_map[0]);
    int n_match = matching_size(g, &mate_map[0]);
    
    //std::cout << n_match << std::endl;
    if (n_match == n/2){
        std::cout << "not optimal" << std::endl;
    }else{
        std::cout << "optimal" << std::endl;
    }
  }

  return 0;
}