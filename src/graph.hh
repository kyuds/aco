#ifndef ACOTSP_GRAPH
#define ACOTSP_GRAPH

#include <vector>

class Node {
    public:
        Node(int _id);
        ~Node() {};
    private:
        int id;
        std::vector<Edge*> edges;
};

class Edge {
    public:
        Edge(Node * n1, Node * n2, int _weight);
        ~Edge() {};
    private:
        int weight;
        Node * nd1;
        Node * nd2;
};

std::vector<Node*> random_complete_n_node_graph(int n);
void cleanup(std::vector<Node*> graph);

#endif
