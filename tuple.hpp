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
    tuple(var<V>...) -> tuple<V...>;

    template <typename T, T ...V>
    tuple(std::integer_sequence<T, V...>) -> tuple<V...>;


    template <auto ...V>
    struct tuple {

        static constexpr auto value = std::tuple{eval(V)...};

        /* constructors */
        constexpr tuple() = default;

        // from vars
        template<auto ...W>
        requires (sizeof...(V) == sizeof...(W)) && (std::is_same_v<var<V>, var<W>> && ...)
        constexpr tuple(var<W>...) {}

        // from any
        constexpr tuple(auto ...w) : tuple{var<eval(w)>{}...} {}

        // from std::integer_sequence
        template<typename T, T ...W>
        requires (sizeof...(V) == sizeof...(W))
        constexpr tuple(std::integer_sequence<T, W...>) : tuple<W...>{var<W>{}...} {}


        /* methods */

        // at
        template<auto I>
        static consteval auto at(var<I> i={}) { 
            static_assert(I < size(), "at: index out of range");
            return var<std::get<I>(value)>{}; 
        }

        // size
        static consteval auto size() { return sizeof...(V); }

        // empty
        static consteval auto empty() { return size() == 0; }

        // front
        static consteval auto front() { return at(0_i); }

        // back
        static consteval auto back() { return at(len(self()) - 1_i); }

        // self
        static consteval auto self() { return tuple<V...>{}; }

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
                return left + tuple<pivot.value>{} + right;
            }
        }

        // for each
        template<typename F, auto ...W>
        static constexpr auto for_each(F&& f, tuple<W...> t) { (f(at<W>()), ...); return self(); }
        template<typename F>
        static constexpr auto for_each(F&& f) { (f(var<V>{}), ...); return self(); }

        // map
        template<typename F, auto ...W>
        static consteval auto map(F, tuple<W...> t) { return tuple<F{}(at<W>())...>{}; }
        template<typename F>
        static consteval auto map(F f) { return tuple<F{}(var<V>{})...>{}; }

        // reduce
        template<typename F>
        static consteval auto reduce(F f) {
            static_assert(size() > 0, "reduce: tuple must have at least one element");
            if constexpr (size() == 1) 
                return front();
            else if constexpr (size() > 1) 
                return f(front(), slice(1_i, len(self())).reduce(f));
        }
        template<typename F>
        static consteval auto reduce(F f, auto init) { return prepend(init).reduce(f); }

        // sum
        static consteval auto sum() { return reduce([](auto a, auto b) { return a + b; }); }

        // min
        static consteval auto min() { return reduce([](auto a, auto b) { return a < b ? a : b; }); }

        // max
        static consteval auto max() { return reduce([](auto a, auto b) { return a > b ? a : b; }); }


        // accumulate
        template<typename F>
        static consteval auto accumulate(F f) {
            static_assert(size() > 0, "accumulate: tuple must have at least one element");
            if constexpr (size() == 1) 
                return tuple<front()>{};
            else {
                auto prev = slice(0_i, len(self()) - 1_i).accumulate(f);
                return prev + tuple<F{}(prev.back(), back())>{};
            }
        }
        template<typename F>
        static consteval auto accumulate(F f, auto init) { return prepend(init).accumulate(f); }

        // filter
        template<typename F>
        static consteval auto filter(F f) {
            if constexpr (size() == 0) 
                return self();
            else if constexpr (F{}(back())) {
                auto prev = slice(0_i, len(self()) - 1_i).filter(f);
                return prev + tuple<back().value>{};
            } else 
                return slice(0_i, len(self()) - 1_i).filter(f);
        }

        // slice
        template<auto B, auto E, auto D = 1>
        static consteval auto slice(var<B> begin, var<E> end, var<D> diff={}) {
            return range(begin, end, diff).map([](auto i) { return at(i); });
        }

        // search



        // append
        template<auto W>
        static consteval auto append(var<W> w) { return insert(len(self()), w); }

        // prepend
        template<auto W>
        static consteval auto prepend(var<W> w) { return insert(0_i, w); }

        // insert
        template<auto I, auto W>
        static consteval auto insert(var<I> i, var<W> w) { return slice(0_i, i) + tuple<W>{} + slice(i, len(self())); }


        /* operators */

        // add
        template <auto ...W>
        consteval auto operator+(tuple<W...>) const { return tuple<V..., W...>{}; }

        // mul
        template <auto N>
        consteval auto operator*(var<N> n) const {
            if constexpr (n == 0_i) return tuple<>{};
            else if constexpr (n == 1_i) return self();
            else return self() + (self() * (n - 1_i));
        }

        // get
        template <auto I>
        consteval auto operator[](var<I> i) const { return at(i); }

        // ostream
        friend std::ostream& operator<<(std::ostream& os, tuple t) {
            if constexpr (t.size() == 0) return os << "()";
            else if constexpr (t.size() == 1) return os << '(' << t[0_i] << ",)";
            os << '(';
            t.for_each([&os](auto v) { os << v << ", "; });
            return os << "\b\b)";
        }

        // compare
        

    };


    template <auto B, auto E, auto D = 1>
    consteval auto range(var<B> begin, var<E> end, var<D> diff={}) {
        static_assert(D != 0_i, "range: step cannot be 0");
        if constexpr (D > 0_i && B >= E || D < 0_i && B <= E) return tuple<>{};
        else return tuple{begin} + range(begin + diff, end, diff);
    }

    template <auto E>
    consteval auto range(var<E> end) { return range(0_i, end); }


    template <auto T>
    requires (is_tuple(T))
    struct var<T> : decltype(T) {};

    template <auto ...V>
    consteval auto make_tuple(var<V>...) { return var<tuple<eval(V)...>{}>{}; }

}