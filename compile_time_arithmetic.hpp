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
            BIN_OP(^) BIN_OP(&) BIN_OP(|) BIN_OP(<<) BIN_OP(>>)
            BIN_OP(==) BIN_OP(!=) BIN_OP(<) BIN_OP(>) BIN_OP(<=) BIN_OP(>=) 
        #undef BIN_OP
        friend std::ostream &operator<<(std::ostream &os, const var &v) { return os << v.value; }
    };

    
    template <char... C>
    consteval int parseDigits() {
        auto r = 0;
        return ((r = r * 10 + (C - '0')), ...);
    }

    inline namespace literals {
        template <char... C>
        consteval auto operator"" _v() { return var<parseDigits<C...>()>{}; }
    }

}
