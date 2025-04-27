#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <pthread.h>

using namespace std;
using namespace std::chrono;

const int N = 100;
const int THREAD_COUNT = 4;

int A[N][N];
int B[N][N];
int C[N][N];

void create_matrix(int matrix[N][N]) {
    srand(time(0));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matrix[i][j] = rand() % 10;
        }
    }
}

struct ThreadData {
    int start_row;
    int end_row;
};

void* multiply_part(void* arg) {
    ThreadData* data = (ThreadData*) arg;
    int start_row = data->start_row;
    int end_row = data->end_row;

    for (int i = start_row; i < end_row; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = 0;
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return nullptr;
}

int main() {
    create_matrix(A);
    create_matrix(B);

    auto start_time = high_resolution_clock::now();

    pthread_t threads[THREAD_COUNT];
    ThreadData thread_data[THREAD_COUNT];
    int rows_per_thread = N / THREAD_COUNT;

    for (int i = 0; i < THREAD_COUNT; i++) {
        thread_data[i].start_row = i * rows_per_thread;
        thread_data[i].end_row = (i == THREAD_COUNT - 1) ? N : (i + 1) * rows_per_thread;

        pthread_create(&threads[i], nullptr, multiply_part, &thread_data[i]);
    }

    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], nullptr);
    }

    auto stop_time = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(stop_time - start_time);

    cout << "Execution time: " << duration.count() << " nanoseconds" << endl;

    return 0;
}
