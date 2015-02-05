
#include <iostream>       // std::cout
#include <string>         // std::string
#include <bitset>         // std::bitset
#include <cstdlib>
#include <sys/time.h>
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
const int size = 10;
int main ()
{
  bitset<size> foo (444);
  bitset<size> foo2 (511);
  bitset<size> foo3 (foo&foo2);
  cout << foo << " has ";
  cout << foo.count() << " ones and ";
  cout << (foo.size()-foo.count()) << " zeros.\n";
  cout << foo2 << " has ";
  cout << foo2.count() << " ones and ";
  cout << (foo2.size()-foo2.count()) << " zeros.\n";
  cout<< sizeof(unsigned long long) << endl;
  cout<< sizeof(foo)*8 << endl;
  cout << foo3 << " has ";
  cout << foo3.count() << " ones and ";
  cout << (foo3.size()-foo3.count()) << " zeros.\n";
  foo2[2]=0;
  cout << foo2 <<endl;
  Stopwatch timer;
  bitset<size> fooFinal;
  for(int n=0; n<10000; n++){
    fooFinal=foo&foo2;
  }
  cout << "Elapsed time " << timer.elapsed() << endl;
  Stopwatch timer2;
  cout << "Elapsed time " << timer2.elapsed() << endl;
  return 0;
}

