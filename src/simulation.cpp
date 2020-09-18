#include "simulation.h"
#include "types/event.h"
#include <cassert>
#include <fstream>
#include <iostream>
#include<sstream>

using namespace std;


void Simulation::run(const string& filename) {
    read_file(filename);

    // While their are still events to process, invoke the corresponding methods
    // to handle them.
    while (!events.empty()) {
        const Event* event = events.top();
        events.pop();
      
        // Invoke the appropriate method on the scheduler for the given event type.
        switch (event->type) {
             
        
        case Event::THREAD_ARRIVED:
            
            handle_thread_arrived(event);
            logger.print_state_transition(event,event->thread->previous_state, event->thread->current_state);

            break;

        case Event::THREAD_DISPATCH_COMPLETED:
            handle_thread_dispatch_completed(event);
        
            logger.print_state_transition(event,event->thread->previous_state, event->thread->current_state);
            break;

        case Event::PROCESS_DISPATCH_COMPLETED:
            handle_process_dispatch_completed(event);
            logger.print_state_transition(event,event->thread->previous_state, event->thread->current_state);
            
            break;

        case Event::CPU_BURST_COMPLETED:
            handle_cpu_burst_completed(event);
            
                    
            break;

        case Event::IO_BURST_COMPLETED:
            handle_io_burst_completed(event);
            
            logger.print_state_transition(event,event->thread->previous_state, event->thread->current_state);
            
            break;

        case Event::THREAD_COMPLETED:

           handle_thread_completed(event);
           logger.print_state_transition(event,Thread::RUNNING, Thread::EXIT);
           
            break;

        case Event::THREAD_PREEMPTED:
            handle_thread_preempted(event);
            logger.print_verbose(event, event->thread, "");
            break;

        case Event::DISPATCHER_INVOKED:
            handle_dispatcher_invoked(event);
            //string amountThreads = "Selected from " + to_string(scheduler->size() + 1) + " threads: will run to completion of burst";
            logger.print_verbose(event, active_thread, scheduler->print_dispatcher_invoked_message());
            break;
        }


        if(active_thread == nullptr && !scheduler->empty() && !invoked) {
                events.push(new Event(Event::DISPATCHER_INVOKED, event->time, event->thread));
                invoked = true;
        }
  

        // Free the event's memory.  
        delete event;
    }
   

    stats.thread_counts[0] = systemThreads;
    stats.thread_counts[1] = interactiveThreads;
    stats.thread_counts[2] = normalThreads;
    stats.thread_counts[3] = batchThreads;
    
   
    stats.total_idle_time = stats.total_time - (stats.service_time + stats.dispatch_time);
    stats.avg_thread_response_times[0] = systemResponseTime/systemThreads;
    stats.avg_thread_response_times[1] = interactiveResponseTime/interactiveThreads;
    stats.avg_thread_response_times[2] = normalResponseTime/normalThreads;
    stats.avg_thread_response_times[3] = batchResponseTime/batchThreads;

    stats.avg_thread_turnaround_times[0] = systemTAT/systemThreads;
    stats.avg_thread_turnaround_times[1] = interactiveTAT/interactiveThreads;
    stats.avg_thread_turnaround_times[2] = normalTAT/normalThreads;
    stats.avg_thread_turnaround_times[3] = batchTAT/batchThreads;

    stats.total_cpu_time = stats.service_time + stats.dispatch_time;

   
    stats.cpu_utilization = (double)(stats.total_cpu_time)/(stats.total_time) * 100.0;
    stats.cpu_efficiency = (double) (stats.service_time)/(stats.total_time) * 100.0;



    for(auto x: processes) {
        logger.print_process_details(x.second);
    }
    
    logger.print_statistics(stats);
    
    
}


//==============================================================================
// Event-handling methods
//==============================================================================


void Simulation::handle_thread_arrived(const Event* event) {
    // TODO: handle this event properly (feel free to modify code structure, tho)
   
    scheduler->enqueue(event, event->thread);
    
    event->thread->set_thread_ready();
   
    event->thread->state_change_time = event->time;

     if(active_thread == nullptr && !invoked) {
         
         events.push(new Event(Event::DISPATCHER_INVOKED, event->time, event->thread));
         invoked = true;
        
     }

     event->thread->arrival_time = event->time;
     
     event->thread->totalNumBursts = event->thread->bursts.size();
     
    if(event->thread->process->type == Process::BATCH) {
        batchThreads++;

    }

    else if(event->thread->process->type == Process::INTERACTIVE) {
        interactiveThreads++;
    }

    else if(event->thread->process->type == Process::NORMAL) {
        normalThreads++;
           
    }

    else if(event->thread->process->type == Process::SYSTEM) {
        systemThreads++;

          
    }

    
}


