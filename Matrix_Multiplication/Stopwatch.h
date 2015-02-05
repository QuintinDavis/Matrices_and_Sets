#ifndef __STOPWATCH_H
#define __STOPWATCH_H

#include <sys/time.h>

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

#endif