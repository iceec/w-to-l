#include <string>
#include <vector>
#include <list>
#include <unordered_map>
#include <set>
#include <stdexcept>
using namespace std;

namespace Yu
{
  namespace serialize
  {
    class data_stream
    {
      
      public: 
      enum 
      {
        data_bool = static_cast<unsigned int>(0), 
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
        data_define
      };
      
      void write(int value);
      
      void write(char value);
      
      void write(int64_t value);
      
      void write(float value);
      
      void write(double value);
      
      void write(bool value);
      
      void write(const char * value);
      
      void write(const std::basic_string<char> & value);
      
      void write(const char * p, size_t len);
      
      template<class T>
      void write(const vector<T> & p);
      template<class T>
      void write(const list<T> & p);
      template<class T, class V>
      void write(const unordered_map<T, V> & p);
      template<class T>
      void write(const set<T> & p);
      template<class T, class ... V>
      void write_args(const T & p, const V &... args);
      inline void write_args()
      {
      }
      
      void reseverse(size_t len);
      
      void read(bool & value);
      
      void read(char & value);
      
      void read(int & value);
      
      void read(int64_t & value);
      
      void read(float & value);
      
      void read(double & value);
      
      void read(char * value);
      
      void read(std::basic_string<char> & value);
      
      void read(char *const value, size_t len);
      
      inline void read_args()
      {
      }
      
      template<class T, class ... V>
      void read_args(T & p, V &... args);
      template<class T>
      void read(vector<T> & p);
      template<class T>
      void read(list<T> & p);
      template<class T, class V>
      void read(unordered_map<T, V> & p);
      template<class T>
      void read(set<T> & p);
      template<class T>
      friend data_stream & operator<<(data_stream & p, T value);
      
      /* First instantiated from: insights.cpp:608 */
      #ifdef INSIGHTS_USE_TEMPLATE
      template<>
      data_stream & operator<<<int>(data_stream & p, int value)
      {
        p.write(value);
        return p;
      }
      #endif
      
      template<class T>
      friend data_stream & operator>>(data_stream & p, T & value);
      
      /* First instantiated from: insights.cpp:557 */
      #ifdef INSIGHTS_USE_TEMPLATE
      template<>
      data_stream & operator>><int>(data_stream & p, int & value)
      {
        p.read(value);
        return p;
      }
      #endif
      
      void show();
      
      inline data_stream()
      : m_buf{std::vector<char, std::allocator<char> >()}
      {
        this->m_buf.clear();
        this->m_pos = 0;
      }
      
      
      private: 
      std::vector<char, std::allocator<char> > m_buf;
      size_t m_pos;
      public: 
      // inline ~data_stream() noexcept = default;
    };
    
    template<class T>
    data_stream & operator<<(data_stream & p, T value)
    {
      write(value);
      return p;
    }
    
    /* First instantiated from: insights.cpp:608 */
    #ifdef INSIGHTS_USE_TEMPLATE
    template<>
    data_stream & operator<<<int>(data_stream & p, int value)
    {
      p.write(value);
      return p;
    }
    #endif
    
    template<class T>
    data_stream & operator>>(data_stream & p, T & value)
    {
      read(value);
      return p;
    }
    
    /* First instantiated from: insights.cpp:557 */
    #ifdef INSIGHTS_USE_TEMPLATE
    template<>
    data_stream & operator>><int>(data_stream & p, int & value)
    {
      p.read(value);
      return p;
    }
    #endif
    
    template<class T, class ... V>
    void data_stream::write_args(const T & p, const V &... args)
    {
      write(p);
      write_args(args... );
    }
    template<class T, class ... V>
    void data_stream::read_args(T & p, V &... args)
    {
      read(p);
      read(args... );
    }
    template<class T>
    void data_stream::write(const vector<T> & p)
    {
      char type = static_cast<char>(data_stream::data_vector);
      this->write(&type, sizeof(type));
      int len = p.size();
      this->write(len);
      for(int i = 0; i < len; i++) {
        write(p[i]);
      }
      
    }
    template<class T>
    void data_stream::write(const list<T> & p)
    {
      char type = static_cast<char>(data_stream::data_list);
      this->write(&type, sizeof(type));
      int len = p.size();
      this->write(len);
      for(auto i = p.begin(); operator!=(i, p.end()); i++) {
        write(*i);
      }
      
    }
    template<class T, class V>
    void data_stream::write(const unordered_map<T, V> & p)
    {
      char type = static_cast<char>(data_stream::data_map);
      this->write(&type, sizeof(type));
      int len = p.size();
      this->write(len);
      for(auto i = p.begin(); operator!=(i, p.end()); i++) {
        write(i->first);
        write(i->second);
      }
      
    }
    template<class T>
    void data_stream::write(const set<T> & p)
    {
      char type = static_cast<char>(data_stream::data_set);
      this->write(&type, sizeof(type));
      int len = p.size();
      this->write(len);
      for(auto i = p.begin(); operator!=(i, p.end()); i++) {
        write(*i);
      }
      
    }
    template<class T>
    void data_stream::read(vector<T> & p)
    {
      if(static_cast<int>(this->m_buf.operator[](this->m_pos)) != static_cast<int>(data_stream::data_vector)) {
        throw std::logic_error(std::logic_error("vector format wrong"));
      } 
      
      p.clear();
      ++this->m_pos;
      int len;
      this->read(len);
      p.resize(len);
      for(int i = 0; i < len; i++) {
        read(p[i]);
      }
      
    }
    template<class T>
    void data_stream::read(list<T> & p)
    {
      if(static_cast<int>(this->m_buf.operator[](this->m_pos)) != static_cast<int>(data_stream::data_list)) {
        throw std::logic_error(std::logic_error("list format wrong"));
      } 
      
      p.clear();
      ++this->m_pos;
      int len;
      this->read(len);
      T temp;
      for(int i = 0; i < len; i++) {
        read(temp);
        p.push_back(temp);
      }
      
    }
    template<class T, class V>
    void data_stream::read(unordered_map<T, V> & p)
    {
      if(static_cast<int>(this->m_buf.operator[](this->m_pos)) != static_cast<int>(data_stream::data_map)) {
        throw std::logic_error(std::logic_error("map format wrong"));
      } 
      
      p.clear();
      ++this->m_pos;
      int len;
      this->read(len);
      T temp1;
      V temp2;
      for(int i = 0; i < len; i++) {
        read(temp1);
        read(temp2);
        p[temp1] = temp2;
      }
      
    }
    template<class T>
    void data_stream::read(set<T> & p)
    {
      if(static_cast<int>(this->m_buf.operator[](this->m_pos)) != static_cast<int>(data_stream::data_map)) {
        throw std::logic_error(std::logic_error("set format wrong"));
      } 
      
      p.clear();
      ++this->m_pos;
      int len;
      this->read(len);
      T temp;
      for(int i = 0; i < len; i++) {
        read(temp);
        p.insert(temp);
      }
      
    }
    
  }
  
}

#include <stdexcept>
#include <string.h>
#include <wchar.h>
#include <iostream>
namespace Yu
{
  namespace serialize
  {
    void data_stream::write(const char * p, size_t len)
    {
      this->reseverse(len);
      size_t before = this->m_buf.size();
      this->m_buf.resize(this->m_buf.size() + len);
      memcpy(reinterpret_cast<void *>(&this->m_buf.operator[](before)), reinterpret_cast<const void *>(p), len);
    }
    
    void data_stream::reseverse(size_t len)
    {
      size_t temp = len + this->m_buf.size();
      if(temp > this->m_buf.capacity()) {
        this->m_buf.reserve(2 * temp);
      } 
      
    }
    
    void data_stream::write(bool value)
    {
      char type = static_cast<char>(data_stream::data_bool);
      this->write(&type, sizeof(type));
      this->write(reinterpret_cast<char *>(&value), sizeof(value));
    }
    
    void data_stream::write(char value)
    {
      char type = static_cast<char>(data_stream::data_char);
      this->write(&type, sizeof(type));
      this->write(static_cast<char *>(&value), sizeof(value));
    }
    
    void data_stream::write(int value)
    {
      char type = static_cast<char>(data_stream::data_int);
      this->write(&type, sizeof(type));
      this->write(reinterpret_cast<char *>(&value), sizeof(value));
    }
    
    void data_stream::write(float value)
    {
      char type = static_cast<char>(data_stream::data_float);
      this->write(&type, sizeof(type));
      this->write(reinterpret_cast<char *>(&value), sizeof(value));
    }
    
    void data_stream::write(double value)
    {
      char type = static_cast<char>(data_stream::data_double);
      this->write(&type, sizeof(type));
      this->write(reinterpret_cast<char *>(&value), sizeof(value));
    }
    
