#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include<memory>
#include <sstream>
using namespace std;


//优化 赋值啥的 
//测试数字是否成功
//union  因为空间的重合 所以不能轻易的清零
namespace Yu
{
    namespace json
    {

        class json
        {

        public:
            enum Type
            {
                json_null = 0,
                json_int,
                json_double,
                json_string,
                json_array,
                json_obj,
                json_bool
            };
            using array_iterator = vector<json>::iterator;

            json() : m_type(json_null) {}
            json(const int &p);
            json(const double &p);
            json(const bool p);
            json(const char *p);
            json(const string &p);
            json(const json &p);
            json(json &&p); // 把之前指针变成0即可
            json(const vector<json> &p);
            json(const unordered_map<string, json> &p);
            json(string &&p);
            json(vector<json> &&p);
            json(unordered_map<string, json> &&p);
            ~json();

            string str() const;

            friend ostream &operator<<(ostream &f, const json &p);

            json &operator=(const json &p);
            json &operator=(json &&p);

            bool has(int index) const;
            json &get(int index);
            json get(int index) const;
            void remove(int index);
            json &operator[](int index);
            json operator[](int index) const;
            void append(json &&p);
            void append(const json &p);
            array_iterator abegin();
            array_iterator aend();

            bool has(string &s) const;
            json &get(string &s);
            json get(string &s) const;
            json &operator[](string &s);
            json operator[](string &s) const;

            bool has(const char *s) const;
            json &get(const char *s);
            json get(const char *s) const;
            json &operator[](const char *s);
            json operator[](const char *s) const;

            void append(const pair<string, json> &p);
            void append(pair<string, json> &&p);
            void remove(string &s);
            // using map_iterator=unordered_map<string,json>::iterator;

        private:
            void clear();
            union
            {
                int m_int;
                double m_double;
                std::unique_ptr<string> m_string{nullptr};
                bool m_bool;
                std::unique_ptr<vector<json>> m_array;
                std::unique_ptr<unordered_map<string,json>> m_obj;
            };
            Type m_type;
        };
    }

}