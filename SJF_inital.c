#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct {
    int process_id;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int waiting_time;
    int turnaround_time;
    int is_completed;
} Process;

int numProcesses; // Number of processes

// Function to find the index of the process with the shortest remaining time
int findNextProcess(Process proc[], int current_time) {
    int min_time = INT_MAX;
    int index = -1;
    for (int i = 0; i < numProcesses; i++) {
        if (proc[i].arrival_time <= current_time && !proc[i].is_completed) {
            if (proc[i].remaining_time < min_time) {
                min_time = proc[i].remaining_time;
                index = i;
            }
        }
    }
    return index;
}

// SRTF function
void srtf(Process proc[]) {

    int time = 0;
    int done = 0;
    int prev = -1;
    int order_index = 0;
    int execution_order[1000];

    // Error Check: If there aren't any processes then there's nothing to do so just exit.
    if (numProcesses == 0) {
        fprintf(stderr, "Error: No processes found to schedule.\n");
        exit(EXIT_FAILURE);
    }

    // Error Check: For duplicate id's
    for (int i = 0; i < numProcesses; i++) {
        for (int j = i + 1; j < numProcesses; j++) {
            if (proc[i].process_id == proc[j].process_id) {
                fprintf(stderr, "Error: Duplicate process ID found.\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    // Initializing all the current processes variables.
    for (int i = 0; i < numProcesses; i++) {
        proc[i].remaining_time = proc[i].burst_time;
        proc[i].waiting_time = 0;
        proc[i].turnaround_time = 0;
        proc[i].is_completed = 0;
    }

    // Build the loop to execute processes in the queue list
    while (done < numProcesses) {
        int index = findNextProcess(proc, time);
        if (index != -1) {
            // Process found to execute so I print 
            if (prev != index) {
                printf("Time %d: Process %d started to execute! \n", time, proc[index].process_id); // Print execution order
            }
            // Iterating the remaining time by -1 each loop to represent time taken to execute
            //  and then iterating the current time by +1 to calculate things like turnover time
            // and keep track of the total time of execution.
            proc[index].remaining_time--;
            time++;

            // Adding the currently executing process to the order array and then
            //  iterating the order_index by 1 so it won't overlap.
            execution_order[order_index++] = proc[index].process_id;

            /*
            * If a process completes then like in RR code but a little different. The completed status
            * is set to 1 (or true in c, wherre 0 is false), increment the amount of processes "done" by 1
            * calculating the turnround time the same way as RR. 
            */
            if (proc[index].remaining_time == 0) {
                proc[index].is_completed = 1;
                printf("Time %d: Process %d done executing...\n", time, proc[index].process_id);
                done++;
                proc[index].turnaround_time = time - proc[index].arrival_time;
                proc[index].waiting_time = proc[index].turnaround_time - proc[index].burst_time;
            }
            // Just setting the prev variable kind of like in a linked list or tree, basically just index - 1 in the
            //  next iteration after prev is assigned.
            prev = index;
        } else {
            // No process available to execute at this time so just adding time
            time++;
        }
    
    }
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

int main() {
    // ID, Arrival time, Burst Time
    Process proc[] = {{1, 0, 8}, {2, 1, 4}, {3, 2, 9}, {4, 3, 5}};
    numProcesses = sizeof(proc) / sizeof(proc[0]);

    srtf(proc);
    printProcesses(proc, numProcesses);

    return 0;
}
