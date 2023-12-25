#ifndef ACO_TSP_DMATRIX
#define ACO_TSP_DMATRIX

#include <cstdlib>
#include <iostream>

#define DEV_MODE

// Graph represented as a distance matrix 
// for ant colony optimization

template <typename T>
class DMatrix {
    public:
        DMatrix(int _length);
        ~DMatrix();

        T get(int i, int j);
        void set(int i, int j, T val);
        int getLength();

#ifdef DEV_MODE
        void print();
#endif
    private:
        T ** arr;
        int length;
};

// specifically for randomizing a TSP problem
// diagonal (x == y) will be set to -1 (because you can't get to yourself)
DMatrix<int> * fillRandom(int length, unsigned int seed, int min, int max);

// also for generating a TSP problem from file
DMatrix<int> * fillFromFile(const char * filename);

// for making examples
#ifdef DEV_MODE
void outputToFile(const char * filename, DMatrix<int> * dm);
#endif

// mostly for pheromone map init.
template <typename T>
DMatrix<T> * fillUniform(int length, T val) {
    DMatrix<T> * dm = new DMatrix<T>(length);
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < length; j++) {
            dm->set(i, j, val);
        }
    }
    return dm;
}

// impl
template <typename T>
DMatrix<T>::DMatrix(int _length) {
    assert(_length > 0);
    length = _length;
    arr = (T **) malloc(length * sizeof(T *));
    assert(arr != NULL);
    for (int i = 0; i < length; i++) {
        arr[i] = (T *) malloc(length * sizeof(T));
        assert(arr[i] != NULL);
    }
}

template <typename T>
DMatrix<T>::~DMatrix() {
    for (int i = 0; i < length; i++) {
        free(arr[i]);
    }
    free(arr);
}

template <typename T>
T DMatrix<T>::get(int i, int j) {
    return arr[i][j];
}

template <typename T>
void DMatrix<T>::set(int i, int j, T val) {
    arr[i][j] = val;
}

template <typename T>
int DMatrix<T>::getLength() {
    return length;
}

#ifdef DEV_MODE
template <typename T>
void DMatrix<T>::print() {
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < length; j++) {
            std::cout << get(i, j) << " ";
        }
        std::cout << std::endl;
    }
}
#endif

#endif
