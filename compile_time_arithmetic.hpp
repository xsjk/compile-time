#include <iostream>


namespace meta {

    template <auto V> struct var { 
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
            return var<(V[I])>{};
        }

        // operator ()
        template <auto... I>
        consteval auto operator()(var<I>...) const {
            return var<(V(I...))>{};
        }

        friend std::ostream &operator<<(std::ostream &os, const var &v) { return os << v.value; }
    };

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

}
