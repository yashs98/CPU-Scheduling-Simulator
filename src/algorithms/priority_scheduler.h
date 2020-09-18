#pragma once
#include "algorithms/scheduler.h"
#include "types/event.h"
#include "types/scheduling_decision.h"
#include "types/thread.h"
#include "types/process.h"
using namespace std;


/**
 * Represents a scheduling queue that gives threads of each type a different
 * priority.
 */
class PriorityScheduler : public Scheduler {
public:

    virtual SchedulingDecision* get_next_thread(const Event* event) override;


    virtual void enqueue(const Event* event, Thread* thread) override;


    virtual bool should_preempt_on_arrival(const Event* event) const override;


    virtual size_t size() const override;

    virtual string print_dispatcher_invoked_message() override;

private:

    // TODO: add any instance variables you need
    queue<Thread*> rq0;
    queue<Thread*> rq1;
    queue<Thread*> rq2;
    queue<Thread*> rq3;

    size_t queueSelected;
};
