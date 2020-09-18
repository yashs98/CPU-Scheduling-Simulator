#pragma once
#include "burst.h"
#include <cassert>
#include <queue>


// Forward declaration (circular dependency resolution).
struct Process;


/**
 * Represents a thread of execution, with one or more CPU + I/O bursts.
 */
struct Thread {

    /**
     * Represents valid states in which threads / processes can exist.
     */
    enum State {
        NEW,
        READY,
        RUNNING,
        BLOCKED,
        EXIT
    };

    /**
     * The ID of this thread
     */
    int id;

    /**
     * The time at which this thread arrived
     */
    size_t arrival_time = -1;

    /**
     * The time at which this thread was first executed.
     */
    size_t start_time = -1;

    /**
     * The time at which this thread finished executing.
     */
    size_t end_time = -1;

    /**
     * The total amount of time spent executing on the CPU for this thread.
     */
    size_t service_time = 0;

    /**
     * The total amount of time spent doing I/O for this thread.
     */
    size_t io_time = 0;

    /**
     * The absolute time at which the last state change occurred.
     */
    size_t state_change_time = -1;

    /**
     * The current state of the thread.
     */
    State current_state = NEW;

    /**
     * The previous state of the thread.
     */
    State previous_state;

    /**
     * All bursts that are a part of this thread.
     */
    std::queue<Burst*> bursts;

    /**
     * The process associated with this thread.
     */
    Process* process;

    int first_time_execute = 0;

    /**
     * Constructor.
     */
    Thread(size_t arrival, int id, Process* process) :
            id(id),
            arrival_time(arrival),
            process(process) {}

    /**
     * Returns the response time.
    */
    size_t response_time() const {
        //assert(current_state == EXIT);
        double responseTime = start_time - arrival_time;
       
        return responseTime;
    }
    
    
    size_t turnaround_time() const {
       
        double tat = end_time - arrival_time;
        return tat;
    }


    size_t totalNumBursts;


    void set_thread_run() {

        previous_state = current_state;
        current_state = RUNNING;
    }

     void set_thread_blocked() {
        previous_state = current_state;
        current_state = BLOCKED;
    }

    void set_thread_new() {
        //previous_state = current_state;
        current_state = NEW;
    }

    void set_thread_ready() {
        previous_state = current_state;
        current_state = READY;
    }

    void set_thread_exit() {
        previous_state = current_state;
        current_state = EXIT;
    }

    int current_priority;

    

    


    // TODO: threads in our simulation have behavior, so add helper methods here
    //         that perform helpful logic (optional).
    //
    // For example, I have methods for setting the thread to running, blocked, and
    // so forth. These take care of updating the state and times based on the
    // provided event.

};
