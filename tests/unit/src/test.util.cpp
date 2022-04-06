#include <algorithm>
#include <array>
#include <chrono>
#include <gtest/gtest.h>
#include <thread>
#include <vector>

#include "../../../include/util.hpp"

TEST(util, split_with_delimiter_multiple) {
  const std::array<std::string, 3> expected_tokens = {"one", "two", "three"};
  const auto tokens = split_with_delimiter("one,two,three", ',');
  EXPECT_EQ(expected_tokens.size(), tokens.size());

  for (std::size_t i = 0; i < expected_tokens.size(); ++i) {
    EXPECT_EQ(expected_tokens.at(i), tokens.at(i));
  }
}

TEST(util, split_with_delimiter_multiple_more) {
  const std::array<std::string, 6> expected_tokens = {
      "one", "two", "three", "foooooooobar", "b", "barfoo"};
  const auto tokens =
      split_with_delimiter("one,two,three,foooooooobar,b,barfoo", ',');
  EXPECT_EQ(expected_tokens.size(), tokens.size());

  for (std::size_t i = 0; i < expected_tokens.size(); ++i) {
    EXPECT_EQ(expected_tokens.at(i), tokens.at(i));
  }
}

TEST(util, split_with_delimiter_single_token) {
  const auto tokens = split_with_delimiter("one", ',');
  EXPECT_EQ(1, tokens.size());
  EXPECT_EQ("one", tokens.at(0));
}

TEST(util, split_with_delimiter_no_token) {
  const auto tokens = split_with_delimiter("", ',');
  EXPECT_EQ(0, tokens.size());
}

TEST(util, split_with_delimiter_irrelevant_delimiter) {
  const auto tokens = split_with_delimiter("test,", ',');
  EXPECT_EQ(1, tokens.size());
  EXPECT_EQ("test", tokens.at(0));
}

TEST(util, split_with_delimiter_empty_values) {
  const auto tokens = split_with_delimiter("test,,aab", ',');
  EXPECT_EQ(2, tokens.size());
  EXPECT_EQ("test", tokens.at(0));
  EXPECT_EQ("aab", tokens.at(1));
}

TEST(util, subprocess_return_string) {
  const std::string echo_string = "test";
  const auto echo_return_value =
      execute_subprocess("echo '" + echo_string + '\'');
  std::cout << echo_return_value << std::endl;
  EXPECT_EQ(echo_string + '\n', echo_return_value);
}

TEST(util, generate_unique_file_name) {
  std::array<std::string, 1000> unique_file_names;

  for (std::size_t i = 0; i < unique_file_names.size(); ++i) {
    unique_file_names[i] = generate_unique_file_name();
  }

  std::sort(std::begin(unique_file_names), std::end(unique_file_names));
  const auto pos = std::adjacent_find(std::begin(unique_file_names),
                                      std::end(unique_file_names));
  EXPECT_EQ(pos, std::end(unique_file_names));
}
