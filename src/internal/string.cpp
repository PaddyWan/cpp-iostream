#include "string.hpp"

size_t string_size_internal(const std::string& s)
{
    return s.size();
}
size_t string_size_internal(const char* cstring)
{
    return std::strlen(cstring);
}

const char* string_cstring_internal(const std::string& s)
{
    return s.c_str();
}
const char* string_cstring_internal(const char* cstring)
{
    return cstring;
}

const char* string_data_internal(const std::string& s)
{
    return s.data();
}
const char* string_data_internal(const char* cstring)
{
    return cstring;
}

std::back_insert_iterator<std::string> string_inserter_internal(std::string& s)
{
    return back_inserter(s);
}
