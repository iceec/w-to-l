#pragma once
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