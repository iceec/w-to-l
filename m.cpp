#include "json.h"
#include "parse.h"
#include<iostream>
using namespace Yu::json;

int main()
{
    ofstream fout;
    parse t;

    t.load("x.json");

    fout.open("out.json");
   fout << t.parsefile();
}