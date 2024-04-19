// 单例模式 可以是 static 函数里面有东西
#pragma once
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;
// 这样模板类可以访问你的私有函数
// #define  \后面不能有空格

namespace Yu
{
    namespace reflect
    {
#define SINGLE(a)          \
    friend single<a>;      \
                           \
private:                   \
    a() = default;         \
    a(const a &) = delete; \
    ~a() = default;        \
    a &operator=(const a &) = delete;

        template <class T>
        class single
        {
        public:
            static T *instance()
            {
                static T a;
                return &a;
            }
        };

     class Class_field
        {
        public:
            string fieldname;
            string type;
            size_t offset;
            Class_field() {}
            Class_field(const string &a, const string &b, size_t c) : fieldname(a), type(b), offset(c) {}
            string &FieldName() { return fieldname; }
            string &Type() { return type; }
            size_t &Offset() { return offset; }
        };

        class Class_func
        {
        private:
            string func_name;
            void *func_addr;

        public:
            Class_func() {}
            Class_func(const string &name, void *addr) : func_name(name), func_addr(addr) {}
            string name() { return func_name; }
            void *funcaddr() { return func_addr; }
        };

        class Objs
        {
        public:
            Objs() = default;
            virtual ~Objs() = default;
            void setname(const string &p) { my_name = p; }
            template <class T>
            T *getvalue(const string type_name);
            template <class T>
            bool setvalue(const string type_name, const T &val);
            template<class T,class ...V,class M>
                void getfunc(const string func_name,M x, T funtype,V && ...arg);

        private:
            string my_name;
        };
        using create_class = Objs *(*)(void);

       

        class Factory
        {
        public:
            void regist_class(const string &name, create_class func);
            void regist_class_field(const string &name, const string a, const string b, size_t c);
            void regist_class_func(const string &classname, const string funcname, void *funaddr);
            Objs *createclass(const string &name);
            Class_field *find_class_field(const string name, const string type_name);
            Class_func *find_calss_func(const string classname, const string funcname);

        private:
            unordered_map<string, create_class> m_class_map;
            unordered_map<string, vector<Class_field>> m_class_field;
            unordered_map<string, vector<Class_func>> m_class_func;
            SINGLE(Factory)
        };

        template <class T>
        T *Objs::getvalue(const string type_name)
        {
            Class_field *p = single<Factory>::instance()->find_class_field(my_name, type_name);
            if (p == nullptr)
                return nullptr;
            return ((T *)((size_t)this + p->Offset()));
        }

        template <class T>
        bool Objs::setvalue(const string type_name, const T &val)
        {
            T *p = getvalue<T>(type_name);
            if (p == nullptr)
                return false;
            *p = val;
            return true;
        }

        template<class T,class ... V,class M>
            void Objs::getfunc(const string func_name,M x, T funtype,V &&...arg){
                Class_func * p=single<Factory>::instance()->find_calss_func(my_name,func_name);
                if(p==nullptr)
                    return;
             (*(T *)(p->funcaddr()))(x,std::forward<V>(arg)...); //这样原函数左值 =左值引用 右值=右值 左值引用=左值引用
               return;
            }

    }

}