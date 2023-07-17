#pragma once

#include <iostream>
#include <cstddef>
#include <algorithm>
#include <functional>
#include <tuple>
#include <string>
#include <stdexcept>

#include "compile_time_arithmetic.hpp"

namespace meta {


    constexpr auto min(auto a, auto b) { return a < b ? a : b; }
    constexpr auto isalpha(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
    constexpr auto isupper(char c) { return c >= 'A' && c <= 'Z'; }
    constexpr auto islower(char c) { return c >= 'a' && c <= 'z'; }
    constexpr auto isdigit(char c) { return c >= '0' && c <= '9'; }
    constexpr auto isspace(char c) { return c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r'; }
    constexpr auto isalnum(char c) { return isalpha(c) || isdigit(c); }
    constexpr auto tolower(char c) { return isupper(c) ? c - 'A' + 'a' : c; }
    constexpr auto toupper(char c) { return islower(c) ? c - 'a' + 'A' : c; }
    
    /* Forward declarations */

    template <std::size_t N> class string;

    // /* Helper functions */
    // static consteval std::size_t length(char) { return 1; }
    // template<std::size_t N>
    // static consteval std::size_t length(const char(&)[N]) { return N - 1; }
    // static consteval std::size_t length(std::string_view sv) { return sv.size(); }
    // template<std::size_t N>
    // static consteval std::size_t length(const string<N> &) { return N; }
    // static consteval std::size_t length(auto... t) requires (sizeof...(t) > 1) { return (length(t) + ...); }


    /* Deduction guides */

    template <std::size_t N>
    string(const char(&)[N]) -> string<N - 1>;
    string(auto... chars)->string<sizeof...(chars)>;
    // string(const std::string_view& sv) -> string<sv.size()>; 
    template <std::size_t... M>
    string(const string<M>&...) -> string<(M + ...)>;


    /* Declarations */

    template <std::size_t N>
    struct string {

        constexpr static auto npos = size_t(-1);

        /* Members */
        char data[N + 1] {};

        /* Constructors */
    public:
        consteval string(auto... chars) requires (sizeof...(chars) == N) && (std::is_same_v<decltype(chars), char> && ...) : data { chars... } { }
        template<std::ranges::sized_range R> consteval string(R &&r) { std::ranges::copy(r, data); }
        consteval string(const char(&s)[N+1]) { std::ranges::copy(s, data); }
    private:
        consteval string(auto... str) requires (sizeof...(str) > 1) && ((str.size() + ...) == N) { auto pos = data; ((pos = std::copy_n(str.data, str.size(), pos)), ...); }

        /* Accessors */
    public:
        consteval auto size() const { return N; }
        consteval auto at(std::size_t i) requires (i < N) { return data[i]; }
        consteval auto operator[](std::size_t i) const { return data[i]; }
        constexpr auto begin() const { return data; }
        constexpr auto end() const { return data + N; }
        constexpr auto rbegin() const { return std::make_reverse_iterator(end()); }
        constexpr auto rend() const { return std::make_reverse_iterator(begin()); }
        consteval auto front() const { return *begin(); }
        consteval auto back() const { return *rbegin(); }

        /* Methods */
    public:


        // slice
        template<auto start, auto count = N - start> requires (start + count <= N)
            consteval auto substr(var<start> = {}, var<count> = {}) const {
            return string<count>{std::ranges::subrange(begin() + start, begin() + start + count)};
        }


        // insert
        template<auto pos, size_t M> requires (pos <= N)
            consteval auto insert(var<pos>, string<M> s) const {
            return substr<0, pos>() + s + substr<pos, N - pos>();
        }
        template<auto pos, size_t M>
        consteval auto insert(var<pos>, const char(&s)[M]) const {
            return insert<pos>({}, string<M - 1>{s});
        }
        template<auto pos>
        consteval auto insert(var<pos>, char c) const {
            return insert<pos>({}, string<1>{c});
        }


        // replace
        template<auto pos, auto count, size_t M> requires (pos + count <= N)
            consteval auto replace(var<pos>, var<count>, string<M> s) const {
            return substr<0, pos>() + s + substr<pos + count, N - pos - count>();
        }

        // search 
        consteval auto find(char c, std::size_t pos = 0) const { return find(string<1>{c}, pos); }
        consteval auto find(const char *s, std::size_t pos = 0) const { return find(std::string_view { s }, pos); }
        consteval auto find(const auto &s, std::size_t pos = 0) const {
            const auto it = std::search(begin() + pos, end(), s.begin(), s.end());
            return it == end() ? npos : std::distance(begin(), it);
        }
        consteval auto rfind(char c, std::size_t pos = N - 1) const { return rfind(string<1>{c}, pos); }
        consteval auto rfind(const char *s, std::size_t pos = N - 1) const { return rfind(std::string_view { s }, pos); }
        consteval auto rfind(const auto &s, std::size_t pos = N - 1) const {
            const auto it = std::search(rbegin() + N - pos - 1, rend(), s.rbegin(), s.rend());
            return it == rend() ? npos : std::distance(it, rend()) - s.size();
        }
        consteval auto find_first_of(const char *s, std::size_t pos = 0) const { return find_first_of(std::string_view { s }, pos); }
        consteval auto find_first_of(const auto &s, std::size_t pos = 0) const {
            const auto it = std::find_first_of(begin() + pos, end(), s.begin(), s.end());
            return it == end() ? npos : std::distance(begin(), it);
        }
        consteval auto find_first_not_of(const char *s, std::size_t pos = 0) const { return find_first_not_of(std::string_view { s }, pos); }
        consteval auto find_first_not_of(const auto &s, std::size_t pos = 0) const {
            const auto it = std::find_if_not(begin() + pos, end(), [&s](char c) { return std::ranges::find(s, c) != s.end(); });
            return it == end() ? npos : std::distance(begin(), it);
        }
        consteval auto find_last_of(const char *s, std::size_t pos = N - 1) const { return find_last_of(std::string_view { s }, pos); }
        consteval auto find_last_of(const auto &s, std::size_t pos = N - 1) const {
            const auto it = std::find_first_of(rbegin() + N - pos - 1, rend(), s.begin(), s.end());
            return it == rend() ? npos : std::distance(it, rend()) - 1;
        }
        consteval auto find_last_not_of(const char *s, std::size_t pos = N - 1) const { return find_last_not_of(std::string_view { s }, pos); }
        consteval auto find_last_not_of(const auto &s, std::size_t pos = N - 1) const {
            const auto it = std::find_if_not(rbegin() + N - pos - 1, rend(), [&s](char c) { return std::ranges::find(s, c) != s.end(); });
            return it == rend() ? npos : std::distance(it, rend()) - 1;
        }

        consteval auto starts_with(const auto &s) const { return std::equal(s.begin(), s.end(), begin()); }
        consteval auto starts_with(const char *s) const { return starts_with(std::string_view { s }); }

        consteval auto ends_with(const auto &s) const { return std::equal(s.rbegin(), s.rend(), rbegin()); }
        consteval auto ends_with(const char *s) const { return ends_with(std::string_view { s }); }

        consteval auto contains(const auto &s) const { return find(s) != npos; }
        consteval auto contains(const char *s) const { return contains(std::string_view { s }); }

        // consteval auto lower() const { 
        //     // the following code is not constexpr
        //     // return string<N>{std::ranges::transform_view{ *this, tolower }};
        //     for (std::size_t i = 0; i < N; ++i) {
        //         if (isupper(data[i])) {
        //             return 
        //         }
        //     }
            
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


        // constexpr auto to_string_view() const { return std::string_view { data }; }

        /* Conversion operators */
    public:


        /* Friends */
        template<std::size_t M> friend class string;
        friend std::ostream &operator<<(std::ostream &os, const string &s) { return os << s.data; }

    };


    namespace literals {
        template <string s>
        consteval auto operator""_s() { return s; }
        consteval auto operator""_s(char c) { return string { c }; }
    }

    template <std::size_t N>
    struct string_literal {
        char data[N] {};
        constexpr string_literal(const char (&s) [N]) { std::ranges::copy(s, data); }
        friend std::ostream &operator<<(std::ostream &os, const string_literal &s) { return os << s.data; }
    };

    template<size_t N, string_literal<N> S>
    struct var<S> {
        void print() {
            std::cout << "Specialization for class1" << std::endl;
        }
    };






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

