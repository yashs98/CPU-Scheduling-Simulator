#include "algorithms/multilevel_feedback_scheduler.h"

using namespace std;


SchedulingDecision* MultilevelFeedbackScheduler::get_next_thread(const Event* event) {
    // TODO: implement me
    SchedulingDecision* sd = new SchedulingDecision();
    
    if(rq0.size() > 0) {
        sd->thread = rq0.front();
        rq0.pop();
        queueSelected = 0;
        time_slice = 1;
    }

    else if(rq1.size() > 0) {
        sd->thread = rq1.front();
        rq1.pop();
        queueSelected = 1;
        time_slice = 2;
    }

    else if(rq2.size() > 0) {
        sd->thread = rq2.front();
        rq2.pop();
        queueSelected = 2;
        time_slice = 4;
    }

    else if(rq3.size() > 0) {
        sd->thread = rq3.front();
        rq3.pop();
        queueSelected = 3;
        time_slice = 8;
    }

    sd->time_slice = time_slice;

    return sd;
}


void MultilevelFeedbackScheduler::enqueue(const Event* event, Thread* thread) {
    // TODO: implement me

   

    if(event->type == Event::THREAD_PREEMPTED) {
        if(thread->current_priority == 0) {
            thread->current_priority = 1; 
            rq1.push(thread);
        }

        else if(thread->current_priority == 1) {
            thread->current_priority = 2; 
            rq2.push(thread);
        }

        else if(thread->current_priority == 2) {
            thread->current_priority = 3; 
            rq3.push(thread);
        }

        else if(thread->current_priority == 3) {
            rq3.push(thread);
        }
    }

    else if(event->type == Event::THREAD_ARRIVED) {
        
        thread->current_priority = 0;  
            
        rq0.push(thread);
    }

    else if(event->type == Event::IO_BURST_COMPLETED) {
        

        if(thread->current_priority == 0) {
            //thread->current_priority = 0;
            rq0.push(thread);
        }

        else if(thread->current_priority == 1) {
            rq1.push(thread);
            //thread->current_priority = 1;
        }

        else if(thread->current_priority == 2) {
            rq2.push(thread);
            //thread->current_priority = 2;
        }

        else if(thread->current_priority == 3) {
            rq3.push(thread);
            //thread->current_priority = 3;
        }
    

    }
    
    


}


bool MultilevelFeedbackScheduler::should_preempt_on_arrival(const Event* event) const {
    // TODO: implement me
    if(event->thread->bursts.front()->length - (int) time_slice > 0) {
        return true;
    }

    else {
        return false;
    }
}


size_t MultilevelFeedbackScheduler::size() const {
    // TODO: implement me
    return rq0.size() + rq1.size() + rq2.size() + rq3.size();
}

string MultilevelFeedbackScheduler::print_dispatcher_invoked_message() {
     string message = "Selected to run from queue " + to_string(queueSelected); 
     return message;
 };