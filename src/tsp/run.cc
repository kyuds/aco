#include <iostream>
#include <cstdlib>

#include "aco.hh"
#include "dmatrix.hh"

int main(void) {
    // Settings:
    int min_val = 1;
    int max_val = 10;
    unsigned int seed = 1;
    int graph_size = 20;

    auto dm = fillRandom(graph_size, seed, min_val, max_val);

    // Example for file IO:
    // dm->print();
    // outputToFile("samples/acotsp1.txt", dm);
    // auto dm = fillFromFile("samples/acotsp1.txt");
    // dm->print();

    Aco * aco = new Aco(defaultConfig(), dm);
    std::vector<int> run = aco->optimize(true);

    return 0;
}
