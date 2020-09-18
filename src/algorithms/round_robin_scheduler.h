#pragma once
#include "algorithms/scheduler.h"
#include "types/event.h"
#include "types/scheduling_decision.h"
#include "types/thread.h"
#include "types/process.h"
#include <queue>
using namespace std;


typedef std::priority_queue<const Thread *, std::vector<const Thread*>,
                                                        EventComparator>
        ThreadQueue;
/**
 * Represents a scheduling queue that uses simple round-robin.
 */
class RoundRobinScheduler : public Scheduler {
public:

    RoundRobinScheduler(size_t time_slice) : time_slice(time_slice) {}


    virtual SchedulingDecision* get_next_thread(const Event* event) override;


    virtual void enqueue(const Event* event, Thread* thread) override;


    virtual bool should_preempt_on_arrival(const Event* event) const override;


    virtual size_t size() const override;

    virtual string print_dispatcher_invoked_message() override;


private:

    /**
     * The length of the time slice for this round-robin queue.
     */
    const size_t time_slice;

    queue<Thread*> readyQueue;
    queue<Thread*> blocked;

    size_t prev_time;
    size_t current_time;

    // TODO: add any instance variables you need
};
