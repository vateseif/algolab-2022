#include<iostream>
#include<vector>

#include<boost/graph/adjacency_list.hpp>


typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                              boost::no_property, boost::property<boost::edge_component_t, int>> graph;

typedef boost::graph_traits<graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<graph>::vertex_descriptor          vertex_desc;



int main(){

    std::ios_base::sync_with_stdio(false);


    








    return 0;
}