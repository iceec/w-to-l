#include "json.h"
#include <iostream>

namespace Yu::json
{

    json::json(const int &p)
    {
        clear();
        m_int = p;
        m_type = json_int;
    }

    json::json(const double &p)
    {
        clear();
        m_double = p;
        m_type = json_double;
    }

    json::json(const bool p)
    {
        clear();
        m_bool = p;
        m_type = json_bool;
    }
    json::json(const char *p)
    {
        clear();
        m_type = json_string;
        m_string = new string(p);
    }

    json::json(const string &p)
    {
        clear();

        m_type = json_string;

        m_string = new string(p);
    }
    json::json(const json &p)
    {
        clear();

        m_type = p.m_type;

        switch (m_type)
        {
        case json_int:
            m_int = p.m_int;
            break;
        case json_bool:
            m_bool = p.m_bool;
            break;
        case json_double:
            m_double = p.m_double;
            break;
        case json_string:
            m_string = new string(*(p.m_string));
            break;
        case json_array:
            m_array = new vector<json>();
            for (auto i = p.m_array->begin(); i != p.m_array->end(); ++i)
                m_array->push_back(*i);
            break;

        case json_obj:
            m_obj = new unordered_map<string, json>(*p.m_obj);
            break;
        default:
            break;
        }
    }
    json::json(const vector<json> &p)
    {
        clear();
        m_type = json_array;
        m_array = new vector<json>();
        for (auto i = p.begin(); i != p.end(); ++i)
            m_array->push_back(*i);
    }
    json::json(const unordered_map<string, json> &p)
    {
        clear();
        m_type = json_obj;

        m_obj = new unordered_map<string, json>();

        for (auto i = p.begin(); i != p.end(); ++i)
        {
            (*m_obj)[i->first] = i->second;
        }
    }

    void json::clear()
    {

        switch (m_type)
        {
        case json_array:
            if (m_array != nullptr)
            {
                for (auto i = m_array->begin(); i != m_array->end(); ++i)
                    i->clear();
                delete m_array;
                m_array = nullptr;
            }
            break;
        case json_string:
            if (m_string != nullptr)
                delete m_string;
            m_string = nullptr;
            break;
        case json_obj:
            if (m_obj != nullptr)
            {
                for (auto i = m_obj->begin(); i != m_obj->end(); ++i)
                    i->second.clear();
                delete m_obj;
                m_obj = nullptr;
            }
            break;
        default:
            break;
        }
    }
    json::~json()
    {
        clear();
    }
    json::json(Type type)
    {

        m_type = type;

        switch (m_type)
        {
        case json_int:
            m_int = 0;
            break;
        case json_bool:
            m_bool = 0;
            break;
        case json_double:
            m_double = 0;
            break;
        case json_string:
            m_string = new string("");
            break;
        case json_array:
            m_array = new vector<json>();
            break;
        case json_obj:
            m_obj = new unordered_map<string, json>();
            break;
        default:
            break;
        }
    }

    string json::str() const
    {
        ostringstream ss;
        switch (m_type)
        {
        case json_bool:
            (m_bool == 0) ? ss << "false" : ss << "true";
            return ss.str();
            break;
        case json_int:
            ss << m_int;
            return ss.str();
            break;
        case json_double:
            ss << m_double;
            return ss.str();
            break;
        case json_string:
           cout<<*m_string<<endl;
            ss << "\"" << *m_string << "\"";
            return ss.str();
            break;
        case json_array:
            ss << '[';
            for (auto i = m_array->begin(); i != m_array->end(); ++i)
            {
                if (i != m_array->begin())
                    ss << ",";
                ss << i->str();
            }
            ss << ']';
            return ss.str();
            break;

        case json_obj:
            ss << '{' << endl;
            for (auto i = m_obj->begin(); i != m_obj->end(); ++i)
            {
                cout<<i->first<<endl;
                ss << i->first << ": " << i->second.str() <<","<< endl;
            }
                
            ss << '}';
            return ss.str();
            break;
        default:
            ss << "null";
            return ss.str();
            break;
        }

        return ss.str();
    }

    ostream &operator<<(ostream &f, const json &p)
    {
        f << p.str();
        return f;
    }

    json &json::operator=(const json &p)
    {
        clear();

        m_type = p.m_type;

        switch (m_type)
        {
        case json_int:
            m_int = p.m_int;
            break;
        case json_bool:
            m_bool = p.m_bool;
            break;
        case json_double:
            m_double = p.m_double;
            break;
        case json_string:
            m_string = new string(*(p.m_string));
            break;
        case json_array:
            m_array = new vector<json>();
            for (auto i = p.m_array->begin(); i != p.m_array->end(); ++i)
                m_array->push_back(*i);
            break;

        case json_obj:
            m_obj = new unordered_map<string, json>(*p.m_obj);
            break;
        default:
            break;
        }
        return *this;
    }

