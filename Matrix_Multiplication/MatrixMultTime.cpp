#include <iostream>
#include <cstdlib>
using namespace std;

#include "Stopwatch.h"
#include "Matrix.h"

int main(int argc, char **argv) {
  int size = atoi(argv[1]);
  srand(time(0));
  
  Matrix a(size), b(size), c(size);
  a.randomize();
  b.randomize();
  
  Stopwatch timer;
  matrixMultiply(a,b,c);
  float elapsed = timer.elapsed();
  cout << size << "\t" << elapsed << "\t";
  cout << 1e-6*size*size*size/elapsed << endl;
}
