#include "Process.h"
#include <iostream>
#include <queue>
#include <algorithm>
#include <limits>

// Helper to sort processes by arrival time for initial population of ready queue
bool compareArrivalForRR(const Process& a, const Process& b) {
    return a.arrivalTime < b.arrivalTime;
}

void RoundRobin::schedule(std::vector<Process>& processes, int quantum) {
    if (processes.empty() || quantum <= 0) {
        std::cout << "No processes or invalid quantum for Round Robin." << std::endl;
        return;
    }

    int n = processes.size();
    // Use a copy of processes to work with, as original processes might be needed for unsorted display
    std::vector<Process> tempProcesses = processes;
    std::sort(tempProcesses.begin(), tempProcesses.end(), compareArrivalForRR);

    std::queue<int> readyQueue; // Stores indices of processes in tempProcesses
    std::vector<bool> inReadyQueue(n, false); // To prevent adding same process multiple times
    std::vector<int> arrivalOrderIndices; // Keep track of the order of processes by arrival time

    // Initialize remainingTime and originalBurstTime
    for (int i = 0; i < n; ++i) {
        tempProcesses[i].remainingTime = tempProcesses[i].burstTime;
        tempProcesses[i].originalBurstTime = tempProcesses[i].burstTime;
        arrivalOrderIndices.push_back(i);
    }

    int currentTime = 0;
    int completedCount = 0;
    std::vector<std::string> ganttChart;
    std::vector<int> ganttTimes;

    // Track when the first process is added to ready queue
    bool firstProcessAdded = false;

    while (completedCount < n) {
        // Add newly arrived processes to the ready queue
        for (int i = 0; i < n; ++i) {
            if (tempProcesses[i].arrivalTime <= currentTime && !inReadyQueue[i] && tempProcesses[i].remainingTime > 0) {
                readyQueue.push(i);
                inReadyQueue[i] = true;
            }
        }

        if (readyQueue.empty()) {
            // If the ready queue is empty, and not all processes are completed,
            // advance time to the next process arrival.
            int nextArrivalTime = std::numeric_limits<int>::max();
            bool foundNext = false;
            for (int i = 0; i < n; ++i) {
                if (tempProcesses[i].remainingTime > 0) {
                    nextArrivalTime = std::min(nextArrivalTime, tempProcesses[i].arrivalTime);
                    foundNext = true;
                }
            }
            if (foundNext && currentTime < nextArrivalTime) {
                currentTime = nextArrivalTime;
            } else { // Should not happen if all processes are eventually scheduled
                currentTime++;
            }

            // Re-add newly arrived processes after time jump
            for (int i = 0; i < n; ++i) {
                if (tempProcesses[i].arrivalTime <= currentTime && !inReadyQueue[i] && tempProcesses[i].remainingTime > 0) {
                    readyQueue.push(i);
                    inReadyQueue[i] = true;
                }
            }

            if (readyQueue.empty()) { // If still empty, it means no more processes to schedule
                break;
            }
        }

        int currentProcessIndex = readyQueue.front();
        readyQueue.pop();
        inReadyQueue[currentProcessIndex] = false; // Mark as no longer in queue

        Process& p = tempProcesses[currentProcessIndex];

        int executionTime = std::min(quantum, p.remainingTime);

        // Add to Gantt chart
        ganttChart.push_back("| " + p.id + " ");
        ganttTimes.push_back(currentTime);

        currentTime += executionTime;
        p.remainingTime -= executionTime;

        // Check for new arrivals while this process was executing
        for (int i = 0; i < n; ++i) {
            if (tempProcesses[i].id != p.id && tempProcesses[i].arrivalTime <= currentTime &&
                !inReadyQueue[i] && tempProcesses[i].remainingTime > 0) {
                readyQueue.push(i);
                inReadyQueue[i] = true;
            }
        }

        if (p.remainingTime == 0) {
            p.completionTime = currentTime;
            p.turnaroundTime = p.completionTime - p.arrivalTime;
            p.waitingTime = p.turnaroundTime - p.originalBurstTime; // Use originalBurstTime
            completedCount++;
        } else {
            // If not completed, add back to ready queue
            readyQueue.push(currentProcessIndex);
            inReadyQueue[currentProcessIndex] = true;
        }
    }
    ganttTimes.push_back(currentTime); // Final completion time

    // Copy results back to original processes vector (important if we want main to display sorted or original)
    // Here we'll just display from tempProcesses, but in a real scenario you might map IDs
    for(size_t i = 0; i < processes.size(); ++i) {
        for(size_t j = 0; j < tempProcesses.size(); ++j) {
            if (processes[i].id == tempProcesses[j].id) {
                processes[i] = tempProcesses[j];
                break;
            }
        }
    }

    displayResults(processes, ganttChart, ganttTimes, quantum);
}

void RoundRobin::displayResults(const std::vector<Process>& processes,
                                const std::vector<std::string>& ganttChart,
                                const std::vector<int>& ganttTimes,
                                int quantum) {
    std::cout << "\n--- Round Robin (Quantum = " << quantum << ") Scheduling ---" << std::endl;

    // Display Table (sorting by original ID for consistent output)
    std::vector<Process> sortedProcesses = processes;
    std::sort(sortedProcesses.begin(), sortedProcesses.end(), [](const Process& a, const Process& b){
        return a.id < b.id; // Sort by process ID
    });

    std::cout << "\nProcess\tAT\tBT\tCT\tTAT\tWT" << std::endl;
    std::cout << "---------------------------------------------------" << std::endl;
    for (const auto& p : sortedProcesses) {
        std::cout << p.id << "\t" << p.arrivalTime << "\t" << p.originalBurstTime << "\t" // Use originalBurstTime
                  << p.completionTime << "\t" << p.turnaroundTime << "\t"
                  << p.waitingTime << std::endl;
    }

    // Calculate Averages
    double totalTAT = 0;
    double totalWT = 0;
    for (const auto& p : sortedProcesses) {
        totalTAT += p.turnaroundTime;
        totalWT += p.waitingTime;
    }
    std::cout << "\nAverage Turnaround Time = " << totalTAT / sortedProcesses.size() << std::endl;
    std::cout << "Average Waiting Time = " << totalWT / sortedProcesses.size() << std::endl;

    // Display Gantt Chart
    std::cout << "\nGantt Chart (Round Robin):" << std::endl;
    for (const auto& entry : ganttChart) {
        std::cout << entry;
    }
    std::cout << "|\n";

    // Print time markers below the Gantt chart
    for (size_t i = 0; i < ganttTimes.size(); ++i) {
        std::cout << ganttTimes[i];
        if (i < ganttChart.size()) { // Only add spaces if there's a next segment
             // Calculate width of the process string in Gantt chart for spacing
            int processStringWidth = ganttChart[i].length();
            for (int k = 0; k < processStringWidth - std::to_string(ganttTimes[i]).length() ; ++k) {
                std::cout << " ";
            }
        }
    }
    std::cout << std::endl;
}