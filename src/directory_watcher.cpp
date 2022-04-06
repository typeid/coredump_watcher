#include "../include/directory_watcher.hpp"

#include <array>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <sys/inotify.h>
#include <sys/types.h>
#include <unistd.h>

namespace {
constexpr std::size_t event_buffer_size = 1024;
}
directory_watcher::directory_watcher(
    const std::filesystem::path &directory,
    std::function<void(std::string_view file_name)> on_file_creation_cb)
    : m_on_file_creation_cb(on_file_creation_cb) {
  m_inotify_fd = inotify_init();

  if (m_inotify_fd == -1) {
    throw std::runtime_error("inotify_init call failed");
  }
  m_watch_fd = inotify_add_watch(m_inotify_fd, directory.c_str(), IN_CREATE);
}

void directory_watcher::run() {
  std::array<inotify_event, event_buffer_size> event_buffer;

  while (const ssize_t read_size =
             read(m_inotify_fd, event_buffer.begin(), event_buffer.size())) {

    if (read_size == -1) {
      throw std::runtime_error(
          "Error while reading from inotify file descriptor.");
    }

    for (std::size_t i = 0; i < read_size / sizeof(inotify_event); ++i) {
      const inotify_event &event = event_buffer.at(i);
      if (event.len) {
        if (event.mask & IN_CREATE) {
          if (event.mask & IN_ISDIR) {
            // New type is directory
          } else {
            m_on_file_creation_cb(event.name);
          }
        }
      }
    }
  }
}

directory_watcher::~directory_watcher() {
  int ec = inotify_rm_watch(m_inotify_fd, m_watch_fd);
  if (ec == -1) {
    std::cerr << "inotify_rm_watch failed" << std::endl;
  }

  ec = close(m_inotify_fd);
  if (ec == -1) {
    std::cerr << "Could not close inotify file descriptor" << std::endl;
  }
}