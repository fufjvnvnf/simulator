#include <stdint.h>

namespace stats {
namespace flow {

class Flow {
 public:
  Flow(uint32_t id, double start_time, uint32_t size, uint32_t src_id,
       uint32_t src_port, uint32_t dst_id, uint32_t dst_port);
  ~Flow();

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
  int size_in_byte;
  int size_in_pkt;

  /* transport layer */
  uint32_t init_seq_no;
  uint32_t last_seq_no;
  uint32_t bytes_sent;
  uint32_t bytes_recv;
  uint32_t pkts_sent;
  uint32_t pkts_recv;
  uint32_t acks_sent;
  uint32_t acks_recv;
};

}  // namespace flow
}  // namespace stats