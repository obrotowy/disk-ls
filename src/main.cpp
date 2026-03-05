#include "Disk.hpp"
#include "Partition.hpp"
#include "ext2/Ext2.hpp"
#include <iostream>
#include <cstring>
#include <sstream>
#include <filesystem>

void list_partitions(const std::vector<Partition>& partitions) {
  std::cout << "\tStart\tEnd\tSectors\tSize\tType" << std::endl;
  for (int i = 0; i<partitions.size(); ++i) {
    std::cout << i << '\t' << partitions[i] << std::endl;
  }
}


void interface_loop(std::vector<Partition>& partitions, int partition_n) {
  Ext2 fs(partitions[partition_n]);
  std::filesystem::path current_path("/");

  auto process_cmd = [&](std::vector<std::string>& tokens) {
    if (tokens.empty()) return;

    const std::string& cmd = tokens[0];

    if (cmd == "ls") {
      for (const auto& e: fs.list_directory(current_path)) {
        auto* f = dynamic_cast<Ext2File*>(e.get());
        std::cout << f->name << std::endl;
      }
    }

    else if (cmd == "cd") {
      std::unique_ptr<File> f = std::move(fs.get_file_descriptor(current_path / tokens[1]));
      if (f->type != DIRECTORY)
        throw std::runtime_error("Not a directory.");
      current_path /= tokens[1];
      current_path = current_path.lexically_normal();
    }
    
    else if (cmd == "cat") {
      std::filesystem::path p = tokens[1];
      if (p.is_relative()) {
        p = current_path / p;
      }
      const std::vector<char> file = fs.read_file(p);
      std::cout << file.data() << std::endl;
    }
  };

  std::string line;
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  while (std::cout << current_path.string() << "$ " && std::getline(std::cin, line)) {
    std::stringstream ss(line);
    std::string token;
    std::vector<std::string> tokens;

    while (ss >> token) {
      if (token == "exit")
        return;
      tokens.push_back(token);
    }
    process_cmd(tokens);
  }
}

int main(int argc, const char** argv) {
  if (argc < 2) {
    std::cout << "Usage: ./disk-ls <disk_image_file>" << std::endl;
    exit(-1);
  }
  Disk d = Disk(argv[1]);
  std::vector<Partition> partitions = enumerate_partitions(d);
  list_partitions(partitions);
  
  unsigned int current_partition;
  std::string current_path = "/";
  std::cout << "Select partition: ";
  std::cin >> current_partition;

  if (current_partition >= partitions.size())
    throw std::out_of_range("Partition number out of range.");
  
  interface_loop(partitions, current_partition);
}
