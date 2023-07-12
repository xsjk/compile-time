#pragma once

#include <cstddef>
#include <algorithm>
#include <functional>
#include <tuple>
#include <string>
#include <stdexcept>

#include "compile_time_string.hpp"

namespace meta {

    /* Forward declarations */

    template <std::size_t N> class string;

    /* Helper functions */
    static consteval std::size_t length(char) { return 1; }
    template<std::size_t N>
    static consteval std::size_t length(const char(&)[N]) { return N - 1; }
    static consteval std::size_t length(std::string_view sv) { return sv.size(); }
    template<std::size_t N>
    static consteval std::size_t length(const string<N> &) { return N; }
    static consteval std::size_t length(auto... t) requires (sizeof...(t) > 1) { return (length(t) + ...); }

    constexpr auto min(auto a, auto b) { return a < b ? a : b; }

    /* Deduction guides */

    template <std::size_t N>
    string(const char(&)[N]) -> string<N - 1>;
    string(auto... chars)->string<sizeof...(chars)>;
    // string(const std::string_view& sv) -> string<sv.size()>; 
    template <std::size_t... M>
    string(const string<M>&...) -> string<(M + ...)>;


    /* Declarations */


    template<std::size_t N>
    struct string_literal_helper {
        char _[N] {};
        consteval string_literal_helper(const char(&str)[N]) { std::copy_n(str, N, _); }
    };

    template <std::size_t N>
    class string {

        /* Members */
    private:
        char _data[N + 1] {};

        /* Constructors */
    public:
        consteval string(const std::string_view &sv) : string(sv.begin(), sv.end()) { }
        consteval string(const char(&str)[N + 1]) : string(str, str + N) { }
        consteval string(auto... chars) requires (sizeof...(chars) == N) && (std::is_same_v<decltype(chars), char> && ...) : _data { chars... } { }

    private:

        // this constructor is for concatenation
        consteval string(auto... str) requires (sizeof...(str) > 1) && ((str.size() + ...) == N) { auto pos = _data; ((pos = std::copy_n(str.data(), str.size(), pos)), ...); }
        consteval string(auto... sv) requires (sizeof...(sv) > 1) && (std::is_same_v<decltype(sv), std::string_view> && ...) 
         { auto pos = _data; ((pos = std::copy(sv.begin(), sv.end(), pos)), ...); }


        template<class InputIt>
        consteval string(InputIt first, InputIt last)
            requires (std::is_same_v<std::decay_t<decltype(*std::declval<InputIt>())>, char>) {
            if (N != std::distance(first, last))
                throw std::length_error("string: length mismatch");
            std::copy(first, last, _data);
        }


        /* Accessors */
    public:
        constexpr auto data() const { return _data; }
        consteval auto size() const { return N; }
        consteval auto at(std::size_t i) requires (i < N) { return _data[i]; }
        consteval auto operator[](std::size_t i) const { return _data[i]; }
        constexpr auto begin() const { return data(); }
        constexpr auto end() const { return data() + N; }
        constexpr auto rbegin() const { return std::make_reverse_iterator(end()); }
        constexpr auto rend() const { return std::make_reverse_iterator(begin()); }
        consteval auto front() const { return *begin(); }
        consteval auto back() const { return *rbegin(); }

        /* Methods */
    public:
        // consteval auto insert(std::size_t pos, char c) {
        //     return string<N + 1> {
        //         std::string_view { _data, pos },
        //         c,
        //         std::string_view { _data + pos, N - pos }
        //     };
        // }
        // consteval auto insert(std::size_t pos, auto s) requires (std::is_constructible_v<string, decltype(s)> && std::is_same_v<decltype(s), string<s.size()>>) {
        //     string<N + s.size()> result {
        //         substr(0, pos),
        //         s,
        //         substr(pos, N)
        //     }
        // }

        // find
        consteval auto find(char c, std::size_t pos = 0) const {
            const auto it = std::find(begin() + pos, end(), c);
            return it == end() ? -1 : std::distance(begin(), it);
        }
        consteval auto find(const auto &s, std::size_t pos = 0) const {
            const auto it = std::search(begin() + pos, end(), s.begin(), s.end());
            return it == end() ? -1 : std::distance(begin(), it);
        }
        consteval auto find(const char *s, std::size_t pos = 0) const {
            return find(std::string_view { s }, pos);
        }

        // substr
        template<size_t start, size_t count = N - start>
        consteval auto substr() const {
            static_assert(start < N, "start must be less than N");
            static_assert(start + count <= N, "start + count must be less than or equal to N");
            return string<count>{begin() + start, begin() + start + count};
        }

        // consteval auto substr(size_t start, size_t count = M) const {
        //     return string<M>{begin() + start, begin() + start + count};
        // }


    /* Operators */
    public:

        template <std::size_t M> consteval auto operator==(const string<M> &rhs) const { return std::equal(begin(), end(), rhs.begin(), rhs.end()); }
        template <std::size_t M> consteval auto operator<=>(const string<M> &rhs) const { return std::lexicographical_compare_three_way(begin(), end(), rhs.begin(), rhs.end()); }
        template <std::size_t M> consteval auto operator+(const string<M> &rhs) const { return string<N + M>(*this, rhs); }
        consteval auto operator+(char c) const { return *this + string<1>{c}; }
        template <std::size_t M> consteval auto operator+(const char(&rhs)[M]) const { return *this + string<M - 1>{rhs}; }
        friend consteval auto operator+(char lhs, const string<N> &rhs) { return rhs + lhs; }
        template <std::size_t M> friend consteval auto operator+(const char(&lhs)[M], const string<N> &rhs) { return rhs + lhs; }


        // constexpr auto to_string_view() const { return std::string_view { _data }; }

        /* Conversion operators */
    public:


        /* Friends */
        template<std::size_t M> friend class string;
        friend std::ostream &operator<<(std::ostream &os, const string &s) { return os << s._data; }

    };


    namespace literals {
        template <string_literal_helper s>
        consteval auto operator""_ss() { return string { s._ }; }
        consteval auto operator""_ss(char c) { return string { c }; }
    }





    // template <char... C>
    // struct static_string {
    //     static constexpr char value[sizeof...(C) + 1] = {C..., '\0'};

    //     template<char ... Pattern>
    //     static consteval int _find() {
    //         constexpr auto pattern = static_string<Pattern...>::value;
    //         auto it = std::search(value, value + sizeof...(C), pattern, pattern + sizeof...(Pattern));
    //         return it == value + sizeof...(C) ? -1 : std::distance(value, it);
    //     }
    //     template<char ... Pattern> static constexpr auto find = _find<Pattern...>();

    //     template <std::size_t I>
    //     static consteval char at() { return value[I]; }

    //     static consteval std::size_t size() { return sizeof...(C); }

    //     template <std::size_t Start, std::size_t Count = sizeof...(C) - Start>
    //     static consteval auto _substr() {
    //         static_assert(Start < sizeof...(C), "Start must be less than sizeof...(C)");
    //         static_assert(Start + Count <= sizeof...(C), "Start + Count must be less than or equal to sizeof...(C)");
    //         return static_string<value[Start + I]...>{};
    //     }
    //     template <std::size_t Start, std::size_t Count = sizeof...(C) - Start>
    //     using substr = decltype(_substr<Start, Count>());


    //     template <char... D>
    //     static consteval auto operator+(static_string<D...>) {
    //         return static_string<C..., D...>{};
    //     }



    // };

    // template <char... C>
    // std::ostream &operator<<(std::ostream &os, const static_string<C...> &) { 
    //     return os << static_string<C...>::value; 
    // }

}

