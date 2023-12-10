#include "graph.hh"

#include <cstdlib>

// class method implementations
Node::Node(int _id) {
    id = _id;
}

void Node::addEdge(Edge * e) {
    edges.push_back(e);
}

Edge::Edge(Node * n1, Node * n2, int _weight) {
    nd1 = n1;
    nd2 = n2;
    weight = _weight;
}

// graph generator
std::vector<Node*> random_complete_n_node_graph(int n, unsigned int seed) {
    std::vector<Node*> ret;
    srand(seed);
    for (int id = 0; id < n; id++) {
        ret.push_back(new Node(id));
    }
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            Node * n1 = ret.at(i);
            Node * n2 = ret.at(j);
            int w = rand() % (WEIGHT_MAX - WEIGHT_MIN + 1) + WEIGHT_MIN;
            Edge * e = new Edge(n1, n2, w);
            n1->addEdge(e);
            n2->addEdge(e);
        }
    }
    return ret;
}

void cleanup(std::vector<Node*> graph) {
    
}
