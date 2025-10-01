
# OS_CPUScheduling_GIGANTO

## CPU Scheduling Simulation

This project implements and simulates three common CPU scheduling algorithms: First-Come, First-Served (FCFS), Shortest Job First (SJF) (Non-Preemptive), and Round Robin (RR). The simulation calculates and displays key performance metrics such as Completion Time (CT), Turnaround Time (TAT), Waiting Time (WT), and visualizes the scheduling order using a text-based Gantt chart.

This project simulates CPU scheduling algorithms using C++:
- **First-Come, First-Served (FCFS)**
- **Shortest Job First (SJF)**
- **Round Robin (RR)**

The program calculates **Completion Time (CT), Turnaround Time (TAT), and Waiting Time (WT)** for each process and displays Gantt charts for visualization.

---

## Algorithms Explained

### 1. First-Come, First-Served (FCFS)
FCFS schedules processes in the **order they arrive** in the ready queue.  
- Non-preemptive: once a process starts, it runs until completion.  
- Simple to implement but can suffer from **convoy effect** (long processes block shorter ones).

**OUTPUT**

Enter the number of processes: 3

Enter details for Process P1:
  Arrival Time (AT): 1
  Burst Time (BT): 2

Enter details for Process P2:
  Arrival Time (AT): 6
  Burst Time (BT): 5

Enter details for Process P3:
  Arrival Time (AT): 7
  Burst Time (BT): 4

**First Come First Serve (FCFS)**
--- FCFS Scheduling ---

Process AT      BT      CT      TAT     WT
---------------------------------------------------
P1      1       2       3       2       0
P2      6       5       11      5       0
P3      7       4       15      8       4

Average Turnaround Time = 5
Average Waiting Time = 1.33333

Gantt Chart (FCFS):
| P1 | P2 | P3 |
0     3     11     15

**Shortest Job First (SJF)**

--- SJF (Non-Preemptive) Scheduling ---

Process AT      BT      CT      TAT     WT
---------------------------------------------------
P1      1       2       3       2       0
P2      6       5       11      5       0
P3      7       4       15      8       4

Average Turnaround Time = 5
Average Waiting Time = 1.33333

**Gantt Chart (SJF):**
| P1 | P2 | P3 |
1    6    11   15

Enter quantum for Round Robin :3 (The cmd exit)