void Simulation::handle_thread_dispatch_completed(const Event* event) {
    // TODO: handle this event properly (feel free to modify code structure, tho)
    
   
    if(scheduler->should_preempt_on_arrival(event)) {

        //active_thread->previous_state = Thread::RUNNING;
        //active_thread->current_state = Thread::READY;
        active_thread->set_thread_ready();
        events.push(new Event(Event::THREAD_PREEMPTED, event->time + event->scheduling_decision->time_slice, active_thread));
        event->thread->state_change_time = event->time;
    }
    
    else {

        events.push(new Event(Event::CPU_BURST_COMPLETED, event->time + active_thread->bursts.front()->length, active_thread));
        stats.service_time += active_thread->bursts.front()->length;
       
        
        //active_thread->previous_state = active_thread->current_state;
        //active_thread->current_state = Thread::RUNNING;
        active_thread->set_thread_run();
        event->thread->state_change_time = event->time;
       

    }

    if(event->thread->bursts.size() == event->thread->totalNumBursts && event->thread->first_time_execute == 0) {
            event->thread->start_time = (int) event->time;
            event->thread->first_time_execute = 1;
            
    }

   
     
    

}


void Simulation::handle_process_dispatch_completed(const Event* event) {
    // TODO: handle this event properly (feel free to modify code structure, tho)

    
    if(scheduler->should_preempt_on_arrival(event)) {
        event->thread->state_change_time = event->time + event->scheduling_decision->time_slice;
        //active_thread->previous_state = Thread::READY;
        //active_thread->current_state = Thread::RUNNING;
        active_thread->set_thread_run();
        event->thread->state_change_time = event->time;
        events.push(new Event(Event::THREAD_PREEMPTED, event->time + event->scheduling_decision->time_slice, active_thread));
       
    }

   else {
                
        event->thread->state_change_time = event->time + active_thread->bursts.front()->length;
        event->thread->state_change_time = event->time;
        
        events.push(new Event(Event::CPU_BURST_COMPLETED, event->time + active_thread->bursts.front()->length, active_thread));
        
        stats.service_time += active_thread->bursts.front()->length;
     
        
        //active_thread->previous_state = active_thread->current_state;
        //active_thread->current_state = Thread::RUNNING;
        active_thread->set_thread_run();

    }

     if(event->thread->bursts.size() == event->thread->totalNumBursts && event->thread->first_time_execute == 0) {
            event->thread->start_time = (int) event->time;
            event->thread->first_time_execute = 1;
            
    }

    
   
}


void Simulation::handle_cpu_burst_completed(const Event* event) {
    // TODO: handle this event properly (feel free to modify code structure, tho)
  
    stats.total_time += active_thread->bursts.front()->length;
    stats.total_cpu_time+= active_thread->bursts.front()->length;
    active_thread->service_time += active_thread->bursts.front()->length;
    
    active_thread->bursts.pop();  
   
  

    //active_thread->previous_state = active_thread->current_state;
    //active_thread->current_state = Thread::BLOCKED;
    active_thread->set_thread_blocked();
    event->thread->state_change_time = event->time;

   
    

    if(active_thread->bursts.size() == 0) {
     
        events.push(new Event(Event::THREAD_COMPLETED, event->time, active_thread));
        
    }
    
    else {
        events.push(new Event(Event::IO_BURST_COMPLETED, event->time + event->thread->bursts.front()->length, event->thread));
        event->thread->io_time += event->thread->bursts.front()->length; 
        logger.print_state_transition(event,event->thread->previous_state, event->thread->current_state);
    }

    prev_thread = active_thread;
   
    active_thread = nullptr;
   
    
}


void Simulation::handle_io_burst_completed(const Event* event) {
    // TODO: handle this event properly (feel free to modify code structure, tho)

     
    stats.io_time += event->thread->bursts.front()->length;
    
    //event->thread->previous_state = Thread::BLOCKED;
    //event->thread->current_state = Thread::READY;
    event->thread->set_thread_ready();
    event->thread->state_change_time = event->time;
    event->thread->bursts.pop();
    
   
    
    if(event->thread->bursts.size() == 0) {
        
        events.push(new Event(Event::THREAD_COMPLETED, event->time, event->thread));
      
    }

    else {
        scheduler->enqueue(event, event->thread);
         
    }
   
   
   
    
}