    void data_stream::write(int64_t value)
    {
      char type = static_cast<char>(data_stream::data_int64);
      this->write(&type, sizeof(type));
      this->write(reinterpret_cast<char *>(&value), sizeof(value));
    }
    
    void data_stream::write(const char * value)
    {
      const char type = static_cast<const char>(data_stream::data_string);
      this->write(&type, sizeof(type));
      int32_t len = static_cast<int>(strlen(value));
      this->write(static_cast<int>(len));
      this->write(value, static_cast<unsigned long>(len));
    }
    
    void data_stream::write(const std::basic_string<char> & value)
    {
      this->write(value.c_str());
    }
    
    void data_stream::read(char *const value, size_t len)
    {
      memcpy(reinterpret_cast<void *>(value), reinterpret_cast<const void *>(&this->m_buf.operator[](this->m_pos)), len);
      this->m_pos = this->m_pos + len;
    }
    
    void data_stream::read(bool & value)
    {
      if(static_cast<int>(this->m_buf.at(this->m_pos)) == static_cast<int>(data_stream::data_bool)) {
        ++this->m_pos;
        value = static_cast<bool>(this->m_buf.at(this->m_pos++));
      } else {
        throw std::logic_error(std::logic_error("bool format wrong"));
      } 
      
    }
    
    void data_stream::read(char & value)
    {
      if(static_cast<int>(this->m_buf.at(this->m_pos)) == static_cast<int>(data_stream::data_char)) {
        ++this->m_pos;
        value = this->m_buf.at(this->m_pos++);
      } else {
        throw std::logic_error(std::logic_error("char format wrong"));
      } 
      
    }
    
    void data_stream::read(int & value)
    {
      if(static_cast<int>(this->m_buf.at(this->m_pos)) == static_cast<int>(data_stream::data_int)) {
        ++this->m_pos;
        value = *(reinterpret_cast<int *>(&this->m_buf.operator[](this->m_pos)));
        this->m_pos = this->m_pos + 4;
      } else {
        throw std::logic_error(std::logic_error("int format wrong"));
      } 
      
    }
    
    void data_stream::read(int64_t & value)
    {
      if(static_cast<int>(this->m_buf.at(this->m_pos)) == static_cast<int>(data_stream::data_int64)) {
        ++this->m_pos;
        value = *(reinterpret_cast<int64_t *>(&this->m_buf.operator[](this->m_pos)));
        this->m_pos = this->m_pos + 8;
      } else {
        throw std::logic_error(std::logic_error("int64_t format wrong"));
      } 
      
    }
    
    void data_stream::read(float & value)
    {
      if(static_cast<int>(this->m_buf.at(this->m_pos)) == static_cast<int>(data_stream::data_float)) {
        ++this->m_pos;
        value = *(reinterpret_cast<float *>(&this->m_buf.operator[](this->m_pos)));
        this->m_pos = this->m_pos + 4;
      } else {
        throw std::logic_error(std::logic_error("float format wrong"));
      } 
      
    }
    
    void data_stream::read(double & value)
    {
      if(static_cast<int>(this->m_buf.at(this->m_pos)) == static_cast<int>(data_stream::data_double)) {
        ++this->m_pos;
        value = *(reinterpret_cast<double *>(&this->m_buf.operator[](this->m_pos)));
        this->m_pos = this->m_pos + 8;
      } else {
        throw std::logic_error(std::logic_error("double format wrong"));
      } 
      
    }
    
    void data_stream::read(char * value)
    {
      if(static_cast<int>(this->m_buf.at(this->m_pos)) == static_cast<int>(data_stream::data_string)) {
        ++this->m_pos;
        int p;
        this->read(p);
        memcpy(reinterpret_cast<void *>(value), reinterpret_cast<const void *>(&(this->m_buf.operator[](this->m_pos))), static_cast<unsigned long>(p));
        this->m_pos = this->m_pos + static_cast<unsigned long>(p);
      } else {
        throw std::logic_error(std::logic_error("char * format wrong"));
      } 
      
    }
    
    void data_stream::read(std::basic_string<char> & value)
    {
      if(static_cast<int>(this->m_buf.at(this->m_pos)) == static_cast<int>(data_stream::data_string)) {
        ++this->m_pos;
        int p;
        this->read(p);
        value.assign(&(this->m_buf.operator[](this->m_pos)), static_cast<unsigned long>(p));
        this->m_pos = this->m_pos + static_cast<unsigned long>(p);
      } else {
        throw std::logic_error(std::logic_error("string format wrong"));
      } 
      
    }
    
