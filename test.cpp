#include<iostream>
#include<functional>
#include<optional>
using namespace std;


template<class R,class ...args>
    void show(function<R(args...)>func, args&& ...x)  //匹配即可
    {
        func(x...);
    }


    void aa()
    {
        cout<<"j"<<endl;
    }
constexpr int f(int x)
{
    int sum=0;
    while(--x>0)
        sum+=x/3;
    return sum;
}
int main()
{
  //optional<void> a;


cout<<f(13);

   // show(function<void(int,int)>([](int a,int b){cout<<"haha"<<a<<b<<endl;}),1,2);

}

//muteable class numvber