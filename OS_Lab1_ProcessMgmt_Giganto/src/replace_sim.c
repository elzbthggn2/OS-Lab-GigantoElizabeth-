    // src/replace_sim.c
    #include <stdio.h>
    #include <stdlib.h>
    #include <windows.h>

    int main() {
        DWORD parent_pid = GetCurrentProcessId();
        printf("Parent Process (PID: %lu) starting, simulating exec()...\n", parent_pid);

        STARTUPINFO si;
        PROCESS_INFORMATION pi;

        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));

        // Example command: cmd /c dir
        // This will open a command prompt, run 'dir', and then exit.
        // Replace with any other command you want to simulate.
        char command_line[] = "cmd /c dir"; // Note: Needs to be modifiable for CreateProcess
        // Or you could launch one of your child_echo.exe programs:
        // char command_line[] = "child_echo.exe 0"; // Pass 0 as parent PID since this parent will exit

        // The key here is to NOT wait for the child.
        if (!CreateProcess(
                NULL,               // No module name (use command line)
                command_line,       // Command line
                NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi
            )) {
            fprintf(stderr, "CreateProcess failed (%lu).\n", GetLastError());
            return 1; // Parent exits with an error code
        }

        printf("Child Process (PID: %lu, Thread ID: %lu) launched. Parent will now exit.\n", pi.dwProcessId, pi.dwThreadId);

        // IMPORTANT: In a true exec() scenario, the new process replaces the old.
        // Here, we simulate by having the parent immediately exit after creating the child.
        // We also want to exit with the child's exit code.
        // To get the child's exit code, the parent would normally have to wait.
        // Since we're *not* waiting (to simulate immediate replacement), we can't directly get the child's exit code here
        // without introducing a wait.
        // A closer simulation for the exit code might involve a different strategy
        // or acknowledging this limitation. For now, we'll just exit.
        // If you were to wait briefly and then exit, it would be less like exec.

        // For a strict 'exit with same code as spawned program', the parent *would* need to wait.
        // If the intent is that the parent *doesn't* wait, then it can't know the child's exit code.
        // Let's assume the spirit of the task is that the parent's *execution* quickly ends.
        // If we want the *exit code* to match, the parent *must* wait.

        // Let's modify: the parent will wait to get the exit code, but will not
        // perform any further actions, simulating a quick "hand-off".
        WaitForSingleObject(pi.hProcess, INFINITE);

        DWORD child_exit_code;
        if (!GetExitCodeProcess(pi.hProcess, &child_exit_code)) {
            fprintf(stderr, "GetExitCodeProcess failed (%lu).\n", GetLastError());
            child_exit_code = 1; // Default error
        }

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        printf("Parent Process (PID: %lu) exiting with child's exit code: %lu\n", parent_pid, child_exit_code);
        return child_exit_code; // Parent exits with the child's exit code
    }