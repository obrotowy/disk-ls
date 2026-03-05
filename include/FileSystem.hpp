#pragma once
#include "File.hpp"
#include <string>
#include <vector>
#include <sstream>
#include <memory>
#include <filesystem>

class FileSystem {
  public:
    virtual const std::vector<char> read_file(const std::filesystem::path& path) = 0;
    virtual const std::vector<std::unique_ptr<File>> list_directory(const std::filesystem::path& path) = 0;
    virtual std::unique_ptr<File> get_file_descriptor(const std::filesystem::path& path) = 0;
};
