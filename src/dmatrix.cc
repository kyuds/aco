#include "dmatrix.hh"

#include <climits>
#include <cstdlib>
#include <cstdio>

// implementation of type-specific DMatrix functions.

DMatrix<int> * fillRandom(int length, unsigned int seed, int min, int max) {
    DMatrix<int> * dm = new DMatrix<int>(length);
    srand(seed);
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < length; j++) {
            if (i == j) dm->set(i, j, -1);
            else dm->set(i, j, min + (rand() % max));
        }
    }
    return dm;
}

DMatrix<int> * fillFromFile(const char * filename) {
    FILE * f = fopen(filename, "r");
    assert(f != NULL);
    char buf[128];
    assert(fgets(buf, INT_MAX, f) != NULL);
    int length = atoi(buf);
    assert(length > 0);

    DMatrix<int> * dm = new DMatrix<int>(length);
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < length; j++) {
            assert(fscanf(f, "%s ", buf) == 1);
            dm->set(i, j, atoi(buf));
        }
    }
    return dm;
}

void outputToFile(const char * filename, DMatrix<int> * dm) {
    FILE * f = fopen(filename, "w");
    assert(f != NULL);
    fprintf(f, "%d\n", dm->getLength());
    for (int i = 0; i < dm->getLength(); i++) {
        for (int j = 0; j < dm->getLength(); j++) {
            fprintf(f, "%d ", dm->get(i, j));
        }
        fprintf(f, "\n");
    }
    fclose(f);
}
