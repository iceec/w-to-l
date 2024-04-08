#include "data_stream.h"
#include "functional.h"
#include <iostream>
using std::cout;
using std::endl;
using namespace Yu::rpc;
using namespace Yu::serialize;
void test(int x, int y)
{
    cout << x << " " << y << endl;
}
int test1(int a,int b)
{
    return a+b;
}

std::string test2(string &x)
{
    return x+"po";
}
int main()
{
    Function x;

    x.bind("test", test);
      x.bind("test1", test1);
        x.bind("test2", test2);

    data_stream y;
    y << "ll";

    data_stream xx;
 
  

      x.call("test2", y, xx);
      xx.show();

      //xx.show();
    // data_stream yy;
    // yy<<"kkkk";
    //   x.call("test2", yy, xx);
  
}
