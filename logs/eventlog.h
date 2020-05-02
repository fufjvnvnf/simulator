#ifndef EVENTLOG_H
#define EVENTLOG_H

#include <fstream>

namespace logs {
namespace event {

class EventLog {
 public:
  EventLog(std::ofstream* event_log_file);
  ~EventLog();

  void receive(const PacketLog& packet);

 private:
  std::ofstream* event_log_file;
};

enum EventType { RECEIVE, ARRIVE, DROP, FORWARD };

class PacketLog {
 public:
  PacketLog(uint32_t fid, uint32_t src, uint32_t dst, uint32_t src_port,
            uint32_t dst_port, uint32_t seq_no, PacketType type,
            uint32_t pkt_size, uint32_t hdr_size);
  ~PacketLog();

 private:
  /* flow identification */
  uint32_t id;
  uint32_t src;
  uint32_t dst;
  uint32_t src_port;
  uint32_t dst_port;

  /* packet identification */
  uint32_t seq_no;
  PacketType type;
  uint32_t pkt_size;
  uint32_t hdr_size;
};

enum PacketType { DATA, ACK, NACK };

}  // namespace event
}  // namespace logs

#endif