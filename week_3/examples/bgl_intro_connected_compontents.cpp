// Includes
#include <iostream>
#include <vector>
// BGL includes 
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>



// BGL Graph definitions
// =====================
// Graph Type, OutEdgeList Type, VertexList Type, (un)directedS
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS		// Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
		>					graph;
typedef boost::graph_traits<graph>::vertex_descriptor		vertex_desc;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).	
typedef boost::graph_traits<graph>::edge_iterator		edge_it;		// to iterate over all edges


// Function creating the graph. Read edges from input or enter manually.
// ====================================================================
void create_graph(graph &G) {  
	int m = 14;  // m denoting the number of edges
	std::vector<std::pair<vertex_desc, vertex_desc> > graph_edges = { {0,1}, {1,2}, {2,0}, {3,4}, {4,3}, {3,5}, {5,6}, {5,7}, {6,7}, {6,8}, {7,9}, {8,9}, {9,6}, {10, 11} };
	for (int i = 0; i < m; ++i)
		boost::add_edge(graph_edges[i].first, graph_edges[i].second, G);  // adding edges to the graph
	
	// Output the created graph
	std::cout << "We created a graph on " << boost::num_vertices(G) << " vertices and with the following edges:" << "\n";
	edge_it ebeg, eend;
	for (boost::tie(ebeg, eend) = boost::edges(G); ebeg != eend; ++ebeg)
		std::cout << *ebeg << "\n";

	std::cout << "\n";
}



// Main function looking for connected compontents
// ===============================================
int main() {
	int n;
	n = 12;
	graph G(n);
	create_graph(G);

	// Connected components
	// ====================
	std::vector<int> component_map(n);	// We MUST use such a vector as an Exterior Property Map: Vertex -> Component
	int ncc = boost::connected_components(G, boost::make_iterator_property_map(component_map.begin(), boost::get(boost::vertex_index, G))); 

	// Output all the components
	// ==========================
	std::cout << "Number of connected components: " << ncc << "\n";
	std::vector<std::vector<vertex_desc>> component_vertices(ncc);
	// Iterate over all vertices
	for (int i = 0; i < n; ++i)
		component_vertices[component_map[i]].push_back(i);

	// Iterate over all components
	for (int c_i = 0; c_i < ncc; ++c_i){
		std::cout << "Component " << c_i << " has size " << component_vertices[c_i].size() << ". ";
		std::cout << "It contains the vertices" ;		
		for (int j = 0; j < component_vertices[c_i].size(); ++j)
			std::cout << " " << component_vertices[c_i][j];
		
		std::cout << ".\n";
	}

	return 0;
}

