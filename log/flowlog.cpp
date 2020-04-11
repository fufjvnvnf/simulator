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

  size_in_byte = size;
  size_in_pkt = size / pkt_size;

  bytes_sent = 0;
  bytes_recv = 0;
  pkts_sent = 0;
  pkts_recv = 0;

  last_seq_sent = 0;
  last_seq_recv = 0;

  ack_bytes_sent = 0;
  ack_pkts_sent = 0;
  ack_bytes_recv = 0;
  ack_pkts_recv = 0;
}

void FlowLog::start(double start_time, uint32_t init_seq_no) {
  this->start_time = start_time;
  this->init_seq_no = init_seq_no;
}

void FlowLog::finish(double end_time) {
  this->end_time = end_time;
  write_to_file();
}

void FlowLog::send_pkt(uint32_t pkt_size, uint32_t seq_no) {
  pkts_sent++;
  bytes_sent += pkt_size;
  last_seq_sent = std::max(this->last_seq_sent, seq_no);
  if (sent_pkts.count(seq_no)) {
    pkts_rexmit++;
    bytes_rexmit += pkt_size;
  } else {
    sent_pkts.insert(seq_no);
  }
}

void FlowLog::recv_pkt(uint32_t pkt_size, uint32_t seq_no) {
  pkts_recv++;
  bytes_recv += pkt_size;
  this->last_seq_recv = std::max(this->last_seq_recv, seq_no);
}

void FlowLog::send_ack(uint32_t pkt_size) {
  this->ack_bytes_sent += pkt_size;
  this->ack_pkts_sent++;
}

void FlowLog::recv_ack(uint32_t pkt_size) {
  this->ack_bytes_recv += pkt_size;
  this->ack_pkts_recv++;
}

void FlowLog::cwnd_cut(bool is_timeout) {
  this->total_cwnd_cuts++;
  if (!is_timeout) {
    cgstn_cwnd_cuts++;
  }
}

void FlowLog::timeout() { this->timeouts++; }

void FlowLog::ecn() { this->ecn_pkts++; }

void FlowLog::write_to_file() {
  // TODO
  flow_log_file << "test\n";
}

}  // namespace flow
}  // namespace log