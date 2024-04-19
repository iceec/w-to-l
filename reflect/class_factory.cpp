#include "class_factory.h"
// 一个factory的单例来维护类的所有信息
// 对于想获取有没有这个类 map对应 返回一个函数的执行 这个函数返回对象指针
// 所以对象要继承一个基类对象 这样所有的函数可以类型一致
// 放入map的操作要再来一个对象 用这个对象的构造函数来调用单例模式的某个set函数

// 放入成员信息 有类型名 成员名称和偏移量 获取的话直接调用单例的get
// 注册的话同样调用某一个类的构造函数把东西放进去 要创建你设计的类的对象的
// get 的时候是要做一些解释的  get时 你要基类templat函数 然后我就get 你了
namespace Yu::reflect
{
   void Factory::regist_class(const string &name, create_class func)
    {
        single<Factory>::instance()->m_class_map[name] = func;
    }
    Objs *Factory::createclass(const string &name)
    {
        if (m_class_map.count(name))
            return (Objs *)m_class_map[name]();
        return nullptr;
    }

    void Factory::regist_class_field(const string &name, const string a, const string b, size_t c)
    {
        if (m_class_field.count(name))
            m_class_field[name].push_back(Class_field(a, b, c));
        else
            m_class_field[name] = vector<Class_field>{Class_field(a, b, c)};
    }

    void Factory::regist_class_func(const string &classname,const string funcname,void *funaddr){
            if(m_class_func.count(classname))
               m_class_func[classname].push_back(Class_func(funcname,funaddr));
            else
                m_class_func[classname]=vector<Class_func>{Class_func(funcname,funaddr)}; 
    }

    Class_field * Factory::find_class_field(const string name,const string type_name){
        if(m_class_field.count(name)==0)
            return nullptr;
            auto i=m_class_field[name].begin();
        for(;i!=m_class_field[name].end();++i){
                if(i->FieldName()==type_name)
                    break;
        }
        return (i==m_class_field[name].end())?nullptr:(Class_field *)&(*i);
    }

    Class_func *Factory::find_calss_func(const string classname,const string funcname){
           if(m_class_func.count(classname)==0)
                return nullptr;
            auto i=m_class_func[classname].begin();
            for(;i!=m_class_func[classname].end();++i){
                if(i->name()==funcname)
                    break;
            }
            return (i==m_class_func[classname].end())? nullptr : &(*i);
    }


    
}