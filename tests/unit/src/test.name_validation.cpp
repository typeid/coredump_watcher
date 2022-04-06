#include <gtest/gtest.h>
#include <string_view>

#include "../../../include/filename_validation.hpp"

TEST(filename_validation, validates_correct_file_name) {
  constexpr std::string_view trigger_file_name{
      "core.ServiceName.3057.57dd721409bc4ab4b38a3c33a36a608a.3717."
      "1647975805000000.lz4"};

  EXPECT_TRUE(is_trigger_string(trigger_file_name));
}

TEST(filename_validation, validates_correct_file_name_second) {
  constexpr std::string_view trigger_file_name{"core.MyService.1h7x.192df.3717."
                                               "1647975805000000.lz4"};

  EXPECT_TRUE(is_trigger_string(trigger_file_name));
}

TEST(filename_validation, validates_correct_file_name_single_hex) {
  constexpr std::string_view trigger_file_name{"core.Mi.1647975805000000.lz4"};

  EXPECT_TRUE(is_trigger_string(trigger_file_name));
}

TEST(filename_validation, missing_service_name) {
  constexpr std::string_view trigger_file_name{
      "core.3057.57dd721409bc4ab4b38a3c33a36a608a.3717."
      "1647975805000000.lz4"};

  EXPECT_FALSE(is_trigger_string(trigger_file_name));
}

TEST(filename_validation, missing_lz4_ending) {
  constexpr std::string_view trigger_file_name{
      "core.3057.57dd721409bc4ab4b38a3c33a36a608a.3717."
      "1647975805000000.lz4"};

  EXPECT_FALSE(is_trigger_string(trigger_file_name));
}

TEST(filename_validation, missing_core) {
  constexpr std::string_view trigger_file_name{
      "ServiceName.3057.57dd721409bc4ab4b38a3c33a36a608a.3717."
      "1647975805000000.lz4"};

  EXPECT_FALSE(is_trigger_string(trigger_file_name));
}

TEST(filename_validation, empty_string) {
  constexpr std::string_view trigger_file_name{""};

  EXPECT_FALSE(is_trigger_string(trigger_file_name));
}

TEST(filename_validation, no_hex) {
  constexpr std::string_view trigger_file_name{"core.ServiceName.lz4"};

  EXPECT_FALSE(is_trigger_string(trigger_file_name));
}

TEST(filename_validation, nothing_inbetween_dots) {
  constexpr std::string_view trigger_file_name{"core.ServiceName.a123..lz4"};

  EXPECT_FALSE(is_trigger_string(trigger_file_name));
}