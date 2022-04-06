#include "../include/directory_watcher.hpp"
#include "../include/filename_validation.hpp"
#include "tar_writer.hpp"
#include "util.hpp"

#include <filesystem>
#include <gflags/gflags.h>
#include <iostream>
#include <string>
#include <string_view>

DEFINE_string(watch_directory, ".",
              "directory to be watched for trigger files");

DEFINE_string(archive_files, "testfile1,testfile2",
              "comma-separated list of files to be archived on trigger");

DEFINE_string(archive_folders, "logfolder",
              "comma-separated list of folders to be archived on trigger");

DEFINE_string(archive_output_directory, "/tmp/",
              "output directory for the archives");

DEFINE_bool(archive_coredump, true, "include coredump in tar archive");

static const std::filesystem::path disk_usage_report_path =
    "/tmp/disk_usage_report";

int main(int argc, char *argv[]) {
  gflags::SetUsageMessage(
      "A simple directory watcher that triggers an archive process when a file "
      "in a coredump format is created.");
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  const std::vector<std::string> files_to_tar =
      split_with_delimiter(FLAGS_archive_files, ',');
  const std::vector<std::string> folders_to_tar =
      split_with_delimiter(FLAGS_archive_folders, ',');

  directory_watcher watcher(
      FLAGS_watch_directory, [&](std::string_view file_name) {
        if (!is_trigger_string(file_name)) {
          return;
        }

        std::filesystem::path tar_output_path = FLAGS_archive_output_directory;
        tar_output_path = tar_output_path / generate_unique_file_name();
        tar_output_path.replace_extension("tar");

        tar_writer tar_writer(tar_output_path);

        if (FLAGS_archive_coredump) {
          tar_writer.append_file(std::filesystem::path(FLAGS_watch_directory) /
                                 file_name);
        }

        tar_writer.append_inmemory_file("disk_usage_report",
                                        get_disk_usage_report());

        for (const auto &file : files_to_tar) {
          tar_writer.append_file(file);
        }

        for (const auto &folder : folders_to_tar) {
          tar_writer.append_folder(folder);
        }
      });

  std::cout << "Coredump Watcher started." << std::endl;
  watcher.run();

  // TODO(Claudio): properly handle signals for clean shutdown
  return EXIT_FAILURE;
}