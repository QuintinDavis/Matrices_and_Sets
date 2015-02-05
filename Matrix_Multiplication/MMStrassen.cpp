#include "Matrix.h"

void mmStrassen(Matrix& a, Matrix& b, Matrix& c, int depth, vector<Matrix *>& temps) {
  int s = a.size();
  if (s <= 64) {
	int i,j,k;
    for (i=0; i<s; i++)
      for (j=0; j<s; j++) {
	    c.element(i,j) = 0;
        for (k=0; k<s; k++)
          c.element(i,j) += a.element(i,k)*b.element(k,j);
    } 
  }
  else {
	int h = s/2;
    depth++;
    
    while (temps.size() < 9*depth)
        temps.push_back(new Matrix(h));
     	   
    Matrix *temp1 = temps[9*depth-9];
    Matrix *temp2 = temps[9*depth-8];
    Matrix *m1 = temps[9*depth-7];
    Matrix *m2 = temps[9*depth-6];
    Matrix *m3 = temps[9*depth-5];
    Matrix *m4 = temps[9*depth-4];
    Matrix *m5 = temps[9*depth-3];
    Matrix *m6 = temps[9*depth-2];
    Matrix *m7 = temps[9*depth-1];
    
    for (int i=0; i<h; i++)
      for (int j=0; j<h; j++)
        temp1->element(i,j) = a.element(i,j) + a.element(h+i,h+j);
    for (int i=0; i<h; i++)
      for (int j=0; j<h; j++)
        temp2->element(i,j) = b.element(i,j) + b.element(h+i,h+j);
    mmStrassen(*temp1, *temp2, *m1, depth, temps);
    
    for (int i=0; i<h; i++)
      for (int j=0; j<h; j++)
        temp1->element(i,j) = a.element(h+i,j) + a.element(h+i,h+j);
    for (int i=0; i<h; i++)
      for (int j=0; j<h; j++)
        temp2->element(i,j) = b.element(i,j);
    mmStrassen(*temp1, *temp2, *m2, depth, temps);
    
    for (int i=0; i<h; i++)
      for (int j=0; j<h; j++)
        temp1->element(i,j) = a.element(i,j);
    for (int i=0; i<h; i++)
      for (int j=0; j<h; j++)
        temp2->element(i,j) = b.element(i,h+j) - b.element(h+i,h+j);
    mmStrassen(*temp1, *temp2, *m3, depth, temps);
    
    for (int i=0; i<h; i++)
      for (int j=0; j<h; j++)
        temp1->element(i,j) = a.element(h+i,h+j);
    for (int i=0; i<h; i++)
      for (int j=0; j<h; j++)
        temp2->element(i,j) = b.element(h+i,h) - b.element(i,j);
    mmStrassen(*temp1, *temp2, *m4, depth, temps);
    
    for (int i=0; i<h; i++)
      for (int j=0; j<h; j++)
        temp1->element(i,j) = a.element(i,j) + a.element(i,h+j);
    for (int i=0; i<h; i++)
      for (int j=0; j<h; j++)
        temp2->element(i,j) = b.element(h+i,h+j);
    mmStrassen(*temp1, *temp2, *m5, depth, temps);
    
    for (int i=0; i<h; i++)
      for (int j=0; j<h; j++)
        temp1->element(i,j) = a.element(h+i,j) - a.element(i,h);
    for (int i=0; i<h; i++)
      for (int j=0; j<h; j++)
        temp2->element(i,j) = b.element(i,j) + b.element(i,h+j);
    mmStrassen(*temp1, *temp2, *m6, depth, temps);
    
    for (int i=0; i<h; i++)
      for (int j=0; j<h; j++)
        temp1->element(i,j) = a.element(i,h+j) - a.element(h+i,h+j);
    for (int i=0; i<h; i++)
      for (int j=0; j<h; j++)
        temp2->element(i,j) = b.element(h+i,j) + b.element(h+i,h+j);
    mmStrassen(*temp1, *temp2, *m7, depth, temps);
    
    for (int i=0; i<h; i++)
      for (int j=0; j<h; j++)
        c.element(i,j) = m1->element(i,j) + m4->element(i,j) - m5->element(i,j) + m7->element(i,j);
    for (int i=0; i<h; i++)
      for (int j=0; j<h; j++)
        c.element(i,h+j) = m3->element(i,j) + m5->element(i,j);
    for (int i=0; i<h; i++)
      for (int j=0; j<h; j++)
        c.element(h+i,j) = m2->element(i,j) + m4->element(i,j);
    for (int i=0; i<h; i++)
      for (int j=0; j<h; j++)
        c.element(i,j) = m1->element(i,j) - m2->element(i,j) + m3->element(i,j) + m6->element(i,j);
  }
}

void matrixMultiply(Matrix& a, Matrix& b, Matrix& c) { 
  vector<Matrix *> temps; 
  mmStrassen(a,b,c,0,temps);
  for (vector<Matrix*>::iterator iter = temps.begin(); iter != temps.end(); ++iter)
    delete *iter;
}
