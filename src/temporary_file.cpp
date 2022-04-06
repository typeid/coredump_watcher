#include "../include/temporary_file.hpp"
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <unistd.h>

static constexpr std::string_view temp_file_pattern = "tempfileXXXXXX";

temporary_file::temporary_file() {
  m_path.resize(temp_file_pattern.size() + 1);
  memcpy(m_path.data(), temp_file_pattern.data(), temp_file_pattern.size());
  m_path[m_path.size()] = '\0';

  m_fd = mkstemp(m_path.data());
  if (m_fd == -1) {
    throw std::runtime_error("mkstemp failed");
  }

  m_path.resize(m_path.size() - 1);
}
temporary_file::~temporary_file() {
  std::filesystem::remove(m_path);

  int ec = close(m_fd);
  if (ec == -1) {
    std::cerr << "Could not close temporary file file descriptor" << std::endl;
  }
}

std::filesystem::path temporary_file::get_path() { return m_path; }

void temporary_file::write(std::string_view data) {
  int ec = ::write(m_fd, data.data(), data.size());
  if (ec == -1) {
    throw std::runtime_error("temporary file write failed");
  }
}
