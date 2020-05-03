#include "eventlog.h"

namespace logs {
namespace event {

void EventLog::packet(std::string event, PacketLog* packet, double time,
                      uint32_t send_id, uint32_t node_id) {
  eventlog_file << time << ' ' << event << ' ' << send_id << "->" << node_id
                << ' ';
  eventlog_file << "flow:" << *packet->flow_id << ' ';
  eventlog_file << "packet:" << packet->seq_no << '/' << packet->pkt_id << '/'
                << packet->pkt_size << '/' << packet->hdr_size << ' ';
  eventlog_file << std::endl;
}

void EventLog::var(double time, std::string var, std::string value,
                   logs::flow::FlowId* flow_id) {
  varlog_file << time << ' ' << var << value << ' ';
  varlog_file << "flow:" << flow_id;
  varlog_file << std::endl;
}

PacketLog::PacketLog(std::shared_ptr<logs::flow::FlowId> flow_id,
                     uint32_t seq_no, uint32_t pkt_id, uint32_t pkt_size,
                     uint32_t hdr_size) {
  this->flow_id = flow_id;
  this->seq_no = seq_no;
  this->pkt_id = pkt_id;
  this->pkt_size = pkt_size;
  this->hdr_size = hdr_size;
}

void PacketLog::setType(std::string type) { this->type = type; }

}  // namespace event
}  // namespace logs