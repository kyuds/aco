#include "aco.hh"

///////////////////////////////
// ACO Engine Implementation //
///////////////////////////////
Aco::Aco(Config _c, DMatrix<int> * _graph) {
    assert(validateConfig(_c));
    c = _c;
    graph = _graph;

    cumul_pheromone_map = fillUniform<double>(graph->getLength(), 1.0);
    partial_pheromone_map = fillUniform<double>(graph->getLength(), 0.0);

    for (int i = 0; i < c.numAnts; i++) {
        Ant * a = new Ant(i, 
                          graph, 
                          cumul_pheromone_map, 
                          partial_pheromone_map, 
                          c.alpha, c.beta);
        ants.push_back(a);

#ifdef MULTITHREADING_ACO
        // thread creation
#endif
    }
}

Aco::~Aco() {
#ifdef MULTITHREADING_ACO
    // join all threads
#endif

    for (int i = 0; i < ants.size(); i++) {
        delete ants.at(i);
    }
    delete cumul_pheromone_map;
    delete partial_pheromone_map;
}

std::vector<int> Aco::optimize() {
    int bestScore = INT_MAX;
    std::vector<int> bestRun;

    for (int iter = 0; iter < c.numIter; iter++) {
#ifdef MULTITHREADING_ACO
        // sema cycle
#else
        for (auto ant : ants) {
            ant->work();
        }
#endif
        for (auto ant : ants) {
            if (ant->getScore() < bestScore) {
                bestScore = ant->getScore();
                bestRun = ant->getRun();
            }
        }
        updatePheromones();
    }
    return bestRun;
}

// ACO Engine Utils
#ifdef MULTITHREADING_ACO
void Aco::thread_func(Ant * a, bool& joinNow) {
    // thread function --> semaphore cycling. 
}
#endif

void Aco::updatePheromones() {
    for (int i = 0; i < graph->getLength(); i++) {
        for (int j = 0; j < graph->getLength(); j++) {
            double t = cumul_pheromone_map->get(i, j) * c.decay;
            t += partial_pheromone_map->get(i, j);
            cumul_pheromone_map->set(i, j, t);
            partial_pheromone_map->set(i, j, 0.0);
        }
    }
}

///////////////////////////////
// Ant Worker Implementation //
///////////////////////////////

//////////////////
// Config Utils //
//////////////////
bool validateConfig(Config c) {
    if (c.numAnts < 1) return false;
    if (c.numIter < 1) return false;
    if (c.decay < 0 || c.decay > 1) return false;
    if (c.alpha < 0) return false;
    if (c.beta < 0) return false;
    return true;
}

Config defaultConfig() {
    return Config {
#ifdef MULTITHREADING_ACO
        5,      // numAnts
#else
        2,
#endif
        100,    // numIter
        1,      // alpha
        1,      // beta
        0.95    // decay
    };
}
