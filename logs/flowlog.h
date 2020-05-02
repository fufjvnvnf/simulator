#ifndef FLOWLOG_H
#define FLOWLOG_H

#include <stdint.h>
#include <stdlib.h>

#include <fstream>
#include <unordered_map>
#include <unordered_set>

namespace logs {
namespace flow {

class FlowId;

class FlowLog {
 public:
  // size and pkt_size are in byte
  FlowLog(uint32_t id, uint32_t size, uint32_t pkt_size, uint32_t src_id,
          uint32_t src_port, uint32_t dst_id, uint32_t dst_port);

  static void setLogFile(std::ofstream* log_file);

  void start(double start_time, uint32_t init_seq_no);
  void end(bool finished, uint32_t active_flows, double end_time, uint32_t cwnd,
           double slowdown);
  void send_pkt(uint32_t pkt_size, uint32_t seq_no, double time, uint32_t cwnd);
  void recv_pkt(uint32_t pkt_size, uint32_t seq_no);
  void send_ack(uint32_t pkt_size);
  void recv_ack(uint32_t pkt_size, uint32_t seq_no, double time);
  void cwnd_cut(bool is_timeout);
  void timeout();
  void ecn();
  void dup_ack();
  void rtt(double rtt);

  void write_to_file();

  std::shared_ptr<FlowId> flow_id;
  static std::ofstream log_file;

 private:
  /* flow specifics */
  double start_time;
  double end_time;
  uint32_t size_in_byte;
  uint32_t size_in_pkt;
  uint32_t pkt_size;

  /* transport layer */
  uint32_t init_seq_no;
  uint32_t last_seq_sent;
  uint32_t last_seq_recv;

  // data packets & bytes
  uint32_t bytes_sent;
  uint32_t bytes_recv;
  uint32_t pkts_sent;
  uint32_t pkts_recv;
  std::unordered_map<uint32_t, double> sent_pkts;  // seq_no -> sent time
  std::unordered_set<uint32_t> acked_pkts;

  // ack packets & bytes
  uint32_t ack_bytes_sent;
  uint32_t ack_pkts_sent;
  uint32_t ack_bytes_recv;
  uint32_t ack_pkts_recv;

  uint32_t cgstn_cwnd_cuts;
  uint32_t total_cwnd_cuts;

  uint32_t timeouts;
  uint32_t pkts_rexmit;
  uint32_t bytes_rexmit;

  uint32_t ecn_pkts;
  uint32_t dup_acks;

  /* performance */
  bool finished;
  uint32_t total_cwnd;  // avg_cwnd = total_cwnd / pkts_sent
  uint32_t max_cwnd;
  uint32_t end_cwnd;

  double total_rtt;  // avg_rtt = total_rtt / acked_pkt.size
  double max_rtt;
  double end_rtt;

  uint32_t active_flows;

  double slowdown;
};

class FlowId {
 public:
  FlowId(uint32_t id, uint32_t src, uint32_t dst, uint32_t src_port,
         uint32_t dst_port);

  uint32_t id;
  uint32_t src;
  uint32_t dst;
  uint32_t src_port;
  uint32_t dst_port;
};

}  // namespace flow
}  // namespace logs
#endif
