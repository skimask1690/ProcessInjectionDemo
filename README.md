### CurrentThread
Allocates memory within the current process, writes a payload into it, marks the memory executable, and invokes the payload by calling it directly from the current thread.

### EnumDisplayMonitors
Allocates memory within the current process, writes a payload into it, marks the memory executable, and supplies the payload address to `EnumDisplayMonitors` as callback data so it is invoked through the enumeration callback.

### Process Hollow
Creates a target process in a suspended state, allocates memory within it, writes a payload into that memory, retrieves the primary thread context, updates the instruction pointer to the payload address, and resumes the thread.

### QueueUserAPC (Early Bird)
Creates a target process in a suspended state, allocates memory within it, writes a payload into that memory, queues the payload as an APC on the primary thread, and resumes the thread.

### RemoteThreadContext
Creates a target process, allocates memory within it, writes a payload, suspends the primary thread, retrieves and modifies its execution context to point to the payload, and resumes the thread.

### RemoteThreadSuspended
Creates a target process, allocates memory within it, writes a payload, creates a remote thread in a suspended state with the payload address as the start routine, and resumes the thread.
