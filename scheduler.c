#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>

#define NUM_PROCS 3
#define WORK_STEPS 8          
#define STEP_USEC  400000     
#define RR_QUANTUM 1         

pid_t pids[NUM_PROCS];
int   alive[NUM_PROCS]; 


void child_work(int id)
{
    setbuf(stdout, NULL);

    raise(SIGSTOP);

    for (int i = 1; i <= WORK_STEPS; i++) {
        printf("  [Process %d | PID %d] working... step %d/%d\n",
               id, getpid(), i, WORK_STEPS);
        usleep(STEP_USEC);
    }
    printf("  [Process %d | PID %d] FINISHED\n", id, getpid());
    exit(0);
}

void update_alive(void)
{
    for (int i = 0; i < NUM_PROCS; i++) {
        if (alive[i]) {
            int status;
            pid_t r = waitpid(pids[i], &status, WNOHANG);
            if (r == pids[i]) {
                alive[i] = 0;
            }
        }
    }
}

int any_alive(void)
{
    update_alive();
    for (int i = 0; i < NUM_PROCS; i++) {
        if (alive[i]) return 1;
    }
    return 0;
}

/* ---------- FCFS Scheduling ---------- */
void schedule_fcfs(void)
{
    printf("\n=== Starting FCFS Scheduler ===\n");
    printf("Processes will run one after another to completion.\n\n");

    for (int i = 0; i < NUM_PROCS; i++) {
        if (!alive[i]) continue;

        printf(">>> Dispatching Process %d (PID %d)\n", i, pids[i]);
        kill(pids[i], SIGCONT);

        int status;
        waitpid(pids[i], &status, 0);
        alive[i] = 0;

        printf("<<< Process %d finished\n\n", i);
    }

    printf("=== FCFS completed ===\n");
}

/* ---------- Round Robin Scheduling ---------- */
void schedule_rr(void)
{
    printf("\n=== Starting Round Robin Scheduler ===\n");
    printf("Time quantum = %d second(s)\n\n", RR_QUANTUM);

    int current = 0;

    while (any_alive()) {
        int found = 0;
        for (int tries = 0; tries < NUM_PROCS; tries++) {
            if (alive[current]) {
                found = 1;
                break;
            }
            current = (current + 1) % NUM_PROCS;
        }
        if (!found) break;

        printf(">>> Dispatching Process %d (PID %d) for %d sec\n",
               current, pids[current], RR_QUANTUM);

        kill(pids[current], SIGCONT);
        sleep(RR_QUANTUM);

        update_alive();
        if (alive[current]) {
            kill(pids[current], SIGSTOP);
        } else {
            printf("<<< Process %d finished during its quantum\n", current);
        }

        current = (current + 1) % NUM_PROCS;
        printf("\n");
    }

    printf("=== Round Robin completed ===\n");
}


void create_processes(void)
{
    for (int i = 0; i < NUM_PROCS; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(1);
        }
        if (pid == 0) {
            child_work(i);
        }

        int status;
        waitpid(pid, &status, WUNTRACED);
        if (!WIFSTOPPED(status)) {
            fprintf(stderr, "Child %d did not stop as expected\n", i);
        }

        pids[i]  = pid;
        alive[i] = 1;
        printf("Created Process %d  (PID %d)  [stopped]\n", i, pid);
    }
}

void cleanup(void)
{
    for (int i = 0; i < NUM_PROCS; i++) {
        if (alive[i]) {
            kill(pids[i], SIGKILL);
            waitpid(pids[i], NULL, 0);
            alive[i] = 0;
        }
    }
}

int main(void)
{
    printf("============================================\n");
    printf("  Simple User-Space Process Scheduler\n");
    printf("  Algorithms: FCFS and Round Robin\n");
    printf("============================================\n\n");

    int choice;
    while (1) {
        printf("\nChoose scheduling algorithm:\n");
        printf("  1. FCFS  (First Come First Served)\n");
        printf("  2. Round Robin\n");
        printf("  3. Exit\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            /* clear bad input */
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }

        if (choice == 3) {
            break;
        }

        cleanup();
        create_processes();

        if (choice == 1) {
            schedule_fcfs();
        } else if (choice == 2) {
            schedule_rr();
        } else {
            printf("Invalid choice.\n");
        }
    }

    cleanup();
    printf("\nScheduler exited. Goodbye!\n");
    return 0;
}