
#include<string>
#include<vector>
#include<list>
#include<unordered_map>
#include<set>
#include<stdexcept>
using namespace std;

namespace Yu{
    namespace serialize{

    //void shton(const char *p,size_t len);
   //void sntoh(const char *p,size_t len);
   // bool is_big();
        class data_stream{
            public:
                enum{
                    data_bool=0,
                    data_char,
                    data_int,
                    data_int64,
                    data_float,
                    data_double,
                    data_string,
                    data_vector,
                    data_list,
                    data_map,
                    data_set,
                    data_define,
                };
                void write(int value);
                void write(char value);
                void write(int64_t value);
                void write(float value);
                void write(double value);
                void write(bool value);
                void write(const char * value);
                void write(const string & value);
                void write (const char * p,size_t len);

                template<class T>
                    void write(const vector<T> & p);
                template<class T>
                    void write(const list<T> & p);
                template<class T,class V>
                    void write(const unordered_map<T,V> & p);
                template<class T>
                    void write(const set<T> & p);
                

                template<class T,class ...V>
                    void write_args(const T & p,const V &...args);
                void write_args(){}

                void reseverse(size_t len);

                void read(bool &value);
                void read(char &value);
                void read(int &value);
                void read(int64_t &value);
                void read(float &value);
                void read(double &value);
                void read(char * value);
                void read(string &value);
                void read(char * const value,size_t len);
                void read_args(){}
                template<class T,class ...V>
                    void read_args( T & p, V &...args);

                template<class T>
                    void read(vector<T> & p);
                 template<class T>
                    void read(list<T> & p);
                template<class T,class V>
                    void read(unordered_map<T,V> & p);
                template<class T>
                    void read(set<T> & p);





                template<class T>
                    friend data_stream & operator <<(data_stream & p,T value);
                  template<class T>
                    friend data_stream & operator >>(data_stream & p,T & value);

                void show();
                data_stream(){
                    m_buf.clear();
                    m_pos=0;
                }
            private:
                vector<char> m_buf;
                size_t m_pos;
                
        };

       template<class T>
        data_stream & operator <<(data_stream & p,T value){
            p.write(value);
            return p;
        }
        template<class T>
        data_stream & operator >>(data_stream & p,T & value){
            p.read(value);
            return p;
        }

        template<class T,class ...V>
            void data_stream::write_args(const T & p,const V &...args){
                write(p);
                write_args(args...);
        }

          template<class T,class ...V>
                    void data_stream::read_args( T & p, V &...args){
                        read(p);
                        read(args...);
                    }
        
        template<class T>
            void data_stream::write(const vector<T> & p){
                char type=data_vector;
                write(&type,sizeof(type));
                int len=p.size();
                write(len);
                for(int i=0;i<len;i++)
                    write(p[i]);
            }

            template<class T>
                void data_stream::write(const list<T> & p){
                       char type=data_list;
                write(&type,sizeof(type));
                int len=p.size();
                write(len);
                for(auto i=p.begin();i!=p.end();i++)
                    write(*i);
            }

            template<class T,class V>
                    void data_stream::write(const unordered_map<T,V> & p){
                        char type=data_map;
                    write(&type,sizeof(type));
                        int len=p.size();
                        write(len);
                        for(auto i=p.begin();i!=p.end();i++){
                            write(i->first);
                            write(i->second);
                        }
                    }
            template<class T>
                    void data_stream::write(const set<T> & p){
                        char type=data_set;
                        write(&type,sizeof(type));
                        int len=p.size();
                        write(len);
                         for(auto i=p.begin();i!=p.end();i++){
                            write(*i);
                        }
                    }
            
        
         template<class T>
                void data_stream::read(vector<T> & p){
                    if(m_buf[m_pos]!=data_vector)
                        throw logic_error("vector format wrong");
                    p.clear();
                    ++m_pos;
                    int len;
                    read(len);
                    p.resize(len);
                    for(int i=0;i<len;i++)
                        read(p[i]);
                }
        template<class T>
                void data_stream:: read(list<T> & p){
                        if(m_buf[m_pos]!=data_list)
                            throw logic_error("list format wrong");
                    p.clear();
                    ++m_pos;
                    int len;
                    read(len);
                    T temp;
                    for(int i=0;i<len;i++){
                        read(temp);
                        p.push_back(temp);
                    }
                }
            template<class T,class V>
                void data_stream:: read(unordered_map<T,V> & p){
                    if(m_buf[m_pos]!=data_map)
                            throw logic_error("map format wrong");
                    p.clear();
                    ++m_pos;
                    int len;
                    read(len);
                    T temp1; V temp2;
                    for(int i=0;i<len;i++){
                        read(temp1);
                        read(temp2);
                        p[temp1]=temp2;
                    } 
                }

            template<class T>
                void data_stream:: read(set<T> & p){
                     if(m_buf[m_pos]!=data_map)
                            throw logic_error("set format wrong");
                    p.clear();
                    ++m_pos;
                    int len;
                    read(len);
                    T temp;
                    for(int i=0;i<len;i++){
                        read(temp);
                        p.insert(temp);
                    }
                }
}
}

