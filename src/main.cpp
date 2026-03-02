#include "Disk.hpp"
#include "Partition.hpp"
#include "ext2/Ext2.hpp"
#include <iostream>
#include <cstring>
#include <sstream>

void list_partitions(const std::vector<Partition>& partitions) {
  std::cout << "\tStart\tEnd\tSectors\tSize\tType" << std::endl;
  for (int i = 0; i<partitions.size(); ++i) {
    std::cout << i << '\t' << partitions[i] << std::endl;
  }
}


void interface_loop(std::vector<Partition>& partitions, int partition_n) {
  Ext2 fs(partitions[partition_n]);
  uint32_t current_inode_n = 2;   // start with root directory
  std::string current_path = "/";

  auto process_cmd = [&](std::vector<std::string>& tokens) {
    if (tokens.empty()) return;

    const std::string& cmd = tokens[0];

    if (cmd == "ls") {
      for (const auto& e: fs.list_directory(current_path)) {
        auto* f = dynamic_cast<Ext2File*>(e.get());
        std::cout << f->name << std::endl;
      }
    }

    
    else if (cmd == "cat") {
      const std::vector<char> file = fs.read_file(tokens[1]);
      std::cout << file.data() << std::endl;
    }
  };

  std::string line;
  while (std::cout << current_path << "$ " && std::getline(std::cin, line)) {
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
