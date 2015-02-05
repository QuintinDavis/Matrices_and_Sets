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
void createTownSet(unsigned long long *s, int n) {
  for (int i=0; i<n; i++)
    s[i] = 0;
}

// Take a union of two TownSets, result stored in s1
void unionTownSet(unsigned long long *s1, unsigned long long *s2, int n) {
  for (int i=0; i<n; i++)
    s1[i] = s1[i] | s2[i];
}

// Find the total population in all of the town sets 
int sizeTownSet(unsigned long long *s, int *pop, int size) {
  int num=0;
  for (int i=0; i<size; i++){
    for(int n = 0;n<(8*sizeof(unsigned long long));n++){
      if ((s[i] & (0b1<<n))>0){
	num += pop[i*(8*sizeof(unsigned long long))+n];
      }
    }
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

  // Overhead to reformat initialized data
  int uLLSize = (8*sizeof(unsigned long long));
  int sizeNew = size / uLLSize;
  unsigned long long townSetsCompact[size][sizeNew];
  for (int i=0; i<size; i++) {
    for (int j=0; j<sizeNew; j++) {
      for(int n=0; n<uLLSize;n++){
	if(townSets[i][j*uLLSize+n]){
	  townSetsCompact[i][j]=townSetsCompact[i][j]|(1ULL<<(uLLSize-n-1));
	}
      }
    }
  }
  cout<< ((townSetsCompact[0][0]>>(uLLSize-1))&1); //sucessfully reformated 2d bool array to 1d "bit" array. Still need to implement the union with bit array and make the int pop array more efficient.
  cout<< (8*sizeof(unsigned long long)) <<endl;
  cout<< ((townSetsCompact[4][8])>>(uLLSize-1-5)&1);
  cout<< townSets[4][8*uLLSize+9] <<endl;

  
  
  // Perform the tests
  unsigned long long current[sizeNew];
  int rounds, totalRounds;
  Stopwatch timer;
  switch (mode) {
    case 'a':
      totalRounds=0;
      for (int r=0; r<reps; r++) {
        createTownSet(current, sizeNew);
        rounds = 0;
        while (sizeTownSet(current, pops, sizeNew) <= totalPop/2) {
          unionTownSet(current, townSetsCompact[rand()%size], sizeNew);
          rounds++;
        }
        totalRounds += rounds;
      }
      cout << "Average number of rounds = " << ((float) totalRounds/reps) << endl;
      break;
    case 'u':
      createTownSet(current, size);
      for (int r=0; r<reps; r++)
        unionTownSet(current, townSetsCompact[rand()%size], sizeNew);
      break;
    case 's':
      int s;
      for (int r=0; r<reps; r++)
        s = sizeTownSet(townSetsCompact[rand()%size], pops, sizeNew);
      break;
  }
  cout << "Elapsed time " << timer.elapsed() << endl;
}
