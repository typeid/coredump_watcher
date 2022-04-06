#pragma once

#include <filesystem>
#include <libtar.h>
#include <string_view>

class tar_writer {
public:
  tar_writer(const std::filesystem::path &dest_file);

  tar_writer(tar_writer &&) = delete;

  ~tar_writer();

  /**
   * Appends file from @p src to the tar at the same path.
   **/
  void append_file(const std::filesystem::path &src);

  void append_file(const std::filesystem::path &src,
                   const std::filesystem::path &dest);

  /**
   * Appends folder from @p src to the tar at the same path.
   **/
  void append_folder(const std::filesystem::path &src);

  /**
   * Appends a file containing @p file_content at @p dest tar path.
   **/
  void append_inmemory_file(const std::filesystem::path &dest,
                            std::string_view file_content);

private:
  TAR *m_ptar;
  std::filesystem::path m_dest_path;
};