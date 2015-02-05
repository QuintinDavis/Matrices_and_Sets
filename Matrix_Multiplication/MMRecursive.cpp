#include "Matrix.h"

void mmRecurse(Matrix& a, Matrix& b, Matrix& c, int s, int iStart, int jStart, int kStart) {
  if (s < 100) {
	int i,j,k;
	int iEnd = iStart+s, jEnd = jStart+s, kEnd = kStart+s;
    for (i=iStart; i<iEnd; i++)
      for (j=jStart; j<jEnd; j++) {
	    c.element(i,j) = 0;
        for (k=kStart; k<kEnd; k++)
          c.element(i,j) += a.element(i,k)*b.element(k,j);
    } 
  }
  else {
	int h = s/2;
	
	mmRecurse(a,b,c,h,iStart,jStart,kStart); 
	mmRecurse(a,b,c,h,iStart+h,jStart,kStart); 
	mmRecurse(a,b,c,h,iStart,jStart+h,kStart); 
	mmRecurse(a,b,c,h,iStart+h,jStart+h,kStart);
	mmRecurse(a,b,c,h,iStart+h,jStart+h,kStart+h); 
	mmRecurse(a,b,c,h,iStart+h,jStart,kStart+h); 
	mmRecurse(a,b,c,h,iStart,jStart,kStart+h);	
	mmRecurse(a,b,c,h,iStart,jStart+h,kStart+h); 	   
  }
}

void matrixMultiply(Matrix& a, Matrix& b, Matrix& c) {
  mmRecurse(a,b,c,a.size(),0,0,0);
}
