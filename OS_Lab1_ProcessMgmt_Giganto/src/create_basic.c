// src/create_basic.c
#include <stdio.h>
#include <stdlib.h> // For sprintf
#include <windows.h> // For WinAPI functions

int main() {
    // Get current process ID (parent's PID)
    DWORD parent_pid = GetCurrentProcessId();
    printf("Parent Process (PID: %lu) starting...\n", parent_pid);

    // Structure to hold information about the new process
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    // Initialize STARTUPINFO and PROCESS_INFORMATION structures
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Command line buffer for the child process
    // We'll pass the parent's PID as an argument to the child
    char command_line[256];
    // IMPORTANT: Use the full path or ensure child_echo.exe is in the same directory
    // or in a directory listed in the system's PATH variable.
    // For simplicity, we assume create_basic.exe and child_echo.exe are in the same dir.
    sprintf(command_line, "child_echo.exe %lu", parent_pid);

    // Create the child process
    // lpApplicationName = NULL (uses first token of lpCommandLine)
    // lpCommandLine     = The command line string
    // lpProcessAttributes = NULL (default security descriptor)
    // lpThreadAttributes  = NULL (default security descriptor)
    // bInheritHandles     = FALSE (child does not inherit handles)
    // dwCreationFlags     = 0 (no special creation flags)
    // lpEnvironment       = NULL (use parent's environment block)
    // lpCurrentDirectory  = NULL (use parent's current directory)
    // lpStartupInfo       = Pointer to STARTUPINFO
    // lpProcessInformation = Pointer to PROCESS_INFORMATION
    if (!CreateProcess(
            NULL,               // No module name (use command line)
            command_line,       // Command line
            NULL,               // Process handle not inheritable
            NULL,               // Thread handle not inheritable
            FALSE,              // Set handle inheritance to FALSE
            0,                  // No creation flags
            NULL,               // Use parent's environment block
            NULL,               // Use parent's starting directory
            &si,                // Pointer to STARTUPINFO structure
            &pi                 // Pointer to PROCESS_INFORMATION structure
        )) {
        fprintf(stderr, "CreateProcess failed (%lu).\n", GetLastError());
        return 1;
    }

    printf("Child Process (PID: %lu, Thread ID: %lu) created.\n", pi.dwProcessId, pi.dwThreadId);

    // Wait until child process exits.
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Get the child's exit code
    DWORD exit_code;
    if (GetExitCodeProcess(pi.hProcess, &exit_code)) {
        printf("Child Process (PID: %lu) exited with code: %lu\n", pi.dwProcessId, exit_code);
    } else {
        fprintf(stderr, "GetExitCodeProcess failed (%lu).\n", GetLastError());
    }

    // Close process and thread handles.
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    printf("Parent Process (PID: %lu) finished.\n", parent_pid);

    return 0;
}