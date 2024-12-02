#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_PROCESSES 5
#define TIME_QUANTUM 2
#define AGING_FACTOR 1

typedef struct {
    int id;
    int arrival_time;
    int burst_time;
    int priority;
    int remaining_time;
    int waiting_time;
    int turnaround_time;
    int start_time;
    int end_time;
} Process;

void generate_processes(Process processes[]) {
    srand(time(NULL));

    for (int i = 0; i < NUM_PROCESSES; i++) {
        processes[i].id = i + 1;
        processes[i].arrival_time = rand() % 10;
        processes[i].burst_time = rand() % 10 + 1;
        processes[i].priority = rand() % 5 + 1;
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].waiting_time = 0;
        processes[i].turnaround_time = 0;
         processes[i].start_time = 0;
        processes[i].end_time = 0;
    }
}

void sort_by_arrival_time(Process processes[]) {
    for (int i = 0; i < NUM_PROCESSES - 1; i++) {
        for (int j = i + 1; j < NUM_PROCESSES; j++) {
            if (processes[i].arrival_time > processes[j].arrival_time) {
                Process temp = processes[i];
                processes[i] = processes[j];
                processes[j] = temp;
            }
        }
    }
}

void sort_by_priority(Process processes[]) {
    for (int i = 0; i < NUM_PROCESSES - 1; i++) {
        for (int j = i + 1; j < NUM_PROCESSES; j++) {
            if (processes[i].priority < processes[j].priority) {
                Process temp = processes[i];
                processes[i] = processes[j];
                processes[j] = temp;
            }
        }
    }
}

void sort_by_burst_time(Process processes[]) {
    for (int i = 0; i < NUM_PROCESSES - 1; i++) {
        for (int j = i + 1; j < NUM_PROCESSES; j++) {
            if (processes[i].burst_time > processes[j].burst_time) {
                Process temp = processes[i];
                processes[i] = processes[j];
                processes[j] = temp;
            }
        }
    }
}



void simulate_round_robin(Process processes[]) {
    printf("\nRound Robin:\n");
    int time = 0;
    int completed = 0;

    while (completed < NUM_PROCESSES) {
        for (int i = 0; i < NUM_PROCESSES; i++) {
            if (processes[i].remaining_time > 0) {
                int time_slice = (processes[i].remaining_time < TIME_QUANTUM) ? processes[i].remaining_time : TIME_QUANTUM;
                processes[i].remaining_time -= time_slice;
                time += time_slice;


                printf("Process %d (Remaining time: %d)\n", processes[i].id, processes[i].remaining_time);


                if (processes[i].remaining_time == 0) {
                   processes[i].end_time = time;
                    completed++;
                }
            }
        }
    }
}

void simulate_fcfs(Process processes[]) {
    printf("\nFCFS:\n");
    sort_by_arrival_time(processes);
    int time = 0;

    for (int i = 0; i < NUM_PROCESSES; i++) {
        processes[i].start_time = (time > processes[i].arrival_time) ? time : processes[i].arrival_time;
        time = processes[i].end_time = processes[i].start_time + processes[i].burst_time;
        processes[i].waiting_time = processes[i].start_time - processes[i].arrival_time;
        processes[i].turnaround_time = processes[i].end_time - processes[i].arrival_time;


        printf("Process %d: Start %d, End %d, Waiting %d, Turnaround %d\n",
               processes[i].id, processes[i].start_time, processes[i].end_time, processes[i].waiting_time, processes[i].turnaround_time);
    }
}




void simulate_sjf(Process processes[]) {

     printf("\nShortest Job First (SJF):\n");
    Process sjf_processes[NUM_PROCESSES];

    for (int i = 0; i < NUM_PROCESSES; i++) {
        sjf_processes[i] = processes[i];
    }

    sort_by_burst_time(sjf_processes);


    int time = 0;

    for (int i = 0; i < NUM_PROCESSES; i++) {
         sjf_processes[i].start_time = (time > sjf_processes[i].arrival_time) ? time : sjf_processes[i].arrival_time;
         time = sjf_processes[i].end_time = sjf_processes[i].start_time + sjf_processes[i].burst_time;
         sjf_processes[i].waiting_time = sjf_processes[i].start_time - sjf_processes[i].arrival_time;
         sjf_processes[i].turnaround_time = sjf_processes[i].end_time - sjf_processes[i].arrival_time;


        printf("Process %d: Start %d, End %d, Waiting %d, Turnaround %d\n",
               sjf_processes[i].id, sjf_processes[i].start_time, sjf_processes[i].end_time, sjf_processes[i].waiting_time, sjf_processes[i].turnaround_time);


    }
}


void simulate_priority_aging(Process processes[]) {
    printf("\nPriority Scheduling with Aging:\n");

    Process aging_processes[NUM_PROCESSES];
    for (int i = 0; i < NUM_PROCESSES; i++) {
        aging_processes[i] = processes[i];
    }

    int time = 0;
    int completed = 0;

    while (completed < NUM_PROCESSES) {
        int highest_priority_process = -1;
        int highest_priority = -1;

        for (int i = 0; i < NUM_PROCESSES; i++) {
            if (aging_processes[i].arrival_time <= time && aging_processes[i].remaining_time > 0) {
                 aging_processes[i].priority += (time - aging_processes[i].arrival_time - aging_processes[i].waiting_time) * AGING_FACTOR;  // Aging
                if (aging_processes[i].priority > highest_priority) {
                    highest_priority = aging_processes[i].priority;
                    highest_priority_process = i;
                }
            }
        }


        if (highest_priority_process != -1) {
            aging_processes[highest_priority_process].start_time = time;
            time += aging_processes[highest_priority_process].burst_time;


            aging_processes[highest_priority_process].remaining_time = 0;
            aging_processes[highest_priority_process].end_time = time;
            aging_processes[highest_priority_process].waiting_time = aging_processes[highest_priority_process].start_time - aging_processes[highest_priority_process].arrival_time;


            aging_processes[highest_priority_process].turnaround_time = aging_processes[highest_priority_process].end_time - aging_processes[highest_priority_process].arrival_time;




            printf("Process %d: Start %d, End %d, Waiting %d, Turnaround %d, Priority %d\n",
                   aging_processes[highest_priority_process].id, aging_processes[highest_priority_process].start_time, aging_processes[highest_priority_process].end_time, aging_processes[highest_priority_process].waiting_time, aging_processes[highest_priority_process].turnaround_time, aging_processes[highest_priority_process].priority);




            completed++;




        } else {
            time++;
        }
    }
}






int main() {
    Process processes[NUM_PROCESSES];

    generate_processes(processes);
    simulate_round_robin(processes);

    generate_processes(processes);
    simulate_fcfs(processes);

    generate_processes(processes);
    simulate_sjf(processes);

    generate_processes(processes);
    simulate_priority_aging(processes);

    return 0;
}