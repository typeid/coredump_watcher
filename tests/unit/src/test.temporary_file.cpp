#include <fstream>
#include <gtest/gtest.h>
#include <string_view>

#include "../../../include/temporary_file.hpp"

TEST(temporary_file, correct_content) {
  constexpr std::string_view file_content("test");
  temporary_file tmp_file;
  tmp_file.write(file_content);

  std::ifstream file(tmp_file.get_path());

  std::stringstream buffer;
  buffer << file.rdbuf();

  EXPECT_EQ(file_content, buffer.str());
}
