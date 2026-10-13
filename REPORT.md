# Report: Simple User-Space Process Scheduler

**File:** `scheduler.c`  
**Purpose:** Educational demonstration of process scheduling algorithms using signals and process control in Linux.

---

## 1. Overview

This program implements a **user-space process scheduler** that supports two classic algorithms:

1. **FCFS (First-Come, First-Served)**  
2. **Round Robin (RR)**

Instead of relying on the kernel’s real scheduler, the parent process takes full control of three child processes by stopping and resuming them with POSIX signals (`SIGSTOP` / `SIGCONT`).

---

## 2. Design & Architecture

### 2.1 Process Model

- Parent process = **Scheduler**
- 3 Child processes = **Jobs** to be scheduled

Each child:
1. Immediately stops itself (`raise(SIGSTOP)`)
2. Performs a fixed amount of work (8 steps of 0.4 seconds each)
3. Exits when finished

### 2.2 Key Data Structures

```c
pid_t pids[NUM_PROCS];   // stores child PIDs
int   alive[NUM_PROCS];  // tracks which processes are still running
```

### 2.3 Core Functions

| Function            | Responsibility                                      |
|---------------------|-----------------------------------------------------|
| `create_processes()`| Forks children and waits until they are stopped     |
| `child_work()`      | Work loop of each child process                     |
| `schedule_fcfs()`   | Runs processes one-by-one to completion             |
| `schedule_rr()`     | Round-robin with fixed time quantum                 |
| `update_alive()`    | Non-blocking check if any child has exited          |
| `cleanup()`         | Kills remaining children on exit or re-run          |

---

## 3. Scheduling Algorithms

### 3.1 FCFS (First-Come, First-Served)

- Processes are run in the order they were created (0 → 1 → 2).
- Once a process is started with `SIGCONT`, the parent **blocks** with `waitpid()` until it finishes.
- No preemption.
- Simple and fair in arrival order, but can suffer from convoy effect.

### 3.2 Round Robin

- Time quantum = **1 second** (`RR_QUANTUM`).
- Parent cycle:
  1. Resume current process (`SIGCONT`)
  2. Sleep for 1 second
  3. If process still alive → suspend it (`SIGSTOP`)
  4. Move to next ready process
- Continues until all processes have finished.
- Provides better responsiveness and fairness than FCFS for interactive workloads.

---

## 4. Implementation Highlights

### Process Creation & Synchronization

```c
pid_t pid = fork();
if (pid == 0) {
    child_work(i);          // child path
}
waitpid(pid, &status, WUNTRACED);  // parent waits until child stops
```

This ensures every child is in the stopped state before the scheduler starts.

### Preemption Mechanism

Round Robin uses:

```c
kill(pids[current], SIGCONT);   // resume
sleep(RR_QUANTUM);              // run for quantum
kill(pids[current], SIGSTOP);   // preempt
```

### Detecting Termination

```c
waitpid(pids[i], &status, WNOHANG);
```

Non-blocking check prevents the scheduler from hanging on already finished processes.

---

## 5. Limitations

| Limitation                        | Explanation                                          |
|-----------------------------------|------------------------------------------------------|
| User-space only                   | Real kernel scheduler still decides actual CPU time  |
| Coarse timing                     | Based on `sleep()`, not precise timer interrupts     |
| Fixed quantum                     | No dynamic time quantum adjustment                   |
| No priority support               | All processes treated equally                        |
| Console output interleaving       | Multiple processes writing to stdout simultaneously  |
| Not suitable for real workloads   | Educational demonstration only                       |

---

## 6. How to Build & Run

```bash
gcc -o scheduler scheduler.c
./scheduler
```

Select option `1` for FCFS or `2` for Round Robin.

---

## 7. Learning Outcomes

This program helps understand:

- Process creation with `fork()`
- Process state control using signals (`SIGSTOP` / `SIGCONT`)
- Non-blocking process monitoring with `waitpid(WNOHANG)`
- Difference between non-preemptive (FCFS) and preemptive (RR) scheduling
- Basic structure of a simple scheduler

---

## 8. Possible Extensions

- Add Priority Scheduling
- Implement Shortest Job First (SJF)
- Dynamic time quantum
- Logging of waiting time / turnaround time
- Support for more than 3 processes
- Use real-time signals or timers for more accurate preemption

---

**Author note:** This is an educational tool intended to illustrate core operating system scheduling concepts in a simple and observable way.
