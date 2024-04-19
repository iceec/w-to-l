#pragma once
#include<string>
#include"class_factory.h"
using namespace std;
namespace Yu{
    namespace reflect{

    
      class Classregist
        {
        public:
            Classregist(const string &name, create_class func){
                 single<Factory>::instance()->regist_class(name,func);
            }
            Classregist(const string &name, const string a, const string b, size_t c){
                single<Factory>::instance()->regist_class_field(name,a,b,c);
            }
            Classregist(const string &classname, const string &funcname,void *funcaddr){
                single<Factory>::instance()->regist_class_func(classname,funcname,funcaddr);
            }
        };

    #define REGISETCLASS(classname)   \
    Objs *createclass##classname()        \
    {                                     \
          Objs *p= new classname(); if(p) p->setname(#classname);   return p; \
    }                                     \
    Classregist classregist##classname(string(#classname), createclass##classname)

    #define REGISETCLASS_FIELD(classname,fieldname,type) \
        classname classname##fieldname; \
        Classregist classregist##classname##fieldname(#classname,#fieldname,#type,(size_t)&classname##fieldname.fieldname-(size_t)&classname##fieldname)
    
    #define REGISETCLASS_FUNC(classname,funcname,functype) \
        classname classname##funcname; \
        functype  FUNC_classname##funcname(classname::funcname);                                \
        Classregist Classregist##classname##funcname(#classname,#funcname,(void *)(&(FUNC_classname##funcname)))
    }
}