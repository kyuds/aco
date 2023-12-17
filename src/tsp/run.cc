#include <iostream>
#include <cstdlib>

#include "dmatrix.hh"

int main(int argc, char ** argv) {
    int graph_size = 20;
    if (argc == 2) {
        graph_size = atoi(argv[1]) || graph_size;
    }

    // auto d = fillRandom(graph_size, 0, 1, 10);
    // d->print();
    // outputToFile("samples/acotsp1.txt", d);

    auto dm = fillFromFile("samples/acotsp1.txt");
    dm->print();

    return 0;
}
