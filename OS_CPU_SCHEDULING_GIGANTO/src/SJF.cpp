#include "Process.h"
#include <iostream>
#include <algorithm> // For std::sort
#include <limits>    // For std::numeric_limits

void SJF::schedule(std::vector<Process>& processes) {
    // Make a copy to preserve original order for initial display if needed
    // For SJF, we need to consider arrival times and then burst times.
    // It's a bit more complex than just sorting.
    int n = processes.size();
    std::vector<Process> tempProcesses = processes; // Use a copy for internal calculations
    std::vector<bool> completed(n, false);
    int currentTime = 0;
    int completedCount = 0;
    std::vector<std::string> ganttChart;
    std::vector<int> ganttTimes; // To store the times for the gantt chart

    while (completedCount < n) {
        int shortestJobIndex = -1;
        int minBurst = std::numeric_limits<int>::max();

        // Find the shortest job that has arrived and is not yet completed
        for (int i = 0; i < n; ++i) {
            if (!completed[i] && tempProcesses[i].arrivalTime <= currentTime) {
                if (tempProcesses[i].burstTime < minBurst) {
                    minBurst = tempProcesses[i].burstTime;
                    shortestJobIndex = i;
                }
            }
        }

        if (shortestJobIndex == -1) {
            // No process has arrived yet, increment time
            // Or if current time is before any process arrived, advance to first arrival
            int nextArrivalTime = std::numeric_limits<int>::max();
            for(int i = 0; i < n; ++i) {
                if (!completed[i]) {
                    nextArrivalTime = std::min(nextArrivalTime, tempProcesses[i].arrivalTime);
                }
            }
            if (nextArrivalTime != std::numeric_limits<int>::max()) {
                currentTime = nextArrivalTime;
            } else { // Should not happen if all processes are eventually scheduled
                currentTime++;
            }
            continue; // Re-evaluate after time advances
        }

        Process& currentProcess = tempProcesses[shortestJobIndex];

        // Add to Gantt chart
        ganttChart.push_back("| " + currentProcess.id + " ");
        ganttTimes.push_back(currentTime);

        currentProcess.completionTime = currentTime + currentProcess.burstTime;
        currentProcess.turnaroundTime = currentProcess.completionTime - currentProcess.arrivalTime;
        currentProcess.waitingTime = currentProcess.turnaroundTime - currentProcess.burstTime;

        currentTime = currentProcess.completionTime;
        completed[shortestJobIndex] = true;
        completedCount++;
    }
    ganttTimes.push_back(currentTime); // Add the final completion time to gantt times

    // Copy results back to original processes vector (important if we want main to display sorted or original)
    // Here we'll just display from tempProcesses, but in a real scenario you might map IDs
    // For this simple case, we just assume the 'processes' passed in is what we modify.
    // Let's copy results back based on process IDs
    for(size_t i = 0; i < processes.size(); ++i) {
        for(size_t j = 0; j < tempProcesses.size(); ++j) {
            if (processes[i].id == tempProcesses[j].id) {
                processes[i] = tempProcesses[j];
                break;
            }
        }
    }


    displayResults(processes, ganttChart, ganttTimes);
}

void SJF::displayResults(const std::vector<Process>& processes,
                         const std::vector<std::string>& ganttChart,
                         const std::vector<int>& ganttTimes) {
    std::cout << "\n--- SJF (Non-Preemptive) Scheduling ---" << std::endl;

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
    std::cout << "\nGantt Chart (SJF):" << std::endl;
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