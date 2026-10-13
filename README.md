# Simple User-Space Process Scheduler

A demonstration program that implements two classic CPU scheduling algorithms in user space:

- **FCFS** (First-Come, First-Served)
- **Round Robin** (with fixed time quantum)

The parent process acts as the scheduler. Child processes are created with `fork()`, immediately stopped with `SIGSTOP`, and then controlled by the parent using `SIGCONT` / `SIGSTOP`.

## Features

- Interactive menu to choose the scheduling algorithm
- 3 child processes, each performing 8 work steps
- Clear console output showing when processes are dispatched and finished
- Clean process management (no zombie processes)

## Requirements

- Linux / Unix-like system
- GCC (or any C compiler)
- Standard POSIX libraries

## Compilation

```bash
gcc -o scheduler scheduler.c
```

## Running

```bash
./scheduler
```

Then choose:

```
1 → FCFS
2 → Round Robin
3 → Exit
```

## How it works

1. Parent creates 3 child processes.
2. Each child immediately calls `raise(SIGSTOP)` and waits to be scheduled.
3. Parent uses:
   - `kill(pid, SIGCONT)` to resume a process
   - `kill(pid, SIGSTOP)` to suspend a process
   - `waitpid()` to detect when a process finishes
4. In **FCFS**, each process runs to completion before the next one starts.
5. In **Round Robin**, each process gets a 1-second time quantum, then is stopped and the next ready process is resumed.

## Configuration (in source code)

| Macro         | Default | Meaning                          |
|---------------|---------|----------------------------------|
| `NUM_PROCS`   | 3       | Number of child processes        |
| `WORK_STEPS`  | 8       | Work units each process performs |
| `STEP_USEC`   | 400000  | Microseconds per work step       |
| `RR_QUANTUM`  | 1       | Time quantum in seconds (RR)     |

## Notes

- This is a **user-space simulation**, not a real kernel scheduler.
- Timing is approximate (based on `sleep()`).
- Output may interleave slightly because of concurrent writing to stdout.
