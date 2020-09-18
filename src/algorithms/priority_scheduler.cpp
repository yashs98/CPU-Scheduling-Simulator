#include "algorithms/priority_scheduler.h"

using namespace std;


SchedulingDecision* PriorityScheduler::get_next_thread(const Event* event) {
    // TODO: implement me
    SchedulingDecision* sd = new SchedulingDecision();

    if(rq0.size() > 0) {
        sd->thread = rq0.front();
        rq0.pop();
        queueSelected = 0;
    }

    else if(rq1.size() > 0) {
        sd->thread = rq1.front();
        rq1.pop();
        queueSelected = 1;
    }

    else if(rq2.size() > 0) {
        sd->thread = rq2.front();
        rq2.pop();
        queueSelected = 2;
    }

    else if(rq3.size() > 0) {
        sd->thread = rq3.front();
        rq3.pop();
        queueSelected = 3;
    }
    
    return sd;
}


void PriorityScheduler::enqueue(const Event* event, Thread* thread) {
    // TODO: implement me
    if(thread->process->type == Process::SYSTEM) {
        rq0.push(thread);
    }

    else if(thread->process->type == Process::INTERACTIVE) {
        rq1.push(thread);
    }

    else if(thread->process->type == Process::NORMAL) {
        rq2.push(thread);
    }

    else if(thread->process->type == Process::BATCH) {
        rq3.push(thread);
    }




}


bool PriorityScheduler::should_preempt_on_arrival(const Event* event) const {
    // TODO: implement me
    return false;
}


size_t PriorityScheduler::size() const {
    // TODO: implement me
    return rq0.size() + rq1.size() + rq2.size() + rq3.size();
}

string PriorityScheduler::print_dispatcher_invoked_message() {
    string message = "Selected from queue " + to_string(queueSelected) + " [S:" + to_string(rq0.size()) + " I:" + to_string(rq1.size()) + " N:" + to_string(rq2.size()) + " B:" + to_string(rq3.size()) + "]";
    return message;
}