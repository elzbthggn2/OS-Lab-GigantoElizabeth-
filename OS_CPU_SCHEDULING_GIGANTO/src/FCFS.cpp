#include "Process.h"
#include <iostream>
#include <algorithm> // For std::sort

// Function to compare processes based on arrival time for FCFS
bool compareArrival(const Process& a, const Process& b) {
    return a.arrivalTime < b.arrivalTime;
}

void FCFS::schedule(std::vector<Process>& processes) {
    std::sort(processes.begin(), processes.end(), compareArrival);

    int currentTime = 0;
    std::vector<std::string> ganttChart;

    for (size_t i = 0; i < processes.size(); ++i) {
        if (currentTime < processes[i].arrivalTime) {
            currentTime = processes[i].arrivalTime;
        }

        ganttChart.push_back("| " + processes[i].id + " ");

        processes[i].completionTime = currentTime + processes[i].burstTime;
        processes[i].turnaroundTime = processes[i].completionTime - processes[i].arrivalTime;
        processes[i].waitingTime = processes[i].turnaroundTime - processes[i].burstTime;

        currentTime = processes[i].completionTime;
    }

    displayResults(processes, ganttChart, currentTime);
}

void FCFS::displayResults(const std::vector<Process>& processes,
                          const std::vector<std::string>& ganttChart,
                          int finalTime) {
    std::cout << "\n--- FCFS Scheduling ---" << std::endl;

    std::cout << "\nProcess\tAT\tBT\tCT\tTAT\tWT" << std::endl;
    std::cout << "---------------------------------------------------" << std::endl;
    for (const auto& p : processes) {
        std::cout << p.id << "\t" << p.arrivalTime << "\t" << p.burstTime << "\t"
                  << p.completionTime << "\t" << p.turnaroundTime << "\t"
                  << p.waitingTime << std::endl;
    }

    double totalTAT = 0, totalWT = 0;
    for (const auto& p : processes) {
        totalTAT += p.turnaroundTime;
        totalWT += p.waitingTime;
    }
    std::cout << "\nAverage Turnaround Time = " << totalTAT / processes.size() << std::endl;
    std::cout << "Average Waiting Time = " << totalWT / processes.size() << std::endl;

    std::cout << "\nGantt Chart (FCFS):" << std::endl;
    for (const auto& entry : ganttChart) {
        std::cout << entry;
    }
    std::cout << "|\n";

    int currentGanttTime = 0;
    std::cout << currentGanttTime;
    for (const auto& p : processes) {
        for(int i = 0; i < p.id.length() + 3; ++i) {
            std::cout << " ";
        }
        std::cout << p.completionTime;
    }
    std::cout << std::endl;
}