    void data_stream::show()
    {
      size_t i = 0;
      int p;
      while(i < this->m_buf.size()) {
        switch(static_cast<int>(this->m_buf.operator[](i))) {
          case static_cast<int>(data_stream::data_bool): ++i;
          if(static_cast<int>(this->m_buf.at(i)) != 0) {
            std::operator<<(std::cout, "True ");
          } else {
            std::operator<<(std::cout, "False ");
          } 
          
          ++i;
          break;
          case static_cast<int>(data_stream::data_char): ++i;
          std::operator<<(std::operator<<(std::cout, this->m_buf.at(i)), " ");
          ++i;
          break;
          case static_cast<int>(data_stream::data_int): ++i;
          std::operator<<(std::cout.operator<<(*(reinterpret_cast<int *>(&this->m_buf.operator[](i)))), " ");
          i = i + 4;
          break;
          case static_cast<int>(data_stream::data_int64): ++i;
          std::operator<<(std::cout.operator<<(*(reinterpret_cast<int64_t *>(&this->m_buf.operator[](i)))), " ");
          i = i + 8;
          break;
          case static_cast<int>(data_stream::data_float): ++i;
          std::operator<<(std::cout.operator<<(*(reinterpret_cast<float *>(&this->m_buf.operator[](i)))), " ");
          i = i + 4;
          break;
          case static_cast<int>(data_stream::data_double): ++i;
          std::operator<<(std::cout.operator<<(*(reinterpret_cast<double *>(&this->m_buf.operator[](i)))), " ");
          i = i + 8;
          break;
          case static_cast<int>(data_stream::data_string): ++i;
          if(static_cast<int>(this->m_buf.operator[](i)) != static_cast<int>(data_stream::data_int)) {
            throw std::logic_error(std::logic_error("string type wrong"));
          } 
          
          ++i;
          p = *(reinterpret_cast<int *>(&this->m_buf.operator[](i)));
          i = i + 4;
          std::operator<<(std::operator<<(std::cout, std::basic_string<char>(&this->m_buf.operator[](i), static_cast<unsigned long>(p), std::allocator<char>())), " ");
          i = i + static_cast<unsigned long>(p);
          break;
          case static_cast<int>(data_stream::data_vector): ++i;
          if(static_cast<int>(this->m_buf.operator[](i)) != static_cast<int>(data_stream::data_vector)) {
            throw std::logic_error(std::logic_error("vector type wrong"));
          } 
          
          ++i;
          p = *(reinterpret_cast<int *>(&this->m_buf.operator[](i)));
          i = i + 4;
          default: throw std::logic_error(std::logic_error("type wrong"));
          break;
        }
      }
      
      std::cout.operator<<(std::endl);
    }
    
    bool isbig()
    {
      int x = 1;
      char * p = reinterpret_cast<char *>(&x);
      if(static_cast<int>(p[0]) == 1) {
        return 0;
      } 
      
      if(static_cast<int>(p[sizeof(int) - 1]) == 1) {
        return 1;
      } 
      
      throw std::logic_error(std::logic_error("isbig wrong"));
    }
    
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
      inline Function() noexcept = default;
      inline ~Function() noexcept = default;
      template<class F>
      void bind(const std::basic_string<char> & func_name, F func);
      
      /* First instantiated from: insights.cpp:605 */
      #ifdef INSIGHTS_USE_TEMPLATE
      template<>
      void bind<int (*)(int, int)>(const std::basic_string<char> & func_name, int (*func)(int, int))
      {
        this->m_handler.operator[](func_name).operator=(std::bind(&wrap<int (*)(int, int)>, this, func, std::placeholders::_1, std::placeholders::_2));
      }
      #endif
      
      void call(const std::basic_string<char> & func_name, data_stream & in, data_stream & out);
      
      template<class F>
      void wrap(F func, data_stream & in, data_stream & out);
      
      /* First instantiated from: insights.cpp:516 */
      #ifdef INSIGHTS_USE_TEMPLATE
      template<>
      void wrap<int (*)(int, int)>(int (*func)(int, int), data_stream & in, data_stream & out)
      {
        this->wrap_impl<int, int, int>(func, in, out);
      }
      #endif
      
      template<class R, class ... Args>
      void wrap_impl(R (*func)(Args...), data_stream & in, data_stream & out);
      
      /* First instantiated from: insights.cpp:522 */
      #ifdef INSIGHTS_USE_TEMPLATE
      template<>
      void wrap_impl<int, int, int>(int (*func)(int, int), data_stream & in, data_stream & out)
      {
        this->wrap_impl<int, int, int>(std::function<int (int, int)>(std::function<int (int, int)>(func)), in, out);
      }
      #endif
      
      template<class R, class ... Args>
      void wrap_impl(std::function<R (Args...)> func, data_stream & in, data_stream & out);
      
      /* First instantiated from: insights.cpp:528 */
      #ifdef INSIGHTS_USE_TEMPLATE
      template<>
      void wrap_impl<int, int, int>(std::function<int (int, int)> func, data_stream & in, data_stream & out)
      {
        using tuple_type = std::tuple<typename std::decay<int>::type, typename std::decay<int>::type>;
        constexpr const unsigned long size = std::integral_constant<unsigned long, 2>::value;
        std::tuple<int, int> args = this->get_args<std::tuple<int, int>, 0, 1>(in, std::integer_sequence<unsigned long, 0, 1>{});
        if(std::integral_constant<bool, false>::value) {
          this->call_impl<int, std::function<int (int, int)>, std::tuple<int, int> >(std::function<int (int, int)>(func), std::tuple<int, int>(args));
          return;
        } 
        
        int p = this->call_impl<int, std::function<int (int, int)>, std::tuple<int, int> >(std::function<int (int, int)>(func), std::tuple<int, int>(args));
        Yu::serialize::operator<<(out, p);
      }
      #endif
      
      template<class T, std::size_t ...I>
      T get_args(data_stream & in, std::index_sequence<I...>);
      
      /* First instantiated from: insights.cpp:536 */
      #ifdef INSIGHTS_USE_TEMPLATE
      template<>
      std::tuple<int, int> get_args<std::tuple<int, int>, 0, 1>(data_stream & in, std::integer_sequence<unsigned long, 0, 1>)
      {
        std::tuple<int, int> t = std::tuple<int, int>() /* NRVO variable */;
        std::initializer_list<int>{(this->get_args<std::tuple<int, int>, 0>(in, t) , 0), (this->get_args<std::tuple<int, int>, 1>(in, t) , 0)};
        return t;
      }
      #endif
      
      template<class T, std::size_t id>
      void get_args(data_stream & in, T & t);
      
      /* First instantiated from: insights.cpp:551 */
      #ifdef INSIGHTS_USE_TEMPLATE
      template<>
      void get_args<std::tuple<int, int>, 0>(data_stream & in, std::tuple<int, int> & t)
      {
        Yu::serialize::operator>>(in, std::get<0UL>(t));
      }
      #endif
      
      
      /* First instantiated from: insights.cpp:551 */
      #ifdef INSIGHTS_USE_TEMPLATE
      template<>
      void get_args<std::tuple<int, int>, 1>(data_stream & in, std::tuple<int, int> & t)
      {
        Yu::serialize::operator>>(in, std::get<1UL>(t));
      }
      #endif
      
      template<class R, class F, class T>
      typename std::enable_if<std::is_same<R, void>::value, int32_t>::type call_impl(F func, T args);
      template<class R, class F, class T>
      typename std::enable_if<!std::is_same<R, void>::value, R>::type call_impl(F func, T args);
      
      /* First instantiated from: insights.cpp:539 */
      #ifdef INSIGHTS_USE_TEMPLATE
      template<>
      typename std::enable_if<!std::is_same<int, void>::value, int>::type call_impl<int, std::function<int (int, int)>, std::tuple<int, int> >(std::function<int (int, int)> func, std::tuple<int, int> args)
      {
        constexpr const unsigned long size = std::integral_constant<unsigned long, 2>::value;
        return this->invoke<int, std::function<int (int, int)>, std::tuple<int, int>, 0, 1>(std::function<int (int, int)>(func), std::tuple<int, int>(args), std::integer_sequence<unsigned long, 0, 1>{});
      }
      #endif
      
      template<class R, class F, class T, size_t ...I>
      auto invoke(F func, T args, std::index_sequence<I...>);
      
      #ifdef INSIGHTS_USE_TEMPLATE
      template<>
      int invoke<int, std::function<int (int, int)>, std::tuple<int, int>, 0, 1>(std::function<int (int, int)> func, std::tuple<int, int> args, std::integer_sequence<unsigned long, 0, 1>)
      {
        return func.operator()(std::get<0UL>(args), std::get<1UL>(args));
      }
      #endif
      
      
      private: 
      std::unordered_map<std::basic_string<char>, std::function<void (Yu::serialize::data_stream &, Yu::serialize::data_stream &)>, std::hash<std::basic_string<char> >, std::equal_to<std::basic_string<char> >, std::allocator<std::pair<const std::basic_string<char>, std::function<void (Yu::serialize::data_stream &, Yu::serialize::data_stream &)> > > > m_handler;
      public: 
    };
    
    template<class F>
    void Function::bind(const std::basic_string<char> & func_name, F func)
    {
      this->m_handler.operator[](func_name) = std::bind(operator&(Function::wrap<F>), this, func, std::placeholders::_1, std::placeholders::_2);
    }
    
    /* First instantiated from: insights.cpp:605 */
    #ifdef INSIGHTS_USE_TEMPLATE
    template<>
    void bind<int (*)(int, int)>(const std::basic_string<char> & func_name, int (*func)(int, int))
    {
      this->m_handler.operator[](func_name).operator=(std::bind(&Function::wrap<int (*)(int, int)>, this, func, std::placeholders::_1, std::placeholders::_2));
    }
    #endif
    
    template<class F>
    void Function::wrap(F func, data_stream & in, data_stream & out)
    {
      wrap_impl(func, in, out);
    }
    
    /* First instantiated from: insights.cpp:516 */
    #ifdef INSIGHTS_USE_TEMPLATE
    template<>
    void wrap<int (*)(int, int)>(int (*func)(int, int), data_stream & in, data_stream & out)
    {
      this->wrap_impl<int, int, int>(func, in, out);
    }
    #endif
    
    template<class R, class ... Args>
    void Function::wrap_impl(R (*func)(Args...), data_stream & in, data_stream & out)
    {
      wrap_impl(std::function<R (Args...)>(func), in, out);
    }
    
    /* First instantiated from: insights.cpp:522 */
    #ifdef INSIGHTS_USE_TEMPLATE
    template<>
    void wrap_impl<int, int, int>(int (*func)(int, int), data_stream & in, data_stream & out)
    {
      this->wrap_impl<int, int, int>(std::function<int (int, int)>(std::function<int (int, int)>(func)), in, out);
    }
    #endif
    
    template<class R, class ... Args>
    void Function::wrap_impl(std::function<R (Args...)> func, data_stream & in, data_stream & out)
    {
      using tuple_type = std::tuple<typename std::decay<Args>::type...>;
      constexpr const auto size = std::tuple_size<typename std::decay<tuple_type>::type>::value;
      tuple_type args = get_args<tuple_type>(in, make_index_sequence<size>{});
      if(std::is_same<R, void>::value) {
        call_impl<R>(func, args);
        return;
      } 
      
      auto p = call_impl<R>(func, args);
      operator<<(out, p);
    }
    
    /* First instantiated from: insights.cpp:528 */
    #ifdef INSIGHTS_USE_TEMPLATE
    template<>
    void wrap_impl<int, int, int>(std::function<int (int, int)> func, data_stream & in, data_stream & out)
    {
      using tuple_type = std::tuple<typename std::decay<int>::type, typename std::decay<int>::type>;
      constexpr const unsigned long size = std::integral_constant<unsigned long, 2>::value;
      std::tuple<int, int> args = this->get_args<std::tuple<int, int>, 0, 1>(in, std::integer_sequence<unsigned long, 0, 1>{});
      if(std::integral_constant<bool, false>::value) {
        this->call_impl<int, std::function<int (int, int)>, std::tuple<int, int> >(std::function<int (int, int)>(func), std::tuple<int, int>(args));
        return;
      } 
      
      int p = this->call_impl<int, std::function<int (int, int)>, std::tuple<int, int> >(std::function<int (int, int)>(func), std::tuple<int, int>(args));
      serialize::operator<<(out, p);
    }
    #endif
    
    template<class T, std::size_t ...I>
    T Function::get_args(data_stream & in, std::index_sequence<I...>)
    {
      T t /* NRVO variable */;
      std::initializer_list<int>{{(get_args<T, I>(in, t) , 0)... }};
      return t;
    }
    
