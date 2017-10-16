#ifndef STRING_HPP
#define STRING_HPP

#include <iterator>
#include <string>

#include "string.ipp"
#include "internal/string.hpp"

template<String str>
size_t string_size(str&& s)
{
    return string_size_internal(s);
}

template<String str>
const char* string_cstring(str&& s)
{
    return string_cstring_internal(s);
}

template<String str>
const char* string_data(str&& s)
{
    return string_data_internal(s);
}

template<String str>
auto string_inserter(str&& s)
{
    return string_inserter_internal(s);
}

template<String str1, String str2>
bool string_compare(str1&& s1, str2&& s2)
{
    return std::equal(string_data(s1), string_data(s1) + string_size(s1), string_data(s2), string_data(s2) + string_size(s2));
}

#endif//STRING_HPP
