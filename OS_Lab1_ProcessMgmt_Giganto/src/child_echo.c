// src/child_echo.c (TEMPORARY MODIFICATION FOR TASK D)
#include <stdio.h>
#include <stdlib.h>   // For atoi
#include <windows.h>  // For GetCurrentProcessId, Sleep

int main(int argc, char *argv[]) {
    DWORD current_pid = GetCurrentProcessId();
    DWORD parent_pid = 0;

    if (argc > 1) {
        parent_pid = atoi(argv[1]);
        printf("Child Process (PID: %lu) created by Parent (PID: %lu)\n", current_pid, parent_pid);
    } else {
        printf("Child Process (PID: %lu) started without a specified parent PID.\n", current_pid);
    }

    printf("Child (PID: %lu) will now sleep for 20 seconds. Observe in PowerShell.\n", current_pid);
    Sleep(20000); // Sleep for 20 seconds

    printf("Child (PID: %lu) waking up and exiting normally.\n", current_pid);

    return 0; // Normal termination
}
