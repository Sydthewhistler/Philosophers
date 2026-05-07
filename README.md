# philosophers

**The dining philosophers problem — solved with threads and mutexes.**


---

## Overview

`philosophers` is a simulation of the classic **dining philosophers problem**, originally formulated by Edsger Dijkstra. It serves as a practical introduction to concurrent programming in C — covering thread creation, mutex synchronization, race condition avoidance, and millisecond-accurate timing.

Each philosopher sits at a round table with a fork between every pair of adjacent philosophers. To eat, a philosopher must acquire both the fork on their left and the fork on their right. The challenge is to allow all philosophers to eat and think indefinitely without any of them starving — and without causing deadlocks or data races.

---

## Rules

- Each philosopher is a **thread**.
- Each fork is protected by a **mutex**.
- Philosophers cycle through three states: **thinking**, **eating**, and **sleeping**.
- If a philosopher has not eaten within `time_to_die` milliseconds since their last meal (or since the simulation started), they die.
- The simulation stops as soon as one philosopher dies, or once every philosopher has eaten at least `number_of_times_each_philosopher_must_eat` times (if specified).
- Philosophers do not communicate with each other and cannot know if another philosopher is about to die.

---

## Usage

```bash
make
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

| Argument | Type | Description |
|----------|------|-------------|
| `number_of_philosophers` | int | Number of philosophers (and forks) at the table |
| `time_to_die` | int (ms) | Time before a philosopher dies if they have not started eating |
| `time_to_eat` | int (ms) | Time it takes to eat (both forks must be held) |
| `time_to_sleep` | int (ms) | Time spent sleeping before thinking again |
| `number_of_times_each_philosopher_must_eat` | int (optional) | Simulation ends when all philosophers have eaten this many times |

**Examples:**

```bash
# 5 philosophers, no one should die
./philo 5 800 200 200

# Simulation ends after each philosopher eats 7 times
./philo 5 800 200 200 7

# One philosopher — must die
./philo 1 800 200 200
```

---

## Output Format

Every state change is logged to standard output with a timestamp in milliseconds:

```
timestamp_in_ms X has taken a fork
timestamp_in_ms X is eating
timestamp_in_ms X is sleeping
timestamp_in_ms X is thinking
timestamp_in_ms X died
```

Log messages are protected by a mutex to ensure they are never interleaved or printed after a death has been detected.

---

## Implementation Details

### Thread management

Each philosopher is spawned as a POSIX thread with `pthread_create`. A dedicated **monitor thread** (or in-loop checks) continuously verifies that no philosopher has exceeded `time_to_die` without eating.

### Fork allocation strategy

To prevent deadlocks, philosophers do not simply pick up their left fork and then wait for the right. An ordering strategy is applied:

- **Even-indexed** philosophers pick up their right fork first.
- **Odd-indexed** philosophers pick up their left fork first.

This breaks the circular wait condition that would otherwise cause a deadlock with an even number of philosophers.

### Death detection

The monitor checks each philosopher's last meal timestamp against the current time on every iteration. As soon as a death is detected, a shared flag is set and all threads stop their activity at their next state check.

---

## Concurrency Considerations

| Risk | Mitigation |
|------|-----------|
| Data race on philosopher state | Per-philosopher mutex or atomic flag |
| Interleaved log output | Dedicated print mutex |
| Deadlock on fork acquisition | Asymmetric fork pickup order |
| Death detected too late | Tight monitoring loop with precise timestamps |
| Philosopher acting after death | Shared simulation-stop flag checked before each action |

---

## Build

```bash
make        # Compile
make clean  # Remove object files
make fclean # Remove object files and binary
make re     # Full recompile
```

---

## What This Project Teaches

- **Multithreading** — creating and synchronizing concurrent execution flows with pthreads
- **Mutex usage** — protecting shared resources and understanding the cost of locking
- **Deadlock prevention** — recognizing and breaking circular wait conditions
- **Race condition awareness** — identifying every shared variable and ensuring safe access
- **Timing in concurrent systems** — measuring and controlling time with precision under scheduling uncertainty
- **Thread-safe output** — ensuring logs remain coherent when multiple threads write simultaneously

---

## Requirements

- **Compiler:** `gcc` with flags `-Wall -Wextra -Werror`
- **OS:** Unix/Linux (POSIX threads)
- **Norm:** [Norminette v3](https://github.com/42School/norminette) compliant
- **No data races** — verified with `helgrind` / `ThreadSanitizer`
- **No memory leaks** — verified with `valgrind`

---

## Author

**scavalli** — [GitHub](https://github.com/Sydthewhistler) · 42 School