#include"data_stream.h"
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