void Simulation::handle_thread_completed(const Event* event) {
    // TODO: handle this event properly (feel free to modify code structure, tho)
    

    event->thread->end_time = event->time;
    //stats.service_time += event->time - (int) event->thread->state_change_time; 
   
    if(!scheduler->empty()) {
       
        //event->thread->previous_state = event->thread->current_state;
        //event->thread->current_state = Thread::EXIT;
        event->thread->set_thread_exit();
        event->thread->state_change_time = event->time;
    }

    if(events.size() == 1) {
        stats.total_time = event->time;
    }

    if(event->thread->process->type == Process::BATCH) {
        batchTAT += (double) event->thread->turnaround_time();
        batchResponseTime += event->thread->response_time();
    }

    else if(event->thread->process->type == Process::INTERACTIVE) {
        interactiveTAT += (double) event->thread->turnaround_time();
        interactiveResponseTime += event->thread->response_time();
    }

    else if(event->thread->process->type == Process::NORMAL) {
         normalTAT += (double) event->thread->turnaround_time();
         normalResponseTime += event->thread->response_time();
           
    }

    else if(event->thread->process->type == Process::SYSTEM) {
        systemTAT += (double) event->thread->turnaround_time();
        systemResponseTime += event->thread->response_time();
          
    }

    if(scheduler->empty()) {
        stats.total_time = event->time;
        
    }

    
    
    
}


void Simulation::handle_thread_preempted(const Event* event) {
    // TODO: handle this event properly (feel free to modify code structure, tho)
    active_thread->bursts.front()->length -=  event->time - (int) active_thread->state_change_time; 
    stats.service_time += event->time - (int) active_thread->state_change_time; 

    event->thread->service_time += event->time - (int) active_thread->state_change_time; 
    
    scheduler->enqueue(event, event->thread);
    prev_thread = active_thread;
    active_thread = nullptr;
   
    
}


void Simulation::handle_dispatcher_invoked(const Event* event) {
    // TODO: handle this event properly (feel free to modify code structure, tho)
    SchedulingDecision* next_thread_sd = scheduler->get_next_thread(event);


    invoked = false;
     


    if(active_thread == nullptr) {

        if(prev_thread != nullptr && prev_thread->process == next_thread_sd->thread->process) {
                active_thread = next_thread_sd->thread;

                events.push(new Event(Event::THREAD_DISPATCH_COMPLETED, event->time + thread_switch_overhead, active_thread, next_thread_sd));
                stats.total_time += thread_switch_overhead;
                stats.dispatch_time += thread_switch_overhead;
            
        }


        else {
                active_thread = next_thread_sd->thread;

                events.push(new Event(Event::PROCESS_DISPATCH_COMPLETED, event->time+ process_switch_overhead, active_thread, next_thread_sd));
                
                stats.total_time += process_switch_overhead;
                stats.dispatch_time += process_switch_overhead;
                
        }
        
    }

   
    
}




//==============================================================================
// Utility methods
//=============================%MCEPASTEBIN%=================================================


void Simulation::add_event(Event* event) {
    if (event != nullptr) {
        events.push(event);
    }
}


void Simulation::read_file(const string& filename) {
    ifstream file(filename.c_str());

    if (!file || file.peek() == std::ifstream::traits_type::eof()) {
        cerr << "Unable to open simulation file: " << filename << endl;
        exit(EXIT_FAILURE);
    }

    
    size_t num_processes;

    // Read the total number of processes, as well as the dispatch overheads.
    file >> num_processes >> thread_switch_overhead >> process_switch_overhead;

    // Read in each process.
    for (size_t p = 0; p < num_processes; p++) {
        Process* process = read_process(file);

        processes[process->pid] = process;
    }
}


Process* Simulation::read_process(istream& in) {
    int pid, type;
    size_t num_threads;

    // Read in the process ID, its type, and the number of threads.
    in >> pid >> type >> num_threads;

    // Create the process and register its existence in the processes map.
    Process* process = new Process(pid, (Process::Type) type);

    // Read in each thread in the process.
    for (size_t tid = 0; tid < num_threads; tid++) {
        process->threads.push_back(read_thread(in, tid, process));
    }

    return process;
}


Thread* Simulation::read_thread(istream& in, int tid, Process* process) {
    int arrival_time;
    size_t num_cpu_bursts;

    // Read in the thread's arrival time and its number of CPU bursts.
    in >> arrival_time >> num_cpu_bursts;

    Thread* thread = new Thread(arrival_time, tid, process);

   
    // Read in each burst in the thread.
    for (size_t n = 0, burst_length; n < num_cpu_bursts * 2 - 1; n++) {
        in >> burst_length;
        Burst::Type burst_type = (n % 2 == 0)
                ? Burst::CPU
                : Burst::IO;

        thread->bursts.push(new Burst(burst_type, burst_length));
    }


    // Add an arrival event for the thread.
    events.push(new Event(Event::THREAD_ARRIVED, thread->arrival_time, thread));

    return thread;
}


SystemStats Simulation::calculate_statistics() {
    // TODO: your code here (optional; feel free to modify code structure)

    return SystemStats();
}
