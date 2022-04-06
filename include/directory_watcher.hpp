#pragma once

#include <filesystem>
#include <functional>
#include <string_view>

class directory_watcher {
public:
  directory_watcher(
      const std::filesystem::path &directory,
      std::function<void(std::string_view file_name)> on_file_creation_cb);

  directory_watcher(directory_watcher &&) = delete;

  ~directory_watcher();

  void run();

private:
  std::function<void(std::string_view file_name)> m_on_file_creation_cb;
  int m_inotify_fd;
  int m_watch_fd;
};