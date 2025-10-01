#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <limits>

// =============================
// Process Structure
// =============================
struct Process {
    std::string id;          // Process ID (e.g., "P1", "P2")
    int arrivalTime;         // Arrival Time
    int burstTime;           // Burst Time
    int completionTime;      // Completion Time
    int turnaroundTime;      // Turnaround Time
    int waitingTime;         // Waiting Time
    int remainingTime;       // For Round Robin / preemption
    int originalBurstTime;   // To keep track of original burst time

    // Default constructor
    Process()
        : id(""), arrivalTime(0), burstTime(0),
          completionTime(0), turnaroundTime(0), waitingTime(0),
          remainingTime(0), originalBurstTime(0) {}

    // Parameterized constructor
    Process(std::string id, int at, int bt)
        : id(id), arrivalTime(at), burstTime(bt),
          completionTime(0), turnaroundTime(0), waitingTime(0),
          remainingTime(bt), originalBurstTime(bt) {}
};

// =============================
// FCFS Scheduling Class
// =============================
class FCFS {
public:
    void schedule(std::vector<Process>& processes);
private:
    void displayResults(const std::vector<Process>& processes,
                        const std::vector<std::string>& ganttChart,
                        int finalTime);
};

// =============================
// SJF Scheduling Class
// =============================
class SJF {
public:
    void schedule(std::vector<Process>& processes);
private:
    void displayResults(const std::vector<Process>& processes,
                        const std::vector<std::string>& ganttChart,
                        const std::vector<int>& ganttTimes);
};

// =============================
// Round Robin Scheduling Class
// =============================
class RoundRobin {
public:
    void schedule(std::vector<Process>& processes, int quantum);
private:
    void displayResults(const std::vector<Process>& processes,
                        const std::vector<std::string>& ganttChart,
                        const std::vector<int>& ganttTimes,
                        int quantum);
};

#endif // PROCESS_H
