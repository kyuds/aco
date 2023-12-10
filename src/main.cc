#include <iostream>
#include <vector>
#include <cstdlib>

#include "graph.hh"

int main(int argc, char ** argv) {
    if (argc < 2) {
        std::cout << "Please enter a number." << std::endl;
        return 0;
    }

    int n = atoi(argv[1]);
    if (n < 1) {
        std::cout << "Please enter a positive number." << std::endl;
        return 0;
    }

    std::cout << "Generating a complete graph with " << n << " nodes." << std::endl;
    // using default seed -> guess this is not that important, just need consistent results
    std::vector<Node*> graph = random_complete_n_node_graph(n, 1);
    
    // run ant colony optimization

    cleanup(graph);
    
    return 0;
}