    /* First instantiated from: insights.cpp:536 */
    #ifdef INSIGHTS_USE_TEMPLATE
    template<>
    std::tuple<int, int> get_args<std::tuple<int, int>, 0, 1>(data_stream & in, std::integer_sequence<unsigned long, 0, 1>)
    {
      std::tuple<int, int> t = std::tuple<int, int>() /* NRVO variable */;
      std::initializer_list<int>{(this->get_args<std::tuple<int, int>, 0>(in, t) , 0), (this->get_args<std::tuple<int, int>, 1>(in, t) , 0)};
      return t;
    }
    #endif
    
    template<class T, std::size_t id>
    void Function::get_args(data_stream & in, T & t)
    {
      operator>>(in, std::get<id>(t));
    }
    
    /* First instantiated from: insights.cpp:551 */
    #ifdef INSIGHTS_USE_TEMPLATE
    template<>
    void get_args<std::tuple<int, int>, 0>(data_stream & in, std::tuple<int, int> & t)
    {
      serialize::operator>>(in, std::get<0UL>(t));
    }
    #endif
    
    
    /* First instantiated from: insights.cpp:551 */
    #ifdef INSIGHTS_USE_TEMPLATE
    template<>
    void get_args<std::tuple<int, int>, 1>(data_stream & in, std::tuple<int, int> & t)
    {
      serialize::operator>>(in, std::get<1UL>(t));
    }
    #endif
    
    template<class R, class F, class T>
    typename std::enable_if<std::is_same<R, void>::value, int32_t>::type Function::call_impl(F func, T args)
    {
      constexpr const auto size = std::tuple_size<typename std::decay<T>::type>::value;
      invoke<R>(func, args, std::make_index_sequence<size>{});
      return 0;
    }
    template<class R, class F, class T>
    typename std::enable_if<!std::is_same<R, void>::value, R>::type Function::call_impl(F func, T args)
    {
      constexpr const auto size = std::tuple_size<typename std::decay<T>::type>::value;
      return invoke<R>(func, args, std::make_index_sequence<size>{});
    }
    
    /* First instantiated from: insights.cpp:539 */
    #ifdef INSIGHTS_USE_TEMPLATE
    template<>
    typename std::enable_if<!std::is_same<int, void>::value, int>::type call_impl<int, std::function<int (int, int)>, std::tuple<int, int> >(std::function<int (int, int)> func, std::tuple<int, int> args)
    {
      constexpr const unsigned long size = std::integral_constant<unsigned long, 2>::value;
      return this->invoke<int, std::function<int (int, int)>, std::tuple<int, int>, 0, 1>(std::function<int (int, int)>(func), std::tuple<int, int>(args), std::integer_sequence<unsigned long, 0, 1>{});
    }
    #endif
    
    template<class R, class F, class T, size_t ...I>
    auto Function::invoke(F func, T args, std::index_sequence<I...>)
    {
      return func(std::get<I>(args)... );
    }
    
    #ifdef INSIGHTS_USE_TEMPLATE
    template<>
    int invoke<int, std::function<int (int, int)>, std::tuple<int, int>, 0, 1>(std::function<int (int, int)> func, std::tuple<int, int> args, std::integer_sequence<unsigned long, 0, 1>)
    {
      return func.operator()(std::get<0UL>(args), std::get<1UL>(args));
    }
    #endif
    
    inline void Function::call(const std::basic_string<char> & func_name, data_stream & in, data_stream & out)
    {
      if(static_cast<bool>(this->m_handler.count(func_name))) {
        this->m_handler.operator[](func_name).operator()(in, out);
      } 
      
      return;
    }
    
    
  }
  
}

using namespace Yu::rpc;
using namespace Yu::serialize;

int test(int x, int y)
{
  std::operator<<(std::cout.operator<<(x), " ").operator<<(y).operator<<(std::endl);
  return x + y;
}

int main()
{
  Yu::rpc::Function x = Yu::rpc::Function();
  x.bind<int (*)(int, int)>(std::basic_string<char>("test", std::allocator<char>()), test);
  Yu::serialize::data_stream y = Yu::serialize::data_stream();
  Yu::serialize::operator<<(Yu::serialize::operator<<(y, 1), 2);
  Yu::serialize::data_stream xx = Yu::serialize::data_stream();
  x.call(std::basic_string<char>("test", std::allocator<char>()), y, xx);
  return 0;
}
