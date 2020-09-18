#include "round_robin_scheduler.h"
#include <iostream>
using namespace std;


SchedulingDecision* RoundRobinScheduler::get_next_thread(const Event* event) {
    // TODO: implement me

    SchedulingDecision* sd = new SchedulingDecision();
    sd->time_slice = time_slice;

   

   sd->thread = readyQueue.front();
   readyQueue.pop();

    return sd;
}


void RoundRobinScheduler::enqueue(const Event* event, Thread* thread) {
    // TODO: implement me
    
    readyQueue.push(thread);

}


bool RoundRobinScheduler::should_preempt_on_arrival(const Event* event) const {
    // TODO: implement me
   
  
   if(event->thread->bursts.front()->length - (int) time_slice > 0) {
        return true;
    }

    else {
        return false;
    }
    
}


size_t RoundRobinScheduler::size() const {      
    // TODO: implement me
    
    return readyQueue.size();
}

string RoundRobinScheduler::print_dispatcher_invoked_message() {

    string amountThreads = "Selected from " + to_string(size() + 1) + " threads: will run for at most " + to_string(time_slice) + " ticks";

    return amountThreads;
}



