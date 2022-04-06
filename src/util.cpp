#include "../include/util.hpp"

#include <array>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <chrono>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <memory>

std::vector<std::string> split_with_delimiter(std::string_view sequence,
                                              char delimiter) {
  std::vector<std::string> tokens{};
  std::size_t token_start = 0;
  std::size_t token_end = sequence.size();

  while ((token_end = sequence.find(delimiter, token_start)) !=
         std::string_view::npos) {

    const auto token = sequence.substr(token_start, token_end - token_start);
    if (!token.empty()) {
      tokens.emplace_back(token);
    }

    token_start = token_end + 1;
  }

  const auto final_element = sequence.substr(token_start, token_end);
  if (!final_element.empty()) {
    tokens.emplace_back(final_element);
  }
  return tokens;
}

std::string execute_subprocess(const std::string &subprocess) {
  std::array<char, 1024> buffer;
  std::string result;

  std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(subprocess.c_str(), "r"),
                                                pclose);
  if (!pipe) {
    throw std::runtime_error("Failed executing subprocess: popen() failed!");
  }
  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
    result += buffer.data();
  }
  return result;
}

std::string get_disk_usage_report() { return execute_subprocess("df -T"); }

std::string get_current_time_string() {

  const auto now = std::chrono::system_clock::now();
  const auto in_time_t = std::chrono::system_clock::to_time_t(now);

  std::stringstream ss;
  ss << std::put_time(std::localtime(&in_time_t), "%F_%T");
  std::string timestamp = ss.str();

  std::replace(timestamp.begin(), timestamp.end(), ':', '-');

  return timestamp;
}

std::string generate_unique_file_name() {
  return get_current_time_string() + "__" +
         boost::lexical_cast<std::string>(boost::uuids::random_generator()());
}
