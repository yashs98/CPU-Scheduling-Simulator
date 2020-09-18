# CPU-Scheduling-Simulator


The goal of this project was to develop a CPU scheduling simulation that will complete the execution of a group of multi-threaded processes. It will support several different scheduling algorithms. The user can then specify which one to use via a command-line flag. At the end of execution, the program will calculate and display several performance criteria obtained by the simulation. 

Project overview: 
•	The simulation will process scheduling on a hypothetical computer system.  
•	The input file serves two purposes: (a) general system & simulation parameters (b) process/thread trace that the simulation is going to simulate. Following information is assumed to be provided in the input file: o (a) Parameters: 
▪	The total number of processes 
▪	Process and thread switching overhead o (b) Trace: 
▪	For every process: 
•	Priority of a process 
•	Total number of threads in a process ▪ 	For every thread: 
•	First arrival time of a thread: The time when the thread issues its first burst. 
•	CPU&I/O bursts: A burst corresponds CPU and I/O resource usage duration requested by the thread at a time. Throughout their lifetime, threads will ask for multiple bursts based on their CPU and I/O needs.  
•	The simulation is event-based (https://en.wikipedia.org/wiki/Discrete-event_simulation). The simulator will not proceed by every clock internal like it does in real HW. Rather, the simulator will build an event queue based on the thread arrival times and bursts, and the main loop of the simulator will iterate based on this event queue.   

2.	Additional Information 
1.	There is a single CPU, so only one process can be running at a time. 
2.	There are an infinite number of I/O devices, so any number of processes can be blocked on I/O at the same time. 
3.	Processes consist of one or more kernel-level threads (KLTs). 
4.	Threads (not processes) can exist in one of five states: 

        •	NEW 
        •	READY 
        •	RUNNING 
        •	BLOCKED 
        •	EXIT 
        
5.	Dispatching threads requires a non-zero amount of OS overhead. 
•	If the previously executed thread belongs to a different process than the new thread, a full process switch occurs. This is also the case for the first thread being executed. 
•	If the previously executed thread belongs to the same process as the new thread being dispatched, a cheaper thread switch is done. 
•	A full process switch includes any work required by a thread switch. 
6.	Threads, processes, and dispatch overhead are specified via a file whose format is specified in the next section. 
7.	Each thread requires a sequence of CPU and I/O bursts of varying lengths as specified by the file. 
8.	Processes have an associated priority, specified as part of the file. Each thread in a process has the same priority as its owning process. 
•	0: SYSTEM (highest priority) 
•	1: INTERACTIVE 
•	2: NORMAL 
•	3: BATCH (lowest priority) 
9.	All processes have a distinct process ID, specified as part of the file. Thread IDs are unique only within the context of their owning process (so the first thread in every process has an ID of 0). 
10.	Overhead is incurred only when dispatching a thread (transitioning it from READY to RUNNING); all other OS actions require zero OS overhead. For example, adding a thread to a ready queue or initiating I/O are both free. 
11.	Threads for a given process can arrive at any time, even if some other process is currently running (i.e. some external entity is responsible for creating threads). 
12.	Threads get executed, not processes. 
 
3.	Command-Line Flags 
The simulation supports invocation in the format specified below, including the following command-line flags: 
./simulator [flags] simulation_file.txt 
-t, --per_thread 
Output additional per-thread statistics for arrival time, service time, etc. -v, --verbose 
Output information about every state-changing event and scheduling decision. 
-a, --algorithm 
The scheduling algorithm to use. One of FCFS, RR, PRIORITY, or CUSTOM. 
-h --help 
Display a help message about these flags and exit 
Users should be able to pass the -t or -v flags separately or together. The required output formats for these flags are described in the next section. 
 
The -a flag will dictate the scheduling algorithm your simulator should use. If not set, FCFS should be used as the default. Additional details about the algorithms you must implement are described in a later section. 
 
Finally, the --help (or -h) flag must cause your program to print out instructions for how to run your program and about the flags, it accepts and then immediately exit. 

4.	Simulation File Format 
The simulation file provides a complete specification of a scheduling scenario. Its format is as follows: 
num_processes thread_switch_overhead process_switch_overhead 
 
process_id process_type num_threads  thread_0_arrival_time num_CPU_bursts cpu_time io_time cpu_time io_time 
... // repeat for the number of CPU bursts cpu_time // the last CPU burst can NOT have I/O 
  
thread_1_arrival_time num_CPU_bursts cpu_time io_time cpu_time io_time 
... // repeat for the number of CPU bursts cpu_time // the last CPU burst can NOT have I/O 
... (repeat for the number of threads) 
 
... (repeat for the number of processes) 
Here’s an example. Note that the comments won’t be in the actual files. 
 
2 3 7 // 2 processes, thread/process overheads are 3 and 7  
0 1 2 // process 0 (type 1: interactive) has 2 threads 
      0 3 // the first thread arrives at time 0 and has 3 bursts 
4 5 // CPU burst of 4 and I/O of 5 
3 6 // CPU burst of 3 and I/O of 6 
1 // last CPU burst takes 1 unit; thread terminates 
 
1	2 // the second thread arrives at time 1 and has 2 bursts 
2	2 // CPU burst of 2 and I/O of 2 
7 // last CPU burst takes 7 units; thread terminates  
1 0 3 // process 1 (type 0: system) has 3 threads 
5 3 // the first thread arrives at time 5 and has 3 bursts 
4 1 // CPU burst of 4 and I/O of 1 
2 2 // CPU burst of 2 and I/O of 2 
2 // last CPU burst takes 2 units; thread terminates 
 
6 2 // the second thread arrives at time 6 and has 2 bursts 
2	2 // CPU burst of 2 and I/O of 2 
3	// last CPU burst takes 3 units; thread terminates 
 
7 5 // the third thread arrives at time 7 and has 5 bursts 
5 7 // CPU burst of 5 and I/O of 7 
2 1 // CPU burst of 2 and I/O of 1 
8 1 // CPU burst of 8 and I/O of 1 
5 7 // CPU burst of 5 and I/O of 7 
3 // last CPU burst takes 3 units; thread terminates 
  
 
5.	Output Format 
 
In all cases, regardless of the flags a user passes, the simulation outputs the following information: 
•	For each type of process (types 0 - 3): 
o 	The total number of such threads o 	The average response time o 	The average turnaround time 
•	The total time required to execute all threads to completion 
•	The time spent executing user processes (service time) 
•	The time spent performing I/O (sum of all I/O bursts) 
•	The time spent doing process and thread switches (dispatching overhead) 
•	The amount of time the CPU was idle 
•	CPU utilization 
•	CPU efficiency 
Here is an example in the required format: 
 
SYSTEM THREADS: 
Total count: 3 
Avg response time: 23.33 
Avg turnaround time: 94.67 
INTERACTIVE THREADS: Total count: 2 
Avg response time: 10.00 
Avg turnaround time: 73.50 
NORMAL THREADS: 
Total count: 0 
Avg response time: 0.00 
Avg turnaround time: 0.00 
BATCH THREADS: Total count: 0 
Avg response time: 0.00 
Avg turnaround time: 0.00 
Total elapsed time: 130 
Total service time: 53 
Total I/O time: 34 
Total dispatch time: 69 
Total idle time: 8 
 
CPU utilization: 93.85% 
CPU efficiency: 40.77% 
 
If the -t or --per thread flag is set, the program will also output the arrival time, service time, I/O time, turnaround time, and finish time for each thread. 
 
Process 0 [INTERACTIVE]: 
Thread 0: ARR: 0 CPU: 8 I/O: 11 TRT: 88 END: 88 
Thread 1: ARR: 1 CPU: 9 I/O: 2 TRT: 59 END: 60 
Process 1 [SYSTEM]: 
Thread 0: ARR: 5 CPU: 8 I/O: 3 TRT: 92 END: 97 
Thread 1: ARR: 6 CPU: 5 I/O: 2 TRT: 69 END: 75 
Thread 2: ARR: 7 CPU: 23 I/O: 16 TRT: 123 END: 130 
Finally, if the -v or --verbose flag is set, the program must also output a brief description of every state-changing event and scheduling decision. 
 
At time 11: 
CPU BURST COMPLETED 
Thread 0 in process 0 [INTERACTIVE] 
Transitioned from RUNNING to BLOCKED 
At time 11: 
DISPATCHER INVOKED 
Thread 1 in process 0 [INTERACTIVE] 
Selected from 4 threads; will run to completion of burst 
At time 14: 
THREAD DISPATCH COMPLETED 
Thread 1 in process 0 [INTERACTIVE] Transitioned from READY to RUNNING 
 
6.	Scheduling Algorithms 
The scheduling simulator supports four different scheduling algorithms. These are as follows, with the corresponding flag value indicated in parentheses: 
•	First-Come, First-Served (--algorithm FCFS) 
•	Round Robin (--algorithm RR) 
•	Process-Priority Scheduling, described below (--algorithm PRIORITY) 
•	Multi-level feedback queues (--algorithm MLFQ) 

6.1.	FCFS and RR 
FIFO is first come first served. The RR algorithm should use a hard-coded time slice size of your choosing. The default value of the time quantum should be 3, so that preemption events are common. 
 
6.2.	PRIORITY 
The process-priority scheduling algorithm is a non-preemptive algorithm that uses four separate firstcome, first-served ready queues. They consist of the following: 
•	Queue 0: Dedicated to threads whose processes are of type SYSTEM. 
•	Queue 1: Dedicated to threads whose processes are of type INTERACTIVE. 
•	Queue 2: Dedicated to threads whose processes are of type NORMAL. 
•	Queue 3: Dedicated to threads whose processes are of type BATCH. 
 
The next thread to run is taken from the front of the highest non-empty queue (with queue 0 being the highest). For example, if queue 0 is non-empty, the next thread will be taken from it. If queue 2 is the highest non-empty queue, the next thread will be drawn from it instead. This scheduling algorithm thus preferentially chooses ready threads from higher-priority process types first. 

 
 
7.	Next-Event Simulation 
The simulation structure follows the next-event pattern. At any given time, the simulation is in a single state. The simulation state can only change at event times, where an event is defined as an occurrence that may change the state of the system. 
 
Since the simulation state only changes at an event, the ”clock” can be advanced to the next scheduled event–regardless of whether the next event is 1 or 1,000,000 time units in the future. This is why it is called a ”next-event” simulation model. In our case, time is measured in simple ”units”. The simulation supports the following event types: 
 
•	THREAD ARRIVED: A thread has been created in the system. 
•	THREAD DISPATCH COMPLETED: A thread switch has completed, allowing a new thread to start executing on the CPU. 
•	PROCESS DISPATCH COMPLETED: A process switch has completed, allowing a new thread in a different process to start executing on the CPU. 
•	CPU BURST COMPLETED: A thread has finished one of its CPU bursts and has initiated an I/O request. 
•	IO BURST COMPLETED: A thread has finished one of its I/O bursts and is once again ready to be executed. 
•	THREAD COMPLETED: A thread has finished the last of its CPU bursts. 
•	THREAD PREEMPTED: A thread has been preempted during execution of one of its CPU bursts. 
•	DISPATCHER INVOKED: The OS dispatcher routine has been invoked to determine the next thread to be run on the CPU. 
 
Events are scheduled via an event queue. The event queue is a priority queue that contains future events; the priority of each item in the queue corresponds to its scheduled time, where the event with the highest priority (at the front of the queue) is the one that will happen next. 
 
The main loop of the simulation should consist of processing the next event, perhaps adding more future events in the queue as a result, advancing the clock (by taking the next scheduled event from the front of the event queue), and so on until all threads have terminated. 
