// src/create_two_children.c
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int main() {
    DWORD parent_pid = GetCurrentProcessId();
    printf("Parent Process (PID: %lu) starting to create two children...\n", parent_pid);

    STARTUPINFO si[2];
    PROCESS_INFORMATION pi[2];
    HANDLE child_handles[2]; // Array to store process handles for WaitForMultipleObjects

    char command_line1[256];
    char command_line2[256];

    // Prepare command lines for two children
    sprintf(command_line1, "child_echo.exe %lu", parent_pid);
    sprintf(command_line2, "child_echo.exe %lu", parent_pid);

    // Create Child 1
    ZeroMemory(&si[0], sizeof(si[0]));
    si[0].cb = sizeof(si[0]);
    ZeroMemory(&pi[0], sizeof(pi[0]));

    if (!CreateProcess(NULL, command_line1, NULL, NULL, FALSE, 0, NULL, NULL, &si[0], &pi[0])) {
        fprintf(stderr, "CreateProcess for child 1 failed (%lu).\n", GetLastError());
        return 1;
    }
    printf("Child 1 (PID: %lu, Thread ID: %lu) created.\n", pi[0].dwProcessId, pi[0].dwThreadId);
    child_handles[0] = pi[0].hProcess; // Store handle for waiting

    // Create Child 2
    ZeroMemory(&si[1], sizeof(si[1]));
    si[1].cb = sizeof(si[1]);
    ZeroMemory(&pi[1], sizeof(pi[1]));

    if (!CreateProcess(NULL, command_line2, NULL, NULL, FALSE, 0, NULL, NULL, &si[1], &pi[1])) {
        fprintf(stderr, "CreateProcess for child 2 failed (%lu).\n", GetLastError());
        // Clean up handle from first child if it was created successfully
        CloseHandle(pi[0].hProcess);
        CloseHandle(pi[0].hThread);
        return 1;
    }
    printf("Child 2 (PID: %lu, Thread ID: %lu) created.\n", pi[1].dwProcessId, pi[1].dwThreadId);
    child_handles[1] = pi[1].hProcess; // Store handle for waiting

    // Wait until BOTH child processes exit.
    // The 'TRUE' argument means 'wait for all handles to be signaled'.
    WaitForMultipleObjects(2, child_handles, TRUE, INFINITE);

    // Get and print exit codes for both children
    DWORD exit_code1, exit_code2;

    if (GetExitCodeProcess(pi[0].hProcess, &exit_code1)) {
        printf("Child 1 (PID: %lu) exited with code: %lu\n", pi[0].dwProcessId, exit_code1);
    } else {
        fprintf(stderr, "GetExitCodeProcess for child 1 failed (%lu).\n", GetLastError());
    }

    if (GetExitCodeProcess(pi[1].hProcess, &exit_code2)) {
        printf("Child 2 (PID: %lu) exited with code: %lu\n", pi[1].dwProcessId, exit_code2);
    } else {
        fprintf(stderr, "GetExitCodeProcess for child 2 failed (%lu).\n", GetLastError());
    }

    // Close process and thread handles for both children.
    CloseHandle(pi[0].hProcess);
    CloseHandle(pi[0].hThread);
    CloseHandle(pi[1].hProcess);
    CloseHandle(pi[1].hThread);

    printf("Parent Process (PID: %lu) finished after children.\n", parent_pid);

    return 0;
}