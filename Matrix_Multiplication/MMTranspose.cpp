#include "Matrix.h"

void matrixMultiply(Matrix& a, Matrix& b, Matrix& c) {
  int s = a.size();
  Matrix d(s);
  
  for (int i=0; i<s; i++)
    for (int j=0; j<s; j++)
      d.element(i,j) = b.element(j,i);
  
  int i,j,k;
  for (i=0; i<s; i++)
    for (j=0; j<s; j++) {
	  c.element(i,j) = 0;
      for (k=0; k<s; k++)
        c.element(i,j) += a.element(i,k)*d.element(j,k);
    }
}
