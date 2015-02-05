#include "Matrix.h"

Matrix::Matrix(int s) : _size(s), _data(s*s) {
}

float& Matrix::element(int r, int c) {
 return _data[c*_size+r]; 
}
