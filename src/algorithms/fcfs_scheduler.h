#pragma once
#include "algorithms/scheduler.h"
#include "types/event.h"
#include "types/scheduling_decision.h"
#include "types/thread.h"
#include <queue>
using namespace std;



typedef std::priority_queue<const Thread *, std::vector<const Thread*>,
                                                        EventComparator>
        ThreadQueue;


/**
 * Represents a scheduling queue that uses first-come, first-served.
 */
class FcfsScheduler : public Scheduler {
public:

    virtual SchedulingDecision* get_next_thread(const Event* event) override;


    virtual void enqueue(const Event* event, Thread* thread) override;


    virtual bool should_preempt_on_arrival(const Event* event) const override;


    virtual size_t size() const override;

    virtual string print_dispatcher_invoked_message() override;

     
private:

    // TODO: add any instance variables you need

  

  
    queue <Thread*> readyQueue;
    //SchedulingDecision* sd;
    queue <Thread*> blocked;


};
