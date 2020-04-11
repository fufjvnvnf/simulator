#include <string>

extern std::ofstream flow_log_file;

namespace log {

void init(std::string path) { flow_log_file.open(path + "/flowlog.txt"); }

}  // namespace log