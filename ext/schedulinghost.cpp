#include <assert.h>

#include "../coresim/flow.h"
#include "../coresim/packet.h"
#include "../coresim/params.h"
#include "../coresim/event.h"

#include "factory.h"
#include "schedulinghost.h"
#include "otherevents.h"

extern double get_current_time();
extern void add_to_event_queue(Event*);
extern DCExpParams params;

bool HostFlowComparator::operator() (Flow* a, Flow* b) {
    // use FIFO ordering since all flows are same size
    return a->start_time > b->start_time;
}

SchedulingHost::SchedulingHost(uint32_t id, double rate, uint32_t queue_type) : Host(id, rate, queue_type, SCHEDULING_HOST) {
    this->host_proc_event = NULL;
}

void SchedulingHost::start(Flow* f) {
    this->sending_flows.push(f);
    if (this->host_proc_event == NULL || this->host_proc_event->time < get_current_time()) {
        this->send();
    }
    else if (this->host_proc_event->host != this) {
        assert(false);
    }
}

void SchedulingHost::send() {
    if (this->sending_flows.empty()) {
        return;
    }

    if (!this->queue->busy) {
        while (!this->sending_flows.empty() && (this->sending_flows.top())->finished) {
            this->sending_flows.pop();    
        }
        if (this->sending_flows.empty()) {
            return;
        }
        (this->sending_flows.top())->send_pending_data();
    }
    else {
        QueueProcessingEvent *qpe = this->queue->queue_proc_event;
        uint32_t queue_size = this->queue->bytes_in_queue;
        double td = this->queue->get_transmission_delay(queue_size);
        this->host_proc_event = new HostProcessingEvent(qpe->time + td, this);
        add_to_event_queue(this->host_proc_event);
    }
}
