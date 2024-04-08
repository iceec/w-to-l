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

int main()
{
  optional<void> a;




    show(function<void(int,int)>([](int a,int b){cout<<"haha"<<a<<b<<endl;}),1,2);

}