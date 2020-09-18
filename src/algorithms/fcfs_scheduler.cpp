#include "fcfs_scheduler.h"
#include<iostream>

using namespace std;


SchedulingDecision* FcfsScheduler::get_next_thread(const Event* event) {
    // TODO: implement me

   
    SchedulingDecision* sd = new SchedulingDecision();
    
    sd->thread = readyQueue.front();
    readyQueue.pop();

    return sd;
}


void FcfsScheduler::enqueue(const Event* event, Thread* thread) {
    // TODO: implement me
    readyQueue.push(thread);

    
}


bool FcfsScheduler::should_preempt_on_arrival(const Event* event) const {
    // TODO: implement me

    //next_thread = get_next_thread(event).thread;

    return false;

}


size_t FcfsScheduler::size() const {
    // TODO: implement me


    return readyQueue.size();
}


string FcfsScheduler::print_dispatcher_invoked_message() {
    string amountThreads = "Selected from " + to_string(size() + 1) + " threads: will run to completion of burst";

    return amountThreads;
}


