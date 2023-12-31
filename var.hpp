#pragma once

#include <optional>

namespace meta {

    template <auto V=std::nullopt>
    struct var {
        using value_type = decltype(V);
        static constexpr value_type value = V;
        consteval operator value_type () const { return V; }

        #define BIN_OP(op) \
            template <auto W> \
            consteval auto operator op(var<W>) const \
            requires requires { V op W; }  { \
                return var<(V op W)>{}; \
            }
            BIN_OP(+) BIN_OP(-) BIN_OP(*) BIN_OP(/) BIN_OP(%) 
            BIN_OP(^) BIN_OP(&) BIN_OP(|) BIN_OP(<<) BIN_OP(>>) BIN_OP(&&) BIN_OP(||)
            BIN_OP(==) BIN_OP(!=) BIN_OP(<) BIN_OP(>) BIN_OP(<=) BIN_OP(>=) 
        #undef BIN_OP

        #define UN_OP(op) \
            consteval auto operator op() const \
            requires requires { op V; } { \
                return var<(op V)>{}; \
            }
            UN_OP(+) UN_OP(-) UN_OP(!) UN_OP(~)
        #undef UN_OP

        // operator []
        template <auto I>
        consteval auto operator[](var<I>) const {
            if constexpr (std::is_void_v<decltype(V[I])>)
                return var<>{};
            else
                return var<V[I]>{};
        }

        // operator ()
        template <auto... I>
        consteval auto operator()(var<I>...) const {
            if constexpr (std::is_void_v<decltype(V(I...))>)
                return var<>{};
            else
                return var<V(I...)>{};
        }
    };

    template<auto V>
    auto &operator<<(auto &os, const var<V> &v) { 
        if constexpr (std::is_same_v<std::decay_t<decltype(V)>, std::nullopt_t>)
            return os << "None";
        else
            return os << V; 
    }

    template <auto V>
    consteval auto is_var(var<V>) { return true; }

    template <typename T>
    consteval auto is_var(T) { return false; }

    // template <auto V>
    // concept Var = is_var(var<V>{});


    template <typename T>
    consteval auto eval(T t) {
        if constexpr (requires { T::value; }) {
            if constexpr (is_var(T{}))
                return eval(T::value);
            else 
                return t;
        } else {
            return t;
        }
    }

    #define VAR_LITERAL(T, L) \
        template <char... C> \
        consteval T parse_as_ ## L() { \
            float res = 0; \
            float frac = 1; \
            bool is_frac = false; \
            return ((C == '.' ? is_frac = true : (is_frac ? (frac /= 10, res += (C - '0') * frac) : (res = res * 10 + (C - '0')))), ...); \
        } \
        inline namespace literals { \
            template <char... C> \
            consteval auto operator"" _ ## L() { return var<parse_as_ ## L <C...>()>{}; } \
        }
        VAR_LITERAL(int, i)
        VAR_LITERAL(unsigned, u)
        VAR_LITERAL(long, l)
        VAR_LITERAL(unsigned long, ul)
        VAR_LITERAL(long long, ll)
        VAR_LITERAL(unsigned long long, ull)
        VAR_LITERAL(float, f)
        VAR_LITERAL(double, d)
        VAR_LITERAL(long double, ld)
    #undef VAR_LITERAL


    constexpr auto None = var<>{};

    // template <auto T>
    // requires (is_var(T))
    // struct var<T> : decltype(T) {};
}
