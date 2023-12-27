#ifndef ACO_TSP_ALGO
#define ACO_TSP_ALGO

#define MULTITHREADING_ACO
// #define DEV_MODE

#ifdef MULTITHREADING_ACO
#include <mutex>
#include <semaphore>
#include <thread>
#endif

#include <random>
#include <vector>

#include "dmatrix.hh"

/*
- All distances should be non-negative
- -1 is a placeholder for own node.
- Assumes complete graph for now.

Todo:
- Don't assume complete graph. 
*/

// numAnts when MULTITHREADING is enabled
// is number of cycles per thread per
// iteration.
typedef struct {
#ifdef MULTITHREADING_ACO
    int numThreads;
#endif
    int numAnts;
    int numIter;
    int alpha;
    int beta;
    double decay;
} Config;

class Aco {
    public:
        Aco(Config _c, DMatrix<int> * _graph);
        ~Aco();

        std::vector<int> optimize(bool verbose);
        class Ant; // for declaration purposes. 

    private:
        void updatePheromones();

        Config c;
        DMatrix<int> * graph;
        DMatrix<double> * cumul_pheromone_map;
        DMatrix<double> * partial_pheromone_map;
        std::vector<Ant*> ants;

#ifdef MULTITHREADING_ACO
        static void thread_func(Ant * a, bool& joinNow, int numAnts);
        std::vector<std::thread> threads;
        bool joinNow;
        std::mutex ppmap_mutex;
#endif
    
    public:
        class Ant {
            public:
                Ant(int id, 
                    DMatrix<int> * g, 
                    DMatrix<double> * t, 
                    DMatrix<double> * p, 
                    int _alpha, int _beta
#ifdef MULTITHREADING_ACO
                    ,std::mutex * m);
#else
                    );
#endif
                ~Ant() {}

                void work();

                std::vector<int>& getRun() { return run; }
                int getScore() { return score; }

#ifdef MULTITHREADING_ACO
                std::counting_semaphore<1>& waker() { return wakeThread; }
                std::counting_semaphore<1>& signal() { return signalDone; }
                std::mutex * ppmap_mutex;
#endif
            
            private:
                // pointer to original distance matrix graph.
                // these "reference" graphs are READ ONLY 
                // partial pheromone map only supplies additive sums for
                // pheromones to keep data copying light. 
                DMatrix<int> * reference_graph;
                DMatrix<double> * reference_pheromones;
                DMatrix<double> * partial_pheromone_map;

                // settings (need it here or in main?)
                int alpha;
                int beta;

                // for individual runs
                std::vector<int> run;
                int score;

                // for path choosing
                std::mt19937_64 gen;

                // for multithreading
#ifdef MULTITHREADING_ACO
                // lock for partial_pheromone_map
                std::counting_semaphore<1> wakeThread {0};
                std::counting_semaphore<1> signalDone {0};
#endif
        };
};

bool validateConfig(Config c);
Config defaultConfig();

#endif
