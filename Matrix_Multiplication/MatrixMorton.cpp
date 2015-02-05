#include "Matrix.h"

static const unsigned int B[] = {0x55555555, 0x33333333, 0x0F0F0F0F, 0x00FF00FF};
static const unsigned int S[] = {1, 2, 4, 8};

long maxIndex(long s) {
  long r=s, c=s;
  r = (r | (r << S[3])) & B[3];
  r = (r | (r << S[2])) & B[2];
  r = (r | (r << S[1])) & B[1];
  r = (r | (r << S[0])) & B[0];

  c = (c | (c << S[3])) & B[3];
  c = (c | (c << S[2])) & B[2];
  c = (c | (c << S[1])) & B[1];
  c = (c | (c << S[0])) & B[0];

  return  r | (c << 1);
}

Matrix::Matrix(int s) : _size(s), _data(maxIndex(s)+1) {
}

float& Matrix::element(int r, int c) { 
  r = (r | (r << S[3])) & B[3];
  r = (r | (r << S[2])) & B[2];
  r = (r | (r << S[1])) & B[1];
  r = (r | (r << S[0])) & B[0];

  c = (c | (c << S[3])) & B[3];
  c = (c | (c << S[2])) & B[2];
  c = (c | (c << S[1])) & B[1];
  c = (c | (c << S[0])) & B[0];

  return  _data[r | (c << 1)];
}