#include<stdexcept>
#include<string.h>
#include<wchar.h>
#include<iostream>
namespace Yu::serialize{
    void data_stream::write (const char * p,size_t len){
        //首先 转化为大端,然后存储
        //shton(p,len);
        reseverse(len);
        size_t before=m_buf.size();
        m_buf.resize(m_buf.size()+len);
        memcpy(&m_buf[before],p,len);
}

void data_stream::reseverse(size_t len){
    size_t temp=len+m_buf.size();
    if(temp>m_buf.capacity())
        m_buf.reserve(2*temp);
}

void data_stream::write(bool value){
    char type=data_bool;
    write(&type,sizeof(type));
    write((char *)&value,sizeof(value));
}

void data_stream::write(char value){
    char type=data_char;
    write(&type,sizeof(type));
    write((char *)&value,sizeof(value));
}


void data_stream::write(int value){
    char type=data_int;
    write(&type,sizeof(type));
    write((char *)&value,sizeof(value));
}

void data_stream::write(float value){
    char type=data_float;
    write(&type,sizeof(type));
    write((char *)&value,sizeof(value));
}

void data_stream::write(double value){
    char type=data_double;
    write(&type,sizeof(type));
    write((char *)&value,sizeof(value));
}

void data_stream::write(int64_t value){
    char type=data_int64;
    write(&type,sizeof(type));
    write((char *)&value,sizeof(value));
}

void data_stream::write(const char * value){
    const char type=data_string;
    write(&type,sizeof(type));
    int32_t len=strlen(value);
    write((int32_t)len);
    write(value,len);
}

void data_stream::write(const string & value){
    write(value.c_str());
}

void data_stream::read(char * const value,size_t len){
    memcpy(value,&m_buf[m_pos],len);
    m_pos+=len;
}

void data_stream::read(bool &value){
     if(m_buf.at(m_pos)==data_bool){
        ++m_pos;
        value=m_buf.at(m_pos++);
     }
     else
        throw logic_error("bool format wrong");
}

void data_stream::read(char &value){
  if(m_buf.at(m_pos)==data_char){
        ++m_pos;
        value=m_buf.at(m_pos++);
     }
     else
        throw logic_error("char format wrong");
}

void data_stream::read(int &value){
     if(m_buf.at(m_pos)==data_int){
        ++m_pos;
        value=*(reinterpret_cast<int *>(&m_buf[m_pos]));
        m_pos+=4;
     }
     else
        throw logic_error("int format wrong");
}

void data_stream::read(int64_t &value){
      if(m_buf.at(m_pos)==data_int64){
        ++m_pos;
        value=*(reinterpret_cast<int64_t *>(&m_buf[m_pos]));
        m_pos+=8;
     }
     else
        throw logic_error("int64_t format wrong");

}

void data_stream::read(float &value){
         if(m_buf.at(m_pos)==data_float){
        ++m_pos;
        value=*(reinterpret_cast<float *>(&m_buf[m_pos]));
        m_pos+=4;
     }
     else
        throw logic_error("float format wrong");
}

void data_stream::read(double &value){
         if(m_buf.at(m_pos)==data_double){
        ++m_pos;
        value=*(reinterpret_cast<double *>(&m_buf[m_pos]));
        m_pos+=8;
     }
     else
        throw logic_error("double format wrong");
}

void data_stream::read(char *value){
         if(m_buf.at(m_pos)==data_string){
        ++m_pos;
        int p;
        read(p);
        memcpy(value,&(m_buf[m_pos]),p);
        m_pos+=p;
     }
     else
        throw logic_error("char * format wrong");
}

void data_stream::read(string &value){
     if(m_buf.at(m_pos)==data_string){
        ++m_pos;
        int p;
        read(p);
       value.assign(&(m_buf[m_pos]),p);
       m_pos+=p;
     }
     else
        throw logic_error("string format wrong");

}



void data_stream::show(){
    size_t i=0;
    int p;
    while(i<m_buf.size()){
        switch (m_buf[i])
        {
        case data_bool:
            ++i;
            if(m_buf.at(i)!=0)
                cout<<"True ";
            else
                cout<<"False ";
            ++i;
            break;
        case data_char:
            ++i;
            cout<<m_buf.at(i)<<" ";
            ++i;
            break;
        case data_int:
            ++i;
            cout<<*(reinterpret_cast<int *>(&m_buf[i]))<<" ";
            i+=4;
            break;
        case data_int64:
            ++i;
            cout<<*(reinterpret_cast<int64_t *>(&m_buf[i]))<<" ";
            i+=8;
            break;
        case data_float:
            ++i;
            cout<<*(reinterpret_cast<float *>(&m_buf[i]))<<" ";
            i+=4;
            break;
        case data_double:
            ++i;
            cout<<*(reinterpret_cast<double *>(&m_buf[i]))<<" ";
            i+=8;
            break;
        case data_string:
            ++i;
            if(m_buf[i]!=data_int)
                throw logic_error("string type wrong");
            ++i;
            p=*(reinterpret_cast<int *>(&m_buf[i]));
            i+=4;
            cout<<string(&m_buf[i],p)<<" ";
            i+=p;
            break;
        case data_vector:
            ++i;
            if(m_buf[i]!=data_vector)
                throw logic_error("vector type wrong");
            ++i;
            p=*(reinterpret_cast<int *>(&m_buf[i]));
            i+=4;
        default:
            throw logic_error("type wrong");
            break;
        }
    }

    cout<<endl;
}






bool isbig(){

    int x=1;
    char *p=(char *)&x;

    if(p[0]==1)
        return 0;
    if(p[sizeof(int)-1]==1)
        return 1;
    throw logic_error("isbig wrong"); 
}

}



