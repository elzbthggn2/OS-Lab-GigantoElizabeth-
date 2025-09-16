# OS Lab 1: Windows Process Management
**Student Name:** Elizabeth Giganto
**Objective:**  
- Learn how processes are created and managed in Windows using WinAPI.
- Practice creating, managing, and terminating child processes.
- Observe process behavior in PowerShell.

<!-- Build Instructions (VS Code + MinGW-w64) -->
## Build Instructions (VS Code + MinGW-w64)

1. Open VS Code in the project folder.
2. Open a terminal in VS Code.
3. Compile each C file using GCC:
```bash
gcc src/child_echo.c -o src/child_echo.exe
gcc src/create_basic.c -o src/create_basic.exe
gcc src/create_two_children.c -o src/create_two_children.exe
gcc src/replace_sim.c -o src/replace_sim.exe

<!-- Run the executables in the terminal -->
./src/create_basic.exe
./src/create_two_children.exe
./src/replace_sim.exe
## Sample Outputs

### Task A – Creating a Single Child
- Parent PID: 106908
- Child PID: 133776
- Child exited normally with code: 0

![Task A Output](screenshots/taskA_output.png)

### Notes on Results

*   The PIDs displayed for the parent and child processes were unique for each execution, as expected.
*   The parent PID printed by `create_basic.c` matched the "Parent PID (from argument)" reported by `child_echo.c`, confirming successful argument passing.
*   The child process reported its own PID, which was distinct from the parent's PID.
*   The parent successfully waited for the child's completion, indicated by the child's output appearing before the parent reported the "Child exited with code: 0".
*   The `exit code: 0` confirms the child process terminated successfully.

---

### Task B – Creating Two Children
- Parent PID: 137336
- Child 1 PID: 129332
- Child 2 PID: 12940
- Both children exited normally with code: 0

![Task B Output](screenshots/taskB_output.png)
### Notes on Results

*   Two child processes were launched, each with a unique PID, and both successfully received the parent's PID as an argument. The order of output from the children might vary due to parallel execution, but both completed before the parent reported their exit codes. If `WaitForMultipleObjects` was used, the parent waited for both simultaneously.
---

### Task C – Simulating exec()
- Parent PID: 75700
- Child PID: 38084
- Parent exits after child process completes
- Exit Code: 0

![Task C Output](screenshots/taskC_output.png)
### Notes on Results

*   The parent process (`replace_sim.exe`) quickly launched `cmd /c dir`. The `dir` command's output was displayed directly in the parent's console, and the parent then immediately terminated with the exit code of the `dir` command. This effectively demonstrates that the parent launched the child and adopted its exit status, behaving similarly to `exec()` where the calling process is overlaid by the new process.

---

### Task D – Managing Processes in PowerShell
- Before stopping child process:
```powershell
Get-Process | Where-Object {$_.ProcessName -like "*child*"}
Stop-Process -Id 104408

### Notes on Results

*    When `child_echo.exe` was running (for example, by adding a `Sleep` in `child_echo.c` to keep it alive), `Get-Process child_echo` successfully listed its PID and other details in PowerShell. When `Stop-Process -Id <PID_of_child>` was executed, the child process was immediately terminated by the operating system

---

### ✅ Notes for Adding Screenshots

1. Save your images in your project folder under:

2. Name them clearly according to tasks:  
3. Make sure the **file names in Markdown** match the actual image files.

## Conclusion

This laboratory exercise provided practical experience with Windows process creation and management using the WinAPI. It covered basic parent-child communication, parallel process execution, `exec()`-like behavior, and external process control via PowerShell.
