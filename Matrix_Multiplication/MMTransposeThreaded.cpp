#include "Matrix.h"
#include <thread>
#include <vector>

void loops(int tID, int s, Matrix a, Matrix d, Matrix c){
  int i,j,k;
  for (i=i*s/4; i<(i+1)*s/4; i++)
    for (j=0; j<s; j++) {
	  c.element(i,j) = 0;
      for (k=0; k<s; k++)
        c.element(i,j) += a.element(i,k)*d.element(j,k);
   }
}

void matrixMultiply(Matrix& a,Matrix& b, Matrix& c) {
  int s = a.size();
  Matrix d(s);
  
  for (int i=0; i<s; i++)
    for (int j=0; j<s; j++)
      d.element(i,j) = b.element(j,i);
  vector<thread> threads;
  for(int i=0; i<4;i++){
    threads.push_back(thread(loops, i, s,a,d,c));
  }
  for(int i=0; i<4;i++){
    threads[i].join();
  }
}


