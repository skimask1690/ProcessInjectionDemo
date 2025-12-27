### CurrentThread
Allocates memory within the current process, writes a payload into it, marks the memory executable, and transfers execution to the payload by directly calling it in the current thread.

### EnumDisplayMonitors
Allocates memory within the current process, writes a payload into it, marks the memory executable, and executes the payload via a callback invoked by the `EnumDisplayMonitors` API.

### Process Hollow
Creates a target process in a suspended state, replaces its in‑memory executable image with a payload, updates the primary thread’s execution context to the new entry point, and resumes the process so it executes the injected code.

### QueueUserAPC (Early Bird)
Creates a target process in a suspended state, allocates memory and writes a payload into it, queues the payload as an APC on the primary thread before execution begins, and resumes the thread so the payload executes early.

### RemoteThreadContext
Creates a target process, allocates memory within it, writes a payload, suspends the primary thread, modifies its execution context to redirect the instruction pointer to the payload, and resumes the thread to execute the injected code.

### RemoteThreadSuspended
Creates a target process, allocates memory within it, writes a payload, creates a remote thread in a suspended state with its start address set to the payload, and resumes the thread to execute the injected code.