#include <unordered_map>
#include <optional>
#include <string>
#include <functional>
using namespace Yu::serialize;

namespace Yu
{
    namespace rpc
    {
        class Function
        {
            public:
            Function() = default;
            ~Function() = default;
            template <class F>
            void bind(const string &func_name, F func); // F 大概率是一个函数指针 /是一个实体对象 实体对象保证可复制
            void call(const string &func_name, data_stream &in, data_stream &out);
            template <class F>
            void wrap(F func, data_stream &in, data_stream &out);
            template <class R, class... Args>
            void wrap_impl(R (*func)(Args...), data_stream &in, data_stream &out);
            template <class R, class... Args>
            void wrap_impl(std::function<R(Args...)> func, data_stream &in, data_stream &out);
            template <class T, std::size_t... I>
            T get_args(data_stream &in, std::index_sequence<I...>);
            template <class T, std::size_t id>
            void get_args(data_stream &in, T &t);
            template <class R, class F, class T>
            typename std::enable_if<std::is_same<R, void>::value, int32_t>::type
            call_impl(F func, T args);
            template <class R, class F, class T>
            typename std::enable_if<!std::is_same<R, void>::value, R>::type
            call_impl(F func, T args);

            template <class R, class F, class T, size_t... I>
            auto invoke(F func, T args, std::index_sequence<I...>);

        private:
            std::unordered_map<std::string, std::function<void(data_stream &, data_stream &)>> m_handler;
        };

        template <class F>
        void Function::bind(const string &func_name, F func)
        {
            m_handler[func_name] = std::bind(&Function::wrap<F>, this, func, std::placeholders::_1, std::placeholders::_2);
        }

        template <class F>
        void Function::wrap(F func, data_stream &in, data_stream &out)
        {
            wrap_impl(func, in, out);
        }

        template <class R, class... Args>
        void Function::wrap_impl(R (*func)(Args...), data_stream &in, data_stream &out)
        {
            wrap_impl(std::function<R(Args...)>(func), in, out);
        }

        template <class R, class... Args>
        void Function::wrap_impl(std::function<R(Args...)> func, data_stream &in, data_stream &out)
        {
            using tuple_type = std::tuple<typename std::decay<Args>::type...>;
            constexpr auto size = std::tuple_size<typename std::decay<tuple_type>::type>::value;
            tuple_type args = get_args<tuple_type>(in, make_index_sequence<size>{});
            if (std::is_same<R, void>::value)
            {
                call_impl<R>(func, args);
                return;
            }
            auto p = call_impl<R>(func, args);
            out << p;
            // back
        }

        template <class T, std::size_t... I>
        T Function::get_args(data_stream &in, std::index_sequence<I...>)
        {
            T t;
            std::initializer_list<int>{(get_args<T, I>(in, t), 0)...};
            return t;
        }
        template <class T, std::size_t id>
        void Function::get_args(data_stream &in, T &t)
        {
            in >> std::get<id>(t);
        }

        template <class R, class F, class T>
        typename std::enable_if<std::is_same<R, void>::value, int32_t>::type
        Function::call_impl(F func, T args)
        {
            constexpr auto size = std::tuple_size<typename std::decay<T>::type>::value;

            invoke<R>(func, args, std::make_index_sequence<size>{});
            return 0;
        }

        template <class R, class F, class T>
        typename std::enable_if<!std::is_same<R, void>::value, R>::type
       Function:: call_impl(F func, T args)
        {
            constexpr auto size = std::tuple_size<typename std::decay<T>::type>::value;

            return invoke<R>(func, args, std::make_index_sequence<size>{});
        }

        template <class R, class F, class T, size_t... I>
        auto Function::invoke(F func, T args, std::index_sequence<I...>)
        {
            return func(std::get<I>(args)...);
        }

        inline void Function::call(const string &func_name, data_stream &in, data_stream &out)
        {
            if (m_handler.count(func_name))
                m_handler[func_name](in, out);
            return;
        }
    }
}
using namespace Yu::rpc;
using namespace Yu::serialize;

int test(int x, int y)
{
    cout << x << " " << y << endl;
    return x+y;
}
int main()
{
    Function x;

    x.bind("test", test);

    data_stream y;
    y << 1 << 2;

    data_stream xx;

    x.call("test", y, xx);
}