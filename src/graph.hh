#ifndef ACOTSP_GRAPH
#define ACOTSP_GRAPH

#include <vector>

#define WEIGHT_MIN 1
#define WEIGHT_MAX 20

// idk if this is good practice though...
class Node;
class Edge;

class Node {
    public:
        Node(int _id);
        ~Node() {};
        void addEdge(Edge * e);
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

std::vector<Node*> random_complete_n_node_graph(int n, unsigned int seed);
void cleanup(std::vector<Node*> graph);

#endif
