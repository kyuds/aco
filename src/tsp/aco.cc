#include "aco.hh"

#include <cmath>
#include <unordered_set>

#ifdef DEV_MODE
#include <iostream>
#endif

///////////////////////////////
// ACO Engine Implementation //
///////////////////////////////
Aco::Aco(Config _c, DMatrix<int> * _graph) {
    assert(validateConfig(_c));
    c = _c;
    graph = _graph;
#ifdef MULTITHREADING_ACO
    joinNow = false;
#endif

    cumul_pheromone_map = fillUniform<double>(graph->getLength(), 1.0);
    partial_pheromone_map = fillUniform<double>(graph->getLength(), 0.0);

#ifdef MULTITHREADING_ACO
    int numAnts = c.numThreads;
#else
    int numAnts = c.numAnts;
#endif

    for (int i = 0; i < numAnts; i++) {
        Ant * a = new Ant(i, 
                          graph, 
                          cumul_pheromone_map, 
                          partial_pheromone_map, 
                          c.alpha, c.beta
#ifdef MULTITHREADING_ACO
                         ,&ppmap_mutex);
#else
                          );
#endif
        ants.push_back(a);

#ifdef MULTITHREADING_ACO
        threads.push_back(std::thread(thread_func, a, std::ref(joinNow), c.numAnts));
#endif
    }
}

Aco::~Aco() {
#ifdef MULTITHREADING_ACO
    for (int i = 0; i < threads.size(); i++) {
        threads.at(i).join();
    }
#endif
    for (int i = 0; i < ants.size(); i++) {
        delete ants.at(i);
    }
    delete cumul_pheromone_map;
    delete partial_pheromone_map;
}

std::vector<int> Aco::optimize(bool verbose) {
    int bestScore = INT_MAX;
    std::vector<int> bestRun;

    for (int iter = 0; iter < c.numIter; iter++) {
#ifdef MULTITHREADING_ACO
        for (auto ant : ants) {
            ant->waker().release();
        }
        for (auto ant : ants) {
            ant->signal().acquire();
        }
#else
        for (auto ant : ants) {
            ant->work();
        }
#endif
        for (auto ant : ants) {
            if (ant->getScore() < bestScore) {
                bestScore = ant->getScore();
                bestRun = ant->getRun();
                if (verbose) {
                    std::cout << "Run: " << iter << " Score: " << bestScore << std::endl;
                    for (auto i : bestRun) {
                        std::cout << i << " ";
                    }
                    std::cout << std::endl << std::endl;
                }
            }
        }
        updatePheromones();
    }
#ifdef MULTITHREADING_ACO
    joinNow = true;
    for (auto ant : ants) {
        ant->waker().release();
    }
#endif
    return bestRun;
}

// ACO Engine Utils
#ifdef MULTITHREADING_ACO
void Aco::thread_func(Ant * a, bool& joinNow, int numAnts) {
    while (!joinNow) {
        a->waker().acquire();
        if (joinNow) break;

        for (int i = 0; i < numAnts; i++) {
            a->work();
        }
        a->signal().release();
    }
}
#endif

void Aco::updatePheromones() {
#ifdef DEV_MODE
    partial_pheromone_map->print();
#endif
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
Aco::Ant::Ant(int id, 
              DMatrix<int> * g, 
              DMatrix<double> * t, 
              DMatrix<double> * p, 
              int _alpha, int _beta
#ifdef MULTITHREADING_ACO
              ,std::mutex * m) {
#else
              ) {
#endif
    reference_graph = g;
    reference_pheromones = t;
    partial_pheromone_map = p;
    alpha = _alpha;
    beta = _beta;

    gen = std::mt19937_64(id);
#ifdef MULTITHREADING_ACO
    ppmap_mutex = m;
#endif
}

void Aco::Ant::work() {
    score = 0;
    run = std::vector<int> {0};
    std::unordered_set<int> visited({0});

    // start node is already chosen (default to first)
    int numNodes = reference_graph->getLength();
    for (int i = 1; i < numNodes; i++) {
        int curr = run.back();
        std::vector<int> candidate_nodes;
        std::vector<double> probabilities;

        for (int c = 0; c < numNodes; c++) {
            if (visited.find(c) != visited.end()) {
                continue;
            }
            candidate_nodes.push_back(c);
            double p1 = pow(1.0 / reference_graph->get(curr, c), alpha);
            double p2 = pow(reference_pheromones->get(curr, c), beta);
            probabilities.push_back(p1 * p2);

#ifdef DEV_MODE
            std::cout << "p1: " << p1 << " p2: " << p2 << std::endl;
#endif
        }
        std::discrete_distribution<double> dist(probabilities.begin(), probabilities.end());
#ifdef DEV_MODE
        for (auto d : probabilities) {
            std::cout << d << " ";
        }
        std::cout << std::endl;
#endif
        int next = candidate_nodes.at(dist(gen));
        run.push_back(next);
        visited.insert(next);
        score += reference_graph->get(curr, next);
    }
    // add end node to start node length to score.
    score += reference_graph->get(run.back(), 0);

#ifdef MULTITHREADING_ACO
    std::lock_guard<std::mutex> g(*ppmap_mutex);
#endif
    // update pheromones
    double ch = 1 / score;
    int curr, prev = run.back();
    for (int i = 0; i < numNodes; i++) {
        curr = run.at(i);
        double new_value = partial_pheromone_map->get(prev, curr) + ch;
        partial_pheromone_map->set(prev, curr, new_value);
        prev = curr;
    }
}


//////////////////
// Config Utils //
//////////////////
bool validateConfig(Config c) {
    if (c.numAnts < 1) return false;
    if (c.numIter < 1) return false;
    if (c.decay < 0 || c.decay > 1) return false;
    if (c.alpha < 0) return false;
    if (c.beta < 0) return false;
#ifdef MULTITHREADING_ACO
    if (c.numThreads < 1) return false;
#endif
    return true;
}

Config defaultConfig() {
    return Config {
#ifdef MULTITHREADING_ACO
        5,      // numThreads
        2,      // numAnts
#else
        5,      // numAnts
#endif
        500,    // numIter
        1,      // alpha
        1,      // beta
        0.95    // decay
    };
}
