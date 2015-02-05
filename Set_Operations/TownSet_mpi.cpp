#include <iostream>
#include <cstdlib>
#include <sys/time.h>
#include <mpi/mpi.h>
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
void createTownSet(char *s, int n) {
  for (int i=0; i<n; i++)
    s[i] = false;
}

// Take a union of two TownSets, result stored in s1
void unionTownSet(char *s1, char *s2, int n) {
  for (int i=0; i<n; i++)
    s1[i] = s1[i] || s2[i];
}

// Find the total population in all of the town sets 
int sizeTownSet(char *s, int *pop, int n) {
  int num=0;
  for (int i=0; i<n; i++)
    if (s[i] == true)
      num += pop[i];
  return num;
}

// Count number of towns in a set
int countTownSet(char *s, int n) {
  int count=0;
  for (int i=0; i<n; i++)
    if (s[i] == true)
      count++;
  return count;
}

int main(int argc, char **argv) {
  int numProc, procID;
  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD,&numProc);
  MPI_Comm_rank(MPI_COMM_WORLD,&procID);
  
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
  char current[size];
  int rounds, totalRounds;
  Stopwatch timer;
  
  // Share data
  MPI_Bcast (&townSets,size*size,MPI_CHAR,0,MPI_COMM_WORLD); 
  MPI_Bcast (&pops,size,MPI_INTEGER,0,MPI_COMM_WORLD); 
  
  switch (mode) {
    case 'a':
      totalRounds=0;
      for (int r=0; r<reps/numProc; r++) {
        createTownSet(current, size);
        rounds = 0;
        while (sizeTownSet(current, pops, size) <= totalPop/2) {
          unionTownSet(current, &(townSets[size*(rand()%size)]), size);
          rounds++;
        }
        totalRounds += rounds;
      }
      break;
    case 'u':
      createTownSet(current, size);
      for (int r=0; r<reps/numProc; r++)
        unionTownSet(current, &(townSets[size*(rand()%size)]), size);
      break;
    case 's':
      int s;
      for (int r=0; r<reps/numProc; r++)
        s = sizeTownSet(&(townSets[size*(rand()%size)]), pops, size);
      break;
  }
  
  MPI_Finalize();
  
  if (procID == 0)
    cout << "Elapsed time " << timer.elapsed() << endl;
}
