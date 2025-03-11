#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <cstdlib>

using namespace std;
using namespace std::chrono;

const int N = 100;


void create_matrix(int matrix[N][N]) {

    srand(time(0));
    for (int i = 0; i < N; i++) {
        for(int j  = 0; j < N; j++)
        {
            matrix[i][j] = rand() % 10;
        }
    }
}

void multiply(int A[N][N], int B[N][N], int C[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = 0; // Initialize result matrix element to 0
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main() {
    
int A[N][N];
int B[N][N];
int C[N][N];

create_matrix(A);
create_matrix(B);

auto start_time = high_resolution_clock::now();
multiply(A,B,C);

auto stop_time = high_resolution_clock::now();
auto duration = duration_cast<nanoseconds>(stop_time - start_time);

cout<<"Execution time: " <<duration.count() << " nanaseconds";

return 0;
}