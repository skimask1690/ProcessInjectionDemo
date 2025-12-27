## Injection Techniques

### CurrentThread
Allocates memory in the current process, injects a payload, marks the memory as executable, and executes the payload directly on the current thread.

### EnumDisplayMonitors
Injects a payload into the current process, marks it executable, and passes its address to `EnumDisplayMonitors` to be invoked via the enumeration callback.

### Process Hollowing
Creates a target process in a suspended state, injects a payload into its memory, modifies the primary thread's instruction pointer to the payload, and resumes execution.

### QueueUserAPC (Early Bird)
Suspends a target process, injects a payload into its memory, queues the payload as an APC on the primary thread, and resumes the process.

### RemoteThreadContext
Suspends the primary thread of a target process, injects a payload, modifies the thread's execution context to point to the payload, and resumes the thread.

### RemoteThreadSuspended
Injects a payload into a target process, creates a suspended remote thread starting at the payload, and then resumes the thread.
