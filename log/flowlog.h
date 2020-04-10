#include <stdint.h>
#include <stdlib.h>

#include <unordered_map>

namespace log {
namespace flow {

class FlowLog {
 public:
  // size and pkt_size are in byte
  FlowLog(uint32_t id, uint32_t size, uint32_t pkt_size, uint32_t src_id,
          uint32_t src_port, uint32_t dst_id, uint32_t dst_port);
  ~FlowLog();

  void start(double start_time, uint32_t init_seq_no);
  void finish(double end_time);  // will write to file
  void send_pkt(uint32_t pkt_size, uint32_t seq_no);
  void recv_pkt(uint32_t pkt_size, uint32_t seq_no);
  void ack(uint32_t pkt_size);

  void write_to_file();

 private:
  /* flow identification*/
  uint32_t id;
  uint32_t src;
  uint32_t dst;
  uint32_t src_port;
  uint32_t dst_port;

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

  uint32_t bytes_sent;
  uint32_t bytes_recv;
  uint32_t pkts_sent;
  uint32_t pkts_recv;

  uint32_t acked_bytes;
  uint32_t acked_pkts;
};

}  // namespace flow
}  // namespace log