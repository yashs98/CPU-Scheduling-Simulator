#pragma once
#include "algorithms/scheduler.h"
#include "types/event.h"
#include "types/scheduling_decision.h"
#include "types/thread.h"
#include "types/process.h"


/**
 * Represents a scheduling queue that demotes threads after being preempted.
 */
class MultilevelFeedbackScheduler : public Scheduler {
public:

    MultilevelFeedbackScheduler(size_t time_slice) : time_slice(time_slice) {}

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

    size_t time_slice;

};
