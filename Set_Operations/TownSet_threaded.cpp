
#include <iostream>
#include <cstdlib>
#include <sys/time.h>
#include <thread>
using namespace std;

// Global variable
int numThreads;

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
void createTownSet(bool *s, int n) {
  for (int i=0; i<n; i++)
    s[i] = false;
}

// Take a union of two TownSets, result stored in s1
void unionTownSetInternal(bool *s1, bool *s2, int n) {
  for (int i=0; i<n; i++)
    s1[i] = s1[i] || s2[i];
}

void unionTownSet(bool *s1, bool *s2, int n) {
  thread threads[numThreads];
  for (int i=0; i<numThreads; i++) {
    int start = n*i/numThreads;
    int end = n*(i+1)/numThreads;
    threads[i] = thread(unionTownSetInternal, s1+start, s2+start, end-start);
  }
  for (int i=0; i<numThreads; i++)
    threads[i].join();
}

// Find the total population in all of the town sets 
void sizeTownSetInternal(bool *s, int *pop, int n, int *num) {
  int sum=0;
  for (int i=0; i<n; i++)
    if (s[i] == true)
      sum += pop[i];
  *num = sum;
}

int sizeTownSet(bool *s, int *pop, int n) {
  int num=0;
  int nums[numThreads];
  thread threads[numThreads];
  for (int i=0; i<numThreads; i++) {
    int start = n*i/numThreads;
    int end = n*(i+1)/numThreads;
    threads[i] = thread(sizeTownSetInternal, s+start, pop+start, end-start, &(nums[i]));
  }
  for (int i=0; i<numThreads; i++)
    threads[i].join();
  for (int i=0; i<numThreads; i++)
    num += nums[i];
  return num;
}

// Count number of towns in a set
void countTownSetInternal(bool *s, int n, int *count) {
  int c=0;
  for (int i=0; i<n; i++)
    if (s[i] == true)
      c++;
  *count = c;
}

int countTownSet(bool *s, int n) {
  int count=0;
  int counts[numThreads];
  thread threads[numThreads];
  for (int i=0; i<numThreads; i++) {
    int start = n*i/numThreads;
    int end = n*(i+1)/numThreads;
    threads[i] = thread(countTownSetInternal, s+start, end-start, &(counts[i]));
  }
  for (int i=0; i<numThreads; i++)
    threads[i].join();
  for (int i=0; i<numThreads; i++)
    count += counts[i];
  return count;
}

int main(int argc, char **argv) {
  if (argc != 5) {
    cout << "Usage: " << argv[0] << " mode size reps threads" << endl;
    cout << "  Mode is one of the following:" << endl;
    cout << "    a  test full algorithm" << endl;
    cout << "    u  test taking unions" << endl;
    cout << "    s  test calculating sizes" << endl;
    exit(0);
  }
  int size = atoi(argv[2]);
  int reps = atoi(argv[3]);
  char mode = argv[1][0];
  numThreads = atoi(argv[4]);

  // Initialize data
  bool townSets[size][size];
  for (int i=0; i<size; i++) {
    for (int j=0; j<size; j++) {
      if (rand() < .1*RAND_MAX)
        townSets[i][j] = true;
      else
        townSets[i][j] = false;
    }
    townSets[i][i] = true;
  }
  int pops[size];
  int totalPop = 0;
  for (int i=0; i<size; i++) {
    pops[i] = rand() % 256;
    totalPop += pops[i];
  }

  // Perform the tests
  bool current[size];
  int rounds, totalRounds;
  Stopwatch timer;
  switch (mode) {
    case 'a':
      totalRounds=0;
      for (int r=0; r<reps; r++) {
        createTownSet(current, size);
        rounds = 0;
        while (sizeTownSet(current, pops, size) <= totalPop/2) {
          unionTownSet(current, townSets[rand()%size], size);
          rounds++;
        }
        totalRounds += rounds;
      }
      cout << "Average number of rounds = " << ((float) totalRounds/reps) << endl;
      break;
    case 'u':
      createTownSet(current, size);
      for (int r=0; r<reps; r++)
        unionTownSet(current, townSets[rand()%size], size);
      break;
    case 's':
      int s;
      for (int r=0; r<reps; r++)
        s = sizeTownSet(townSets[rand()%size], pops, size);
      break;
  }
  cout << "Elapsed time " << timer.elapsed() << endl;
}
