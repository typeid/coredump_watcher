#include "../include/filename_validation.hpp"
#include <regex>
#include <string>

bool is_trigger_string(std::string_view file_name) {
  const std::regex e(R"(^core\.[a-zA-Z]+\.(([a-zA-Z]|[[:digit:]])+\.)+lz4$)");

  return std::regex_match(std::string(file_name), e);
}