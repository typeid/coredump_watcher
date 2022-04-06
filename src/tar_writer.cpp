#include "../include/tar_writer.hpp"
#include <fcntl.h>
#include <filesystem>
#include <iostream>
#include <libtar.h>
#include <temporary_file.hpp>

tar_writer::tar_writer(const std::filesystem::path &dest_file)
    : m_dest_path(dest_file.parent_path()) {
  const int res = tar_open(&m_ptar, dest_file.c_str(), NULL, O_WRONLY | O_CREAT,
                           0644, TAR_GNU);
  if (res == -1) {
    throw std::runtime_error("tar_open failed");
  }

  std::cout << "Initialized tar archive " << dest_file << std::endl;
}

void tar_writer::append_file(const std::filesystem::path &src,
                             const std::filesystem::path &dest_path) {
  const int ec = tar_append_file(m_ptar, src.c_str(), dest_path.c_str());

  if (ec != 0) {
    std::cerr << "Could not append file " << src << " to archive." << std::endl;
  } else {
    std::cout << "Appended file " << src << " to archive." << std::endl;
  }
}

void tar_writer::append_file(const std::filesystem::path &src) {
  append_file(src, src);
}

void tar_writer::append_folder(const std::filesystem::path &src) {

  const int ec =
      tar_append_tree(m_ptar, src.string().data(), src.string().data());

  if (ec != 0) {
    std::cerr << "Could not append folder " << src.string() << " to archive."
              << std::endl;
  } else {
    std::cout << "Appended folder " << src.string() << " to archive."
              << std::endl;
  }
}

void tar_writer::append_inmemory_file(const std::filesystem::path &dest,
                                      std::string_view file_content) {
  temporary_file tmp_file;
  tmp_file.write(file_content);
  append_file(tmp_file.get_path(), dest);
}

tar_writer::~tar_writer() {

  int ec = tar_append_eof(m_ptar);
  if (ec == -1) {
    std::cerr << "Could not append eof to tar file" << std::endl;
  }

  ec = tar_close(m_ptar);
  if (ec == -1) {
    std::cerr << "Could not close tar file descriptor" << std::endl;
  }
}