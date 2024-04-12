#pragma once
#include<string>
#include<fstream>
#include<sstream>
#include<stdexcept>
#include"json.h"
using namespace std;

namespace Yu{
    namespace json{
        class parse{
            public:
            parse()=default;
            void load(string &filename);
            void load(const char *filename);

            void jump();

            json parsefile();
            json parsestring();
            json parsenumber();
            json parsebool();
            json parsearray();
            json parsenull();
            json parseobj();

            void skipwhite();
            bool iswhite();

            private:
            string m_str;
            size_t m_idx;
            int a=0;

        };
    }
}
