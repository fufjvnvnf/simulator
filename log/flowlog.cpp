#include "flowlog.h";

#include <fstream>

extern std::ofstream flow_log_file;

namespace log {
namespace flow {

FlowLog::FlowLog(uint32_t id, uint32_t size, uint32_t pkt_size, uint32_t src_id,
                 uint32_t src_port, uint32_t dst_id, uint32_t dst_port) {
  this->id = id;
  this->pkt_size = pkt_size;
  this->src = src_id;
  this->src_port = src_port;
  this->dst = dst_id;
  this->dst_port = dst_port;

  this->size_in_byte = size;
  this->size_in_pkt = size / pkt_size;

  this->bytes_sent = 0;
  this->bytes_recv = 0;
  this->pkts_sent = 0;
  this->pkts_recv = 0;

  this->acked_bytes = 0;
  this->acked_pkts = 0;
}

void FlowLog::start(double start_time, uint32_t init_seq_no) {
  this->start_time = start_time;
  this->init_seq_no = init_seq_no;
  this->seq_no = init_seq_no;
}

void FlowLog::finish(double end_time) {
  this->end_time = end_time;
  write_to_file();
}

void FlowLog::send_pkt(uint32_t pkt_size) {
  pkts_sent++;
  bytes_sent += pkt_size;
}

void FlowLog::recv_pkt(uint32_t pkt_size) {
  pkts_recv++;
  bytes_recv += pkt_size;
}

void FlowLog::write_to_file() {
  // TODO
  flow_log_file << "test\n";
}

}  // namespace flow
}  // namespace log