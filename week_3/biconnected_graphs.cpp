#include <boost/config.hpp>
#include <vector>
#include <list>
#include <boost/graph/biconnected_components.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <unordered_map>

namespace boost
{
    struct edge_component_t
    {
        enum
        {
            num = 555
        };
        typedef boost::edge_property_tag kind;
    } edge_component;
}


int main()
{
    typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property,
    boost::property<boost::edge_component_t, std::size_t > >
            graph_t;
    typedef boost::graph_traits< graph_t >::vertex_descriptor vertex_t;
    typedef boost::graph_traits< graph_t >::edge_iterator edge_it;
    typedef boost::graph_traits<graph_t>::edge_descriptor		edge_desc;

    int n= 9;
    int m = 10;
    graph_t g(9);
    boost::add_edge(0, 5, g);
    boost::add_edge(0, 1, g);
    boost::add_edge(0, 6, g);
    boost::add_edge(1, 2, g);
    boost::add_edge(1, 3, g);
    boost::add_edge(1, 4, g);
    boost::add_edge(2, 3, g);
    boost::add_edge(4, 5, g);
    boost::add_edge(6, 8, g);
    boost::add_edge(6, 7, g);
    boost::add_edge(7, 8, g);



    std::cout << "Using a std::vector to store edge components" << std::endl;    
    std::vector<int> component_vec(10);
    int num1 = boost::biconnected_components(g, boost::make_iterator_property_map(component_vec.begin(), boost::get(boost::edge_component, g))); 
    std::cerr << "Found " << num1 << " biconnected components.\n";

    for (int i=0; i<10; i++){
        std::cout << component_vec[i] << std::endl;
    }
    std::cout << std::endl;
    


    std::cout << "Using a boost::property_map to store edge components" << std::endl;
    boost::property_map< graph_t, boost::edge_component_t >::type component
            = boost::get(boost::edge_component, g);
    
    int num_comps = boost::biconnected_components(g, component);
    std::cerr << "Found " << num_comps << " biconnected components.\n";

    edge_it ei, ei_end;
    std::vector<std::vector<edge_desc>> bi_comp(m); // contains edges of each component
    for (boost::tie(ei, ei_end) = edges(g); ei != ei_end; ++ei){
        
        int component_val = component[*ei];
        std::cout << "component = " << component_val << ", edge = " << *ei << std::endl;

        if (bi_comp[component_val].size() <= 2){
            bi_comp[component_val].push_back(*ei);
        }
    }

    int k = 0;
    std::vector<edge_desc> critical_bridge;
    std::unordered_map<int, std::vector<int>> lexi_edges(n);
    for (int e=0; e<m; e++){
        // if at component e only 1 edge is found
        if (bi_comp[e].size() == 1){
            k++;
            int u = boost::source(bi_comp[e][0], g);
            int v = boost::target(bi_comp[e][0], g);
            if (u < v){
                lexi_edges[u].push_back(v);
            }else{
                lexi_edges[v].push_back(u);
            }
        }
    }

    std::cout << "k = " << k << std::endl;

    for (int i=0; i<n; i++){
        std::vector<int> neighbors = lexi_edges[i];
        if (neighbors.size() > 0){
            std::sort(neighbors.begin(), neighbors.end());
            for (int v : neighbors){
                std::cout << i << " " << v << std::endl;
            }
        }
    }

    return 0;
}