#ifndef EVENTLOG_H
#define EVENTLOG_H

#include <fstream>

#include "flowlog.h"

namespace logs {
namespace event {

class PacketLog;

class EventLog {
 public:
  static void event(std::string event, PacketLog* packet, double time,
                    uint32_t send_id, uint32_t node_id);

  static std::ofstream log_file;
};

class PacketLog {
 public:
  PacketLog(std::shared_ptr<logs::flow::FlowId> flow_id, uint32_t seq_no,
            uint32_t pkt_id, uint32_t pkt_size, uint32_t hdr_size);
  void setType(std::string type);

  std::shared_ptr<logs::flow::FlowId> flow_id;
  /* packet identification */
  uint32_t seq_no;
  uint32_t pkt_id;
  std::string type;
  uint32_t pkt_size;
  uint32_t hdr_size;
};

}  // namespace event
}  // namespace logs

#endif