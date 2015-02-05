#ifndef __MATRIX_H
#define __MATRIX_H

#include <vector>
#include <cstdlib>
using namespace std;

class Matrix {
  private:
    int _size;
    vector<float> _data;

  public:
    Matrix(int s);
    float& element(int r, int c);
    int size() { return _size; }
    void randomize() { 
	  for (int i=0; i<_size*_size; i++)
        _data[i] = ((float) rand())/RAND_MAX - .5;
    }
};


void matrixMultiply(Matrix& a, Matrix& b, Matrix& c);

#endif
