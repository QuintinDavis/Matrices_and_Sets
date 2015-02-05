#include <iostream>
#include <cstdlib>
#include <sys/time.h>
using namespace std;

const int BLOCKSIZE = 32;

// Global variables to calculate sizes
int *sizes_gpu, *subtotals_gpu, *total_gpu;  // For finding sums

// Stopwatch class
class Stopwatch {
  private:
    timeval initialTime;

  public:
    Stopwatch() {
      reset();
    }

    void reset() {
      gettimeofday(&initialTime, 0);
    }

    float elapsed() const {
      timeval currentTime;
      gettimeofday(&currentTime, 0);

      return (float) (currentTime.tv_sec - initialTime.tv_sec) + ((float) (currentTime.tv_usec - initialTime.tv_usec))/1000000;
    }
};

// Create an TownSet of size n with all false entries
void createTownSet(char *current, int n) {
  char temp[n];
  for (int i=0; i<n; i++)
    temp[i] = 0;
  cudaMemcpy(&current, &temp, n, cudaMemcpyHostToDevice);
}

// Take a union of current_gpu and with specified townset and store it in current_gpu
__global__ void unionTownSet_gpu(char *current, char *townSets, int i, int n) {
  int threadId = blockIdx.x*blockDim.x + threadIdx.x;
  current[threadId] += townSets[i*n+threadId];
}

void unionTownSet(char *current, char *townSets, int i, int n) {
  unionTownSet_gpu<<<n/BLOCKSIZE, BLOCKSIZE>>>(current, townSets, i, n);
}

// Find the total population in all of the town sets
__global__ void sizeTownSet_gpu(char *current, int *pops, int n, int *sizes) {
  int threadId = blockIdx.x*blockDim.x + threadIdx.x;
  if (current[threadId] == 1)
    sizes[threadId] = pops[threadId];
  else
    sizes[threadId] = 0;
}

__global__ void totals_gpu(int *output, int *input) {
  __shared__ int temp[BLOCKSIZE];
  int threadId = blockIdx.x*blockDim.x + threadIdx.x;
  temp[threadIdx.x] = input[threadId];
  __syncthreads();

  for (int s=blockDim.x/2; s>0; s>>=1) {
    if (threadIdx.x < s)
      temp[threadIdx.x] += temp[threadIdx.x+s];
    __syncthreads();
  }

  if (threadIdx.x == 0)
    output[blockIdx.x] = temp[0];
}

int sizeTownSet(char *current, int *pops, int n) {
  sizeTownSet_gpu<<<n/BLOCKSIZE, BLOCKSIZE>>>(current, pops, n, sizes_gpu);
  totals_gpu<<<n/BLOCKSIZE, BLOCKSIZE>>>(subtotals_gpu, sizes_gpu);  // Calculate subtotals
  totals_gpu<<<n/BLOCKSIZE, BLOCKSIZE>>>(total_gpu, subtotals_gpu);  // Calculate subtotals
  int result;
  cudaMemcpy(total_gpu, &result, sizeof(int), cudaMemcpyDeviceToHost);
  return result;
}

// Count number of towns in a set
__global__ void countTownSet_gpu(char *current, int n, int *sizes) {
  int threadId = blockIdx.x*blockDim.x + threadIdx.x;
  if (current[threadId] == 1)
    sizes[threadId] = 1;
  else
    sizes[threadId] = 0;
}

int countTownSet(char *current, int n) {
  countTownSet_gpu<<<n/BLOCKSIZE, BLOCKSIZE>>>(current, n, sizes_gpu);
  totals_gpu<<<n/BLOCKSIZE, BLOCKSIZE>>>(subtotals_gpu, sizes_gpu);  // Calculate subtotals
  totals_gpu<<<n/BLOCKSIZE, BLOCKSIZE>>>(total_gpu, subtotals_gpu);  // Calculate subtotals
  int result;
  cudaMemcpy(total_gpu, &result, sizeof(int), cudaMemcpyDeviceToHost);
  return result;
}

int main(int argc, char **argv) {
  if (argc != 4) {
    cout << "Usage: " << argv[0] << " mode size reps" << endl;
    cout << "  Mode is one of the following:" << endl;
    cout << "    a  test full algorithm" << endl;
    cout << "    u  test taking unions" << endl;
    cout << "    s  test calculating sizes" << endl;
    exit(0);
  }
  int size = atoi(argv[2]);
  int reps = atoi(argv[3]);
  char mode = argv[1][0];

  // Initialize data
  char townSets[size*size];
  for (int i=0; i<size; i++) {
    for (int j=0; j<size; j++) {
      if (rand() < .1*RAND_MAX)
        townSets[i*size+j] = 1;
      else
        townSets[i*size+j] = 0;
    }
    townSets[i*size+i] = 1;
  }
  int pops[size];
  int totalPop = 0;
  for (int i=0; i<size; i++) {
    pops[i] = rand() % 256;
    totalPop += pops[i];
  }

  // Perform the tests
  int rounds, totalRounds;
  Stopwatch timer;

  // Copy the data to the GPU
  char *townSets_gpu;
  int *pops_gpu;
  char *current_gpu;  // Stores working townset
  cudaMalloc((void**) &townSets_gpu, size*size);
  cudaMemcpy(townSets_gpu, townSets, size*size, cudaMemcpyHostToDevice);
  cudaMalloc((void**) &pops_gpu, size*sizeof(int));
  cudaMemcpy(pops_gpu, pops, size*sizeof(int), cudaMemcpyHostToDevice);
  cudaMalloc((void**) &current_gpu, size);
  cudaMalloc((void**) &sizes_gpu, size);
  cudaMalloc((void**) &subtotals_gpu, size/BLOCKSIZE);
  cudaMalloc((void**) &total_gpu, 1);

  // Do the simulation
  switch (mode) {
    case 'a':
      totalRounds=0;
      for (int r=0; r<reps; r++) {
        createTownSet(current_gpu, size);
        rounds = 0;
        while (sizeTownSet(current_gpu, pops_gpu, size) <= totalPop/2) {
          unionTownSet(current_gpu, townSets_gpu, rand()%size, size);
          rounds++;
        }
        totalRounds += rounds;
      }
      cout << "Average number of rounds = " << ((float) totalRounds/reps) << endl;
      break;
    case 'u':
      createTownSet(current_gpu, size);
      for (int r=0; r<reps; r++)
        unionTownSet(current_gpu, townSets_gpu, rand()%size, size);
      break;
    case 's':
      int s;
      for (int r=0; r<reps; r++)
        s = sizeTownSet(current_gpu, pops_gpu, size);
      break;
  }
  cout << "Elapsed time " << timer.elapsed() << endl;
}
