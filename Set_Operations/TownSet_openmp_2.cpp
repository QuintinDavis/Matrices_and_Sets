
#include <iostream>
#include <cstdlib>
#include <sys/time.h>
using namespace std;

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
void unionTownSet(bool *s1, bool *s2, int n) {
  for (int i=0; i<n; i++)
    s1[i] = s1[i] || s2[i];
}

// Find the total population in all of the town sets 
int sizeTownSet(bool *s, int *pop, int n) {
  int num=0;
  for (int i=0; i<n; i++)
    if (s[i] == true)
      num += pop[i];
  return num;
}

// Count number of towns in a set
int countTownSet(bool *s, int n) {
  int count=0;
  for (int i=0; i<n; i++)
    if (s[i] == true)
      count++;
  return count;
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
#pragma omp parallel for reduction(+:totalRounds) private(current, rounds)
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
