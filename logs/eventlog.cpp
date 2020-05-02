#include "eventlog.h"

namespace logs {
namespace event {

EventLog::EventLog(std::ofstream* log_file) { this->log_file = log_file; }

void EventLog::receive(PacketLog* packet, double time, uint32_t send_id,
                       uint32_t node_id) {
  *log_file << "receive " << time << ' ' << send_id << "->" << node_id << ' ';
  *log_file << "flow:" << packet->flow_id->id << '/'
            << packet->flow_id->src + ':' + packet->flow_id->src_port << '/'
            << packet->flow_id->dst + ':' + packet->flow_id->dst_port << ' ';
  *log_file << "packet:" << packet->seq_no << '/' << packet->pkt_id << '/'
            << packet->pkt_size << '/' << packet->hdr_size << ' ';
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