#include "log.h";

#include <fstream>
#include <iostream>

namespace log {

Log::Log(std::string log_dir) {
  extern std::ofstream flow_log_file;
  flow_log_file.open(log_dir + "/flowlog.txt");
}

}  // namespace log