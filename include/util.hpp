#pragma once

#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

[[nodiscard]] std::vector<std::string>
split_with_delimiter(std::string_view sequence, char delimiter);

/**
 * Runs @p subprocess and returns the stdout as string.
 * @throws process opening on failure.
 **/
[[nodiscard]] std::string execute_subprocess(const std::string &subprocess);

/**
 * Runs "df -T" as subprocess.
 * @returns the stdout of "df -T"
 **/
[[nodiscard]] std::string get_disk_usage_report();

/**
 * Generates the current time as string in YYYY-MM-DD_HH-MM-ss format.
 * e.g. 2022-04-06_19-41-36
 * @returns the current time string
 **/
[[nodiscard]] std::string get_current_time_string();

/**
 * Generates a unique file name in the following format:
 * YYYY-MM-DD_HH-MM-ss__UUID
 * e.g. 2022-04-06_19-41-36__4aca3ead-a8bf-46f4-9f32-e3db87a14909
 * @returns the unique file name
 **/
[[nodiscard]] std::string generate_unique_file_name();
