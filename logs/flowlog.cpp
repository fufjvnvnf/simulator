#include "flowlog.h"

namespace logs {
namespace flow {

FlowLog::FlowLog(uint32_t id, uint32_t size, uint32_t pkt_size, uint32_t src_id,
                 uint32_t src_port, uint32_t dst_id, uint32_t dst_port,
                 std::ofstream* flow_log_file) {
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
  this->flow_log_file = flow_log_file;
}

void FlowLog::start(double start_time, uint32_t init_seq_no) {
  this->start_time = start_time;
  this->init_seq_no = init_seq_no;
}

void FlowLog::end(bool finished, uint32_t active_flows, double end_time,
                  uint32_t cwnd, double slowdown) {
  this->finished = finished;
  this->end_time = end_time;
  this->end_cwnd = cwnd;
  this->active_flows = active_flows;
  this->slowdown = slowdown;
  write_to_file();
}

void FlowLog::send_pkt(uint32_t pkt_size, uint32_t seq_no, double time,
                       uint32_t cwnd) {
  pkts_sent++;
  bytes_sent += pkt_size;
  last_seq_sent = std::max(last_seq_sent, seq_no);
  total_cwnd += cwnd;
  max_cwnd = std::max(max_cwnd, cwnd);
  if (sent_pkts.find(seq_no) != sent_pkts.end()) {
    pkts_rexmit++;
    bytes_rexmit += pkt_size;
  } else {
    sent_pkts.insert({seq_no, time});
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

void FlowLog::recv_ack(uint32_t pkt_size, uint32_t seq_no, double time) {
  this->ack_bytes_recv += pkt_size;
  this->ack_pkts_recv++;
  if (acked_pkts.find(seq_no) != acked_pkts.end()) {
    double rtt = sent_pkts.find(seq_no)->second - time;
    total_rtt += rtt;
    max_rtt = std::max(max_rtt, rtt);
    end_rtt = rtt;
    acked_pkts.insert(seq_no);
  }
}

void FlowLog::cwnd_cut(bool is_timeout) {
  this->total_cwnd_cuts++;
  if (!is_timeout) {
    cgstn_cwnd_cuts++;
  }
}

void FlowLog::timeout() { this->timeouts++; }

void FlowLog::ecn() { this->ecn_pkts++; }

void FlowLog::dup_ack() { this->dup_acks++; }

void FlowLog::write_to_file() {
  /* identification */
  *flow_log_file << id << '/' << src + ':' + src_port << '/'
                 << dst + ':' + dst_port << ' ';
  /* flow specifics */
  double fct = end_time - start_time;
  float avg_thruput = size_in_byte / fct;
  *flow_log_file << start_time << '/' << end_time << '/' << fct << '/'
                 << slowdown << '/' << size_in_byte << '/' << size_in_pkt << '/'
                 << pkt_size << '/' << avg_thruput << ' ';
  /* transport layer */
  *flow_log_file << init_seq_no << '/' << last_seq_sent << '/' << last_seq_recv
                 << " data:" << bytes_sent << '/' << bytes_recv << '/'
                 << pkts_sent << '/' << pkts_recv << " ack:" << ack_bytes_sent
                 << '/' << ack_bytes_recv << '/' << ack_pkts_sent << '/'
                 << ack_pkts_recv << ' ' << cgstn_cwnd_cuts << '/'
                 << total_cwnd_cuts << '/' << timeouts << '/' << pkts_rexmit
                 << '/' << bytes_rexmit << '/' << ecn_pkts << '/' << dup_acks
                 << ' ';
  /* performance */
  *flow_log_file << total_cwnd / (double)pkts_sent << '/' << max_cwnd << '/'
                 << end_cwnd << ' ' << total_rtt / (double)acked_pkts.size()
                 << '/' << max_rtt << '/' << end_rtt << ' ';
  *flow_log_file << active_flows << ' ' << finished << std::endl;
}

}  // namespace flow
}  // namespace logs