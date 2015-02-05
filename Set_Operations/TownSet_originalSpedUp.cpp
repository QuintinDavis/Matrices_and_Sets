#include <bitset>
#include <thread>
#include <iostream>
#include <cstdlib>
#include <sys/time.h>
#include <vector>
using namespace std;

const int size = 1024;

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
void createTownSet(bitset<size> &s, int n) {
  s=0;
}

// Take a union of two TownSets, result stored in s1
void unionTownSet(bitset<size> &s1, bitset<size> &s2, int n) {
    s1 = s1 | s2;
}

// Find the total population in all of the town sets 
int sizeTownSet(bitset<size> &s1, bitset<size> *pop, int n) {
  int num=0;
  for (int i=0; i<8; i++){
    num+=(s1&pop[i]).count()<<i;
  } 
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

void caseA(int sz, bitset<size> *popsNew, int totalPop, bitset<size> *townSetsCompact, int* totalR, int reps){
  bitset<size> current;
  int rounds, totalRounds;
  totalRounds=0;
  for (int r=0; r<reps; r++) {
    createTownSet(current, size);
    rounds = 0;
    while (sizeTownSet(current, popsNew, size) <= totalPop/2) {
      unionTownSet(current, townSetsCompact[rand()%size], size);
      rounds++;
    }
    totalRounds += rounds;
  }
  *totalR=totalRounds;
}

int main(int argc, const char **argv) {
  if (argc != 5) {
    cout << "Usage: " << argv[0] << " mode size reps threads" << endl;
    cout << "  Mode is one of the following:" << endl;
    cout << "    a  test full algorithm" << endl;
    cout << "    u  test taking unions" << endl;
    cout << "    s  test calculating sizes" << endl;
    exit(0);
  }
  //size = atoi(argv[2]);
  int reps = atoi(argv[3]);
  const int numThreads = atoi(argv[4]);
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

  // Reformat data
  bitset<size> townSetsCompact[size];
  for (int i=0; i<size; i++) {
    for (int j=0; j<size; j++) {
      if (townSets[i][j])
        (townSetsCompact[i])[size-1-j]=1;
    }
    (townSetsCompact[i])[size-1-i]=0;
  }

  bitset<size> popsNew[8];
  for(int i=0; i<size;i++){
    for(int power=0;power<8;power++){
      (popsNew[power])[size-1-i]=1&(pops[i]>>power);
    }
  }

  // Perform the tests
  Stopwatch timer;
  bitset<size> current;
  int totalRounds=0;
  int totalsVect[numThreads];
  std::vector<std::thread> threads;
  switch (mode) {
    case 'a':
      for(int i=0;i<numThreads; i++){
	threads.push_back(thread(caseA, size, popsNew, totalPop, townSetsCompact, &totalsVect[i], reps/numThreads));
      }
      for(int i=0;i<numThreads; i++){
	threads[i].join();
      }
      for(int i=0;i<numThreads; i++){
	totalRounds+=totalsVect[i];
	if(i==numThreads-1){
	  cout << "Average number of rounds = " << ((float) totalRounds/reps) << endl;
	}
      }
      
      break;
    case 'u':
      
      createTownSet(current, size);
      for (int r=0; r<reps; r++)
        unionTownSet(current, townSetsCompact[rand()%size], size);
      break;
    case 's':
      int s;
      for (int r=0; r<reps; r++)
        s = sizeTownSet(townSetsCompact[rand()%size], popsNew, size);
      break;
  }
  cout << "Elapsed time " << timer.elapsed() << endl;
}
