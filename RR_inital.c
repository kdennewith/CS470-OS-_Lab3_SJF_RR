#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int process_id;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int waiting_time;
    int turnaround_time;
} Process;

// Function to calculate waiting times and execution order for all processes
void roundRobin(Process proc[], int numProcesses, int quantum) {
    int time = 0;
    int done = 0;
    int execution_order[1000];
    int order_index = 0;

    // Error Check: Time Quantaum has to be above 0 or there won't be an interval to
    // do the processes in, 0 doesn't work and negatives don't exist in time.
    if (quantum <= 0) {
        fprintf(stderr, "Error: Quantum unit must be a positive integer.\n");
        exit(EXIT_FAILURE);
    }
    // Error Check: If there aren't any processes then there's nothing to do so just exit.
    if (numProcesses == 0) {
        fprintf(stderr, "Error: No processes found to schedule.\n");
        exit(EXIT_FAILURE);
    }

    // Initializing the time it takes for a process to execute and the intial wait time of 0.
    for (int i = 0; i < numProcesses; i++) {
        proc[i].remaining_time = proc[i].burst_time;
        proc[i].waiting_time = 0;
    }

    /* 
    * This will run until done. The Integer done iterates up by 1 each time a process finishs, so when the amount finished is equal too,
    * or greater than (if something weird happened) the amount of processes that are "done" that mean's they have all executed and the 
    * loop will exit. Done starts at 0 that's why it is a less than check since numProccesses starts at 1 and "done" starts at 0. 
    */
    while (done < numProcesses) {
        int executionTracker = 0;
        for (int i = 0; i < numProcesses; i++) {
            /* 
            * If the arrival time of a process is less than the current time, which means
            * that it has arrived. Also check if the remaining time of the process is greater than 0 to make sure
            * the process is executing currently and not finished. 
            */
            if (proc[i].arrival_time < time && proc[i].remaining_time > 0) {
                executionTracker = 1;

                printf("Time %d: Process %d started execution! \n", time, proc[i].process_id);

                /*
                * This is the next execution time which is either the quantime time unit, or if the remaining time of the process
                * is less than the quantum time unit then the execution time segment will be the remaining time of the process. So if
                * the remaining time is 3 then that will be the execution time since 4 is the quantum time I'm using at the moment in main.
                */
                int executionTime = (proc[i].remaining_time > quantum) ? quantum : proc[i].remaining_time;

                /*
                * The remaining time is always going to be the intial remaining time minus the current execution time. This repeats
                * itself each time a process has left. If the quantum is 4 then it would just be -=4 basically unless proc[i].remaining_time
                * is returned into executionTime then it would be -= 1,2, or 3 based on remaining time of the process at i.
                */
                proc[i].remaining_time -= executionTime;
                
                // The overall time that's elapsed.
                time += executionTime;
                
                // Adding the currently executing process to the order array and then
                //  iterating the order_index by 1 so it won't overlap.
                execution_order[order_index++] = proc[i].process_id;

                /*
                * Adding to the amount of done processes if the remaining time of proc[i] is 0
                * Calculating the turnaround time of a process by subtracting the arrival time 
                * from the total time the program's been executing. Could also be said as
                * the Exit-time - the arrival time in a sort.
                * Afterwards just calculating the waiting time and storing it in it's process[i].
                * Then printing the exit message for the process to show in the terminal.
                */
                if(proc[i].remaining_time == 0){
                    done++;
                    proc[i].turnaround_time = time - proc[i].arrival_time;
                    proc[i].waiting_time = proc[i].turnaround_time - proc[i].burst_time;

                    printf("Time %d: Process %d has finished executing... \n", time, proc[i].process_id);
                }
            }  
        }
        // If nothing was executed, which means something is currently processing an amount of 4, since the
        //  quantum is 4 right now in my main.
        if(!executionTracker){
            time++;
        }
    }

    // Print the execution order after the while loop is done followed by a new line for the next print statements.
    printf("Execution Order: ");
    for (int i = 0; i < order_index; i++) {
        printf("%d ", execution_order[i]);
    }
    printf("\n");
}

// This function prints the processes using a specific format to fit the word + a space after.
void printProcesses(Process proc[], int numProcesses) {
    printf("%-12s%-13s%-11s%-13s%-16s\n", 
    "Process ID", "Arrival Time", "Burst Time", "Waiting Time", "Turnaround Time" );
    for (int i = 0; i < numProcesses; i++) {
        // These lengths are chosen because of the upper row word lengths + 1, like Process ID is 11 + 1 width.
        //  These are alligned to the left with the negative sign. Also in line with the top row.
        printf("%-12d%-13d%-11d%-13d%-16d\n",
               proc[i].process_id, 
               proc[i].arrival_time, 
               proc[i].burst_time,
               proc[i].waiting_time,
               proc[i].turnaround_time);
    }
}

/*
* Believe it's the execution time of O(3n + n). 3 for loops and a while loop of length n.
*/
int main() {
    Process proc[] = {{1, 1, 24}, {2, 2, 3}, {3, 4, 3}};
    // This divides the size of proc by using byte size of the whole array and the byte size
    // of the first element, this gets the amount. This array is 36 bytes since each
    // int is 4 bytes, so it'd be 36bytes/12bytes = 3 numProcesses.
    int numProcesses = sizeof(proc) / sizeof(proc[0]);
    int quantum = 4; // The Quantum time unit

    roundRobin(proc, numProcesses, quantum);
    printProcesses(proc, numProcesses);

    return 0;
}