    json &json::operator=(json &&p)
    {

        m_type = p.m_type;

        switch (m_type)
        {
        case json_int:
            m_int = p.m_int;
            break;
        case json_bool:
            m_bool = p.m_bool;
            break;
        case json_double:
            m_double = p.m_double;
            break;
        case json_string:
            m_string = p.m_string;
            p.m_string = nullptr;
            break;
        case json_array:
            m_array = p.m_array;
            p.m_array = nullptr;
            break;
        case json_obj:
            m_obj = p.m_obj;
            p.m_obj = nullptr;
            break;
        default:
            break;
        }

        return *this;
    }

    json::json(json &&p)
    {
        m_type = p.m_type;

        switch (m_type)
        {
        case json_int:
            m_int = p.m_int;
            break;
        case json_bool:
            m_bool = p.m_bool;
            break;
        case json_double:
            m_double = p.m_double;
            break;
        case json_string:
            m_string = p.m_string;
            p.m_string = nullptr;
            break;
        case json_array:
            m_array = p.m_array;
            p.m_array = nullptr;
            break;
        case json_obj:
            m_obj = p.m_obj;
            p.m_obj = nullptr;
            break;
        default:
            break;
        }
    }

    bool json::has(int index) const
    {
        if (m_type != json_array || m_array == nullptr)
            throw logic_error("should be array");
        if (index < 0 || index >= m_array->size())
            return 0;
        return 1;
    }

    json &json::get(int index)
    {
        if (has(index))
        {
            return (*m_array)[index];
        }
        throw logic_error("yue jie can not get");
    }

    json json::get(int index) const
    {
        if (has(index))
        {
            return (*m_array)[index];
        }
        throw logic_error("yue jie can not get");
    }

    void json::remove(int index)
    {
        if (has(index))
        {
            m_array->erase(abegin() + index);
        }
    }
    json::array_iterator json::abegin()
    {
        if (m_array != nullptr)
            return m_array->begin();
        array_iterator pd;
        throw logic_error("m_array is null no begin");
        return pd;
    }

    json::array_iterator json::aend()
    {
        if (m_array != nullptr)
            return m_array->end();
        array_iterator pd;
        throw logic_error("m_array is null no end");
        return pd;
    }

    void json::append(json &&p)
    {
        if (m_type != json_array || m_array == nullptr)
            throw logic_error("should be array append");
        m_array->push_back(forward<json>(p));
    }

    void json::append(const json &p)
    {
        if (m_type != json_array || m_array == nullptr)
            throw logic_error("should be array append");
        m_array->push_back(p);
    }

    json &json::operator[](int index)
    {
        return const_cast<json &>(*this).get(index);
    }

    json json::operator[](int index) const
    {
        return const_cast<const json &>(*this).get(index);
    }

    bool json::has(string &s) const
    {
        if (m_type != json_obj)
            throw logic_error("not obj");
        return m_obj->count(s) > 0;
    }

    bool json::has(const char *s) const
    {
        if (m_type != json_obj)
            throw logic_error("not obj");
        return m_obj->count(s) > 0;
    }

    json &json::get(string &s)
    {
        if (has(s))
            return (*m_obj)[s];
        throw logic_error("kk");
    }

    json &json::get(const char *s)
    {
        if (has(s))
            return (*m_obj)[s];
        throw logic_error("kk");
    }

    json json::get(string &s) const
    {
        if (has(s))
            return (*m_obj)[s];
        throw logic_error("kk");
    }

    json json::get(const char *s) const
    {
        if (has(s))
            return (*m_obj)[s];
        throw logic_error("kk");
    }

    json &json::operator[](string &s)
    {
        return const_cast<json &>(*this).get(s);
    }

    json &json::operator[](const char *s)
    {
        return const_cast<json &>(*this).get(s);
    }

    json json::operator[](string &s) const
    {
        return const_cast<const json &>(*this).get(s);
    }

    json json::operator[](const char *s) const
    {
        return const_cast<const json &>(*this).get(s);
    }

    void json::append(const pair<string, json> &p)
    {
        if (m_type != json_obj)
            throw("not append");
        (*m_obj)[p.first] = p.second;
    }

    void json::append(pair<string, json> &&p)
    {
        if (m_type != json_obj)
            throw("not append");
        (*m_obj)[move(p.first)] = move(p.second);
    }

    void json::remove(string &s)
    {
        if (has(s))
            m_obj->erase(s);
        return;
    }

}
