#pragma once

#include "var.hpp"
#include "len.hpp"
#include <tuple>
#include <type_traits>

namespace meta {

    template <auto ...V>
    struct tuple;


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

        // from std::integer_sequence
        template<typename T, T ...W>
        requires (sizeof...(V) == sizeof...(W))
        constexpr tuple(std::integer_sequence<T, W...>) : tuple<W...>{var<W>{}...} {}

        // from <brace-enclosed initializer list>


        /* methods */

        // size
        static consteval auto size() { return sizeof...(V); }

        // empty
        static consteval auto empty() { return size() == 0; }


        // for each
        template<typename F, auto ...W>
        static constexpr void for_each(F&& f, tuple<W...> t) { (f(var<std::get<W>(value)>{}), ...); }
        template<typename F>
        static constexpr void for_each(F&& f) { (f(var<V>{}), ...); }

        // map
        template<typename F, auto ...W>
        static consteval auto map(F, tuple<W...> t) { return tuple<F{}(var<std::get<W>(value)>{})...>{}; }
        template<typename F>
        static consteval auto map(F f) { return tuple<F{}(var<V>{})...>{}; }

        // slice
        template<auto B, auto E, auto D = 1>
        static consteval auto slice(var<B> begin, var<E> end, var<D> diff={}) {
            return range(begin, end, diff).map([](auto i) { return var<std::get<i>(value)>{}; });
        }

        template<auto W>
        static consteval auto append(var<W>) { return tuple<V..., W>{var<V>{}..., var<W>{}}; }

        template<auto W>
        static consteval auto prepend(var<W>) { return tuple<W, V...>{var<W>{}, var<V>{}...}; }


        /* operators */

        // add
        template <auto ...W>
        consteval auto operator+(tuple<W...>) const { return tuple<V..., W...>{var<V>{}..., var<W>{}...}; }

        // mul
        template <auto N>
        consteval auto operator*(var<N> n) const {
            if constexpr (n == 0_i) return tuple<>{};
            else if constexpr (n == 1_i) return *this;
            else return *this + (*this * (n - 1_i));
        }

        // get
        template <auto I>
        consteval auto operator[](var<I> i) const { return var<std::get<I>(value)>{}; }

        // ostream
        friend std::ostream& operator<<(std::ostream& os, tuple t) {
            if constexpr (t.size() == 0) return os << "()";
            else if constexpr (t.size() == 1) return os << '(' << t[0_i] << ",)";
            os << '(';
            t.for_each([&os](auto v) { os << v << ", "; });
            return os << "\b\b)";
        }

        template <auto ...W>
        requires (sizeof...(V) == sizeof...(W)) && (std::is_same_v<var<V>, var<W>> && ...)
        constexpr auto operator==(tuple<W...>) const { return true; }


    };

    template <auto B, auto E, auto D = 1>
    consteval auto range(var<B> begin, var<E> end, var<D> diff={}) {
        static_assert(D != 0_i, "range: step cannot be 0");
        if constexpr (D > 0_i && B >= E || D < 0_i && B <= E) return tuple<>{};
        else return tuple{begin} + range(begin + diff, end, diff);
    }

    template <auto E>
    consteval auto range(var<E> end) { return range(0_i, end); }


}