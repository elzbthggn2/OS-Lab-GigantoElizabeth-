#include <iostream>
#include <vector>
#include <string>
#include <limits> // For std::numeric_limits

#include "Process.h" // Only this one is needed now

// Function to get valid integer input
int getValidIntInput(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.fail() || value < 0) {
            std::cout << "Invalid input. Please enter a non-negative integer." << std::endl;
            std::cin.clear(); // Clear the error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // flush input
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
    }
}

int main() {
    int numProcesses = getValidIntInput("Enter the number of processes: ");

    std::vector<Process> processes;
    for (int i = 0; i < numProcesses; ++i) {
        std::cout << "\nEnter details for Process P" << i + 1 << ":" << std::endl;
        int at = getValidIntInput("  Arrival Time (AT): ");
        int bt = getValidIntInput("  Burst Time (BT): ");
        processes.emplace_back("P" + std::to_string(i + 1), at, bt);
    }

    // --- FCFS Simulation ---
    std::vector<Process> fcfsProcesses = processes;
    FCFS fcfsScheduler;
    std::cout << "\n===== First Come First Serve (FCFS) =====" << std::endl;
    fcfsScheduler.schedule(fcfsProcesses);

    // --- SJF Simulation ---
    std::vector<Process> sjfProcesses = processes;
    SJF sjfScheduler;
    std::cout << "\n===== Shortest Job First (SJF) =====" << std::endl;
    sjfScheduler.schedule(sjfProcesses);

    // --- Round Robin Simulation ---
    int quantum = getValidIntInput("\nEnter quantum for Round Robin (e.g., 2 or 3): ");
    std::vector<Process> rrProcesses = processes;
    RoundRobin rrScheduler;
    std::cout << "\n===== Round Robin (Quantum = " << quantum << ") =====" << std::endl;
    rrScheduler.schedule(rrProcesses, quantum);

    return 0;
}
