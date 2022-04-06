#pragma once

#include <filesystem>
#include <string>
#include <string_view>

class temporary_file {
public:
  /**
   * Creates a tmp file.
   * Deletion of the file occurs on object destruction.
   **/
  temporary_file();

  temporary_file(temporary_file &&) = delete;

  ~temporary_file();

  /**
   * Does not require a flush, as the data is instantly available as per POSIX
   * standard: https://stackoverflow.com/a/64093650
   **/
  void write(std::string_view data);

  [[nodiscard]] std::filesystem::path get_path();

private:
  std::string m_path;
  int m_fd;
};