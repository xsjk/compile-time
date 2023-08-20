#pragma once

#include "var.hpp"
#include "utils.hpp"
#include <tuple>
#include <type_traits>

namespace meta {

    template <auto ...V>
    struct tuple;


    template <typename T>
    consteval auto is_tuple(T) { return false; }

    template <auto ...V>
    consteval auto is_tuple(tuple<V...>) { return true; }

    template <auto ...V>
    struct tuple {

        static constexpr auto value = std::tuple{V...};

        /* methods */

        // at
        template<auto I>
        static consteval auto at(var<I> i={}) { 
            static_assert(I < size(), "at: index out of range");
            return var<std::get<I>(value)>{}; 
        }

        // size
        static consteval auto size() { return sizeof...(V); }

        // self
        static consteval auto self() { return tuple<V...>{}; }

        // map
        template<typename F>
        static consteval auto map(F f) { return tuple<eval(F{}(var<V>{}))...>{}; }
        static constexpr auto for_each(auto f) { (f(var<V>{}), ...); return self(); }

        /* operators */

        // add
        template <auto ...W>
        consteval auto operator+(tuple<W...>) const { return tuple<V..., W...>{}; }

        // ostream
        friend std::ostream& operator<<(std::ostream& os, tuple t) {
            if constexpr (t.size() == 0) return os << "()";
            else if constexpr (t.size() == 1) return os << '(' << t[0_i] << ",)";
            os << '(';
            t.for_each([&os](auto v) { os << v << ", "; });
            return os << "\b\b)";
        }

    };


    consteval auto range(auto, auto, auto);

    template <auto ...V>
    consteval auto make_tuple(var<V>...);


    template <auto T>
    requires (is_tuple(T))
    struct var<T> {

        using value_type = decltype(T);
        static constexpr value_type value = T;
        consteval operator value_type () const { return T; }

        // at
        template<auto I>
        static consteval auto at(var<I> i={}) { 
            static_assert(I < size(), "at: index out of range");
            return var<std::get<I>(T.value)>{}; 
        }

        static consteval auto size() { return var<T.size()>{}; }
        static consteval auto empty() { return size() == 0_i; }
        static consteval auto front() { return at(0_i); }
        static consteval auto back() { return at(size() - 1_i); }
        static consteval auto self() { return var<T>{}; }
        static constexpr auto for_each(auto f) { return T.for_each(f), self(); }
        static consteval auto map(auto f) { return var<T.map(f)>{}; }

        // reduce
        static consteval auto reduce(auto f) {
            static_assert(size() > 0, "reduce: empty tuple can not be reduced");
            if constexpr (size() == 1)
                return front();
            else
                return f(slice(0_i, size() - 1_i).reduce(f), back());
        }
        static consteval auto sum() { return reduce([](auto a, auto b) { return a + b; }); }
        static consteval auto min() { return reduce([](auto a, auto b) { return a < b ? a : b; }); }
        static consteval auto max() { return reduce([](auto a, auto b) { return a > b ? a : b; }); }

        // accumulate
        static consteval auto accumulate(auto f) {
            static_assert(size() > 0, "accumulate: empty tuple can not be accumulated");
            if constexpr (size() == 1) 
                return make_tuple(front());
            else {
                auto prev = slice(0_i, size() - 1_i).accumulate(f);
                return prev + make_tuple(f(prev.back(), back()));
            }
        }

        // filter
        static consteval auto filter(auto f) {
            if constexpr (size() == 0)
                return self();
            else if constexpr (f(back()))
                return slice(0_i, size() - 1_i).filter(f) + make_tuple(back());
            else
                return slice(0_i, size() - 1_i).filter(f);
        }

        // slice
        template<auto B, auto E, auto D = 1>
        static consteval auto slice(var<B> begin, var<E> end, var<D> diff={}) {
            return range(begin, end, diff).map([](auto i) { return at(i); });
        }

        // sort
        static consteval auto sort() {
            return sort([](auto a, auto b) { return a < b; });
        }
        static consteval auto sort(auto f) {
            if constexpr (size() <= 1) 
                return self();
            else {
                constexpr auto pivot = back();
                auto rest = slice(0_i, len(self()) - 1_i);
                auto left = rest.filter(var<[=](auto v) { return f(v, pivot); }>{}).sort(f);
                auto right = rest.filter(var<[=](auto v) { return !f(v, pivot); }>{}).sort(f);
                return left + make_tuple(pivot) + right;
            }
        }

        static consteval auto append(auto value) { return insert(size(), value); }
        static consteval auto prepend(auto value) { return insert(0_i, value); }
        static consteval auto insert(auto index, auto value) { return slice(0_i, index) + make_tuple(value) + slice(index, size()); }

        /* operators */

        consteval auto operator+(auto arg) const { return var<T + arg.value>{}; }
        consteval auto operator*(auto n) const {
            if constexpr (n == 0_i) return make_tuple();
            else if constexpr (n == 1_i) return self();
            else return self() + (self() * (n - 1_i));
        }
        consteval auto operator[](auto i) const { return T.at(i); }
        
    };


    template <auto ...V>
    consteval auto make_tuple(var<V>...) { 
        static_assert((!(is_var(V)) && ...), "v is already a var");
        return var<tuple<V...>{}>{}; 
    }

    consteval auto range(auto begin, auto end, auto diff) {
        static_assert(diff != 0_i, "range: step cannot be 0");
        if constexpr (diff > 0_i && begin >= end || diff < 0_i && begin <= end) return make_tuple();
        else return make_tuple(begin) + range(begin + diff, end, diff);
    }
    consteval auto range(auto begin, auto end) { return range(begin, end, 1_i); }
    consteval auto range(auto end) { return range(0_i, end); }

}