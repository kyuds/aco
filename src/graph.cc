#include "graph.hh"

// class method implementations
Node::Node(int _id) {
    id = _id;
}

Edge::Edge(Node * n1, Node * n2, int _weight) {
    nd1 = n1;
    nd2 = n2;
    weight = _weight;
}

// graph generator
std::vector<Node*> random_complete_n_node_graph(int n) {
    std::vector<Node*> ret;
    for (int id = 0; id < n; id++) {
        ret.push_back(new Node(id));
    }
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            // generate edges
        }
    }
    return ret;
}

void cleanup(std::vector<Node*> graph) {
    
}
