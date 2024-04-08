#pragma once
#include "data_stream.h"
#include<iostream>
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
            call_impl(F &func, T &args);
            template <class R, class F, class T>
            typename std::enable_if<(!std::is_same<R, void>::value), R>::type
            call_impl(F & func, T & args);

            template <class R, class F, class T, size_t... I>
            auto invoke(F&& func, T&& args, std::index_sequence<I...>);

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
        void Function::wrap_impl(std::function<R(Args...)>  func, data_stream &in, data_stream &out)
        {
            using tuple_type = std::tuple<typename std::decay<Args>::type...>;
            constexpr auto size = std::tuple_size<typename std::decay<tuple_type>::type>::value;
            tuple_type args = get_args<tuple_type>(in, make_index_sequence<size>{});
            if (std::is_same<R, void>::value)
            {
                call_impl<R>(func, args);
                return;
            }
             auto p=call_impl<R>(func, args);
            out<<p;
         
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
        Function::call_impl(F& func, T &args)
        {
            constexpr auto size = std::tuple_size<typename std::decay<T>::type>::value;

            invoke<R>(func, args, std::make_index_sequence<size>{});
            return 0;
        }

        template <class R, class F, class T>
        typename std::enable_if<!std::is_same<R, void>::value, R>::type
        Function::call_impl(F &func, T &args)
        {
            constexpr auto size = std::tuple_size<typename std::decay<T>::type>::value;

            return invoke<R>(func, args, std::make_index_sequence<size>{});
        }

        template <class R, class F, class T, size_t... I>
        auto Function::invoke(F &&func, T &&args, std::index_sequence<I...>)
        {
            return func(std::get<I>(std::forward<T>(args))...);
        }

        inline void Function::call(const string &func_name, data_stream &in, data_stream &out)
        {
            if (m_handler.count(func_name))
                m_handler[func_name](in, out);
            return;
        }
    }
}
