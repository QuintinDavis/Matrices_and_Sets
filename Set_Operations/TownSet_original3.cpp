#include <iostream>
#include <cstdlib>
#include <sys/time.h>
#include <bitset>
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
void createTownSet(unsigned long long *s, int n) {
  for (int i=0; i<n; i++)
    s[i] = 0;
}

// Take a union of two TownSets, result stored in s1
void unionTownSet(unsigned long long *s1, unsigned long long *s2, int n) {
  for (int i=0; i<n; i++){
    s1[i] = s1[i] | s2[i];
  }
}

// Find the total population in all of the town sets 
int sizeTownSet(unsigned long long *s, int *pop, int n) {
  int num=0;
  for (int i=0; i<n; i++){
    for(int n = 0;n<64;n++){
      if ((s[i] & (0b1<<n))>0){
	num += pop[i*64+n];

      }
      // cout <<s[i]<<endl;
    }
  }
  return num;
}

// Count number of towns in a set
int countTownSet(unsigned long long *s, int n) {
  int count=0;
  for (int i=0; i<n; i++)
    if (s[i] == 1)
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
  int size = atoi(argv[2])/64;
  int reps = atoi(argv[3]);
  char mode = argv[1][0];

  // Initialize data
  unsigned long long townSets[size][size];
  for (int i=0; i<size; i++) {
    for (int j=0; j<size; j++) {
      townSets[i][j]= 0;
      for(int n=0; n<64; n++){
      if (rand() < .1*RAND_MAX)
        townSets[i][j]= townSets[i][j] | (1ULL<<n);

      }
    }
      townSets[i][i] = townSets[i][i] | 1;
  }
  int pops[size*64];
  int totalPop = 0;
  for (int i=0; i<size*64; i++) {
    pops[i] = rand() % 256;
    totalPop += pops[i];
  }

  // Perform the tests
  unsigned long long current[size];
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
