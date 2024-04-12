#include "parse.h"
#include <iostream>
#define inrange(x) (x >= '0' && x <= '9')
#define insize(x) ((x) < m_str.size())

namespace Yu::json
{
    void parse::load(string &filename)
    {
        ifstream fin(filename);
        stringstream ss;
        if (!fin.is_open())
            throw logic_error("no such file");
        ss << fin.rdbuf(); // 返回指针
        m_str = ss.str();
    }

    void parse::load(const char *filename)
    {
        ifstream fin(filename);
        stringstream ss;
        if (!fin.is_open())
            throw logic_error("no such file");
        ss << fin.rdbuf();
        m_str = ss.str();
        m_idx = 0;
    }
    json parse::parsefile()
    {
        if (m_str.size() <= m_idx)
            throw logic_error("parse file legth wrong");
        switch (m_str[m_idx])
        {
        case '"':
            m_idx++;
            return parsestring();

        case 'n':
            return parsenull();

        case 't':
        case 'f':
            return parsebool();

        case '[':
            m_idx++;
            return parsearray();

        case '{':
            m_idx++;
            return parseobj();
        default:
            if (inrange(m_str[m_idx]))
                return parsenumber();
            else
            {
             
                cout<<endl;
                throw logic_error("no match");
            }
                
        }
    }

    void parse::skipwhite()
    {
        while (m_idx < m_str.size() && (m_str[m_idx] == ' ' || m_str[m_idx] == '\n' || m_str[m_idx] == '\t' || m_str[m_idx] == '\r'))
            m_idx++;
    }

    json parse::parsebool()
    {
        if (m_str.compare(m_idx, 4, "true") == 0)
        {
            m_idx += 4;
            return json(true);
        }

        else if (m_str.compare(m_idx, 5, "false") == 0)
        {
            m_idx += 5;
            return json(false);
        }

        throw logic_error("parsebool wrong");
    }

    json parse::parsenull()
    {
        if (m_str.compare(m_idx, 4, "null") == 0)
        {
            m_idx += 4;
            return json();
        }

        throw logic_error("parsenull wrong");
    }

    json parse::parsestring()
    {
        size_t pos = m_str.find_first_of('"', m_idx);
        if (pos == string::npos)
            throw logic_error("parsestring wrong");
        if (pos == m_idx)
            return json("");
        string temp =m_str.substr(m_idx, pos-m_idx);
        m_idx = pos + 1;
        return json(temp);
    }

    json parse::parsenumber()
    {

        size_t pos = m_idx; // 判断是不是0
        while (inrange(m_str[m_idx]))
            m_idx++;
        if (m_str[m_idx] == '.')
        {
            m_idx++;
            if (insize(m_idx) && inrange(m_str[m_idx]))
            {
                while (inrange(m_str[m_idx]))
                    m_idx++;
                string a =m_str.substr(pos, m_idx-pos);
                while (m_str[m_idx] != 0 && !iswhite())
                    m_idx++;
                skipwhite();
                return json(atof(a.c_str()));
            }
            throw logic_error("no a float");
        }
        string a = m_str.substr(pos, m_idx-pos);
        while (m_str[m_idx] != 0 && !iswhite())
            m_idx++;
        skipwhite();
        return json(atoi(a.c_str()));
    }

    json parse::parsearray()
    {
        skipwhite();
        vector<json> temp;
    while(m_idx<m_str.size()&&m_str[m_idx]!=']'){
        skipwhite();
        temp.push_back(parsefile());
        skipwhite();
        if(m_str[m_idx]==',')
            m_idx++;
        else if(m_str[m_idx]!=']'){
           
               for(int i=100;i>=0;--i)
                    cout<<m_str[m_idx-i];
                    cout<<m_idx<<endl;
            throw logic_error("array format wrong");
        }
            
        else
            break;
        skipwhite();
    }
    if(m_str[m_idx]!=']')
        throw logic_error("array format wron");
    return json(temp);
    }

  

    json parse::parseobj()
    {
        skipwhite();
        unordered_map<string,json>temp;
        while(m_idx<m_str.size()&&m_str[m_idx]!='}'){
            skipwhite();
            size_t pos=m_str.find_first_of(":}",m_idx);
            if(pos==string::npos)
                throw logic_error("obj format wrong");
            if(m_str[pos]=='}'){
                m_idx=pos+1;
                break;
            }
            else{
                string name=m_str.substr(m_idx, pos-m_idx);
                size_t namepos=name.find_last_not_of(" \r\n\t");
                
                if(namepos!=string::npos)
                    name=name.substr(0,namepos+1);
                m_idx=pos+1;
                skipwhite();
                temp[name]=parsefile();
            }
            
        }
        return json(temp);
    
    }

    bool parse::iswhite()
    {
        if (m_idx < m_str.size() && (m_str[m_idx] == ' ' || m_str[m_idx] == '\n' || m_str[m_idx] == '\t' || m_str[m_idx] == '\r'))
            return 1;
        return 0;
    }

}