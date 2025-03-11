#include <chrono>
#include <cstdlib>
#include <iostream>
#include <pthread.h>

using namespace std::chrono;
using namespace std;

struct ThreadData {
    int* v1;
    int* v2;
    int* v3;
    int start;
    int end;
};

void randomVector(int vector[], int size) {
    for (int i = 0; i < size; i++) {
        vector[i] = rand() % 100;
    }
}

void* vectorAdditionThread(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    for (int i = data->start; i < data->end; i++) {
        data->v3[i] = data->v1[i] + data->v2[i];
    }
    return nullptr;
}

int main() {
    unsigned long size = 10000000;
    srand(time(0));

    int* v1 = (int*)malloc(size * sizeof(int));
    int* v2 = (int*)malloc(size * sizeof(int));
    int* v3 = (int*)malloc(size * sizeof(int));

    if (!v1 || !v2 || !v3) {
        cout << "Memory allocation failed!" << endl;
        return 1;
    }

    randomVector(v1, size);
    randomVector(v2, size);

    int num_threads = 4;
    auto start = high_resolution_clock::now();

    pthread_t threads[num_threads];
    ThreadData thread_data[num_threads];
    int chunk_size = size / num_threads;

    for (int i = 0; i < num_threads; i++) {
        thread_data[i].v1 = v1;
        thread_data[i].v2 = v2;
        thread_data[i].v3 = v3;
        thread_data[i].start = i * chunk_size;
        thread_data[i].end = (i == num_threads - 1) ? size : (i + 1) * chunk_size;
        pthread_create(&threads[i], nullptr, vectorAdditionThread, &thread_data[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], nullptr);
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(stop - start);
    cout << "Execution time: " << duration.count() << " nanoseconds" << endl;

    free(v1);
    free(v2);
    free(v3);

    return 0;
}
