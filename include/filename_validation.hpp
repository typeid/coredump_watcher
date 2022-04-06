#pragma once

#include <string_view>

/**
 * Checks if the @p file_name matches the following regexp:
 * ^core\.[a-zA-Z]+\.(([a-zA-Z]|[[:digit:]])+\.)+lz4$
 **/
[[nodiscard]] bool is_trigger_string(std::string_view file_name);