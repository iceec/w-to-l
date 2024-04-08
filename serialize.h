#pragma once
#include"data_stream.h"
namespace Yu{
    namespace serialize
    {
        class serializ
        {
        private:
          
        public:
        void write_define(data_stream &p)const{
            char type=p.data_define;
            p.write(&type,sizeof(type));
            return;
        }
        bool read_define(data_stream &p){
            char type;
            p.read(&type,sizeof(type));
            return type==p.data_define;
        }
       
        template<class ...T>
            data_stream & userialize(data_stream &p,const T & ...args)const;
         template<class ...T>
            data_stream & deserialize(data_stream &p,T & ...args);
     

        };
     template<class ...T>
            data_stream & serializ::userialize(data_stream &p,const T & ...args)const{
                p.write_args(args...);
                return p;
            }
    template<class ...T>
        data_stream & serializ::deserialize(data_stream &p,T & ...args){
            p.read_args(args...);
            return p;
        }

    #define    DATA_streamIO(...)    \
data_stream & p1(data_stream &p)const{  \
    write_define(p);                             \
    userialize(p,__VA_ARGS__);       \
    return p; \
}\
data_stream & P2(data_stream &p){\
    if(read_define(p)){\
        deserialize(p,__VA_ARGS__);\
    }\
    return p;\
}\
        
    } 
    
}