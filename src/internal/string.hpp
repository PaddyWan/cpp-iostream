#include <cstddef>
#include <cstring>
#include <string>

using std::size_t;

size_t string_size_internal(const std::string& s);
size_t string_size_internal(const char* cstring);

const char* string_cstring_internal(const std::string& s);
const char* string_cstring_internal(const char* cstring);

const char* string_data_internal(const std::string& s);
const char* string_data_internal(const char* cstring);

std::back_insert_iterator<std::string> string_inserter_internal(std::string& s);
