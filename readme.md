# DESCRIPTION

The `sigaction()` system call is used to change the action taken by a process on receipt of a specific signal. (See `signal(7)` for an overview of signals.)

`signum` specifies the signal and can be any valid signal except `SIGKILL` and `SIGSTOP`.

If `act` is non-NULL, the new action for signal `signum` is installed from `act`. If `oldact` is non-NULL, the previous action is saved in `oldact`.

The `sigaction` structure is defined as something like:

```c
struct sigaction {
    void     (*sa_handler)(int);
    void     (*sa_sigaction)(int, siginfo_t *, void *);
    sigset_t   sa_mask;
    int        sa_flags;
    void     (*sa_restorer)(void);
};
```

On some architectures, a union is involved: do not assign to both `sa_handler` and `sa_sigaction`.

## The `siginfo_t` Argument to a `SA_SIGINFO` Handler

When the `SA_SIGINFO` flag is specified in `act.sa_flags`, the signal handler address is passed via the `act.sa_sigaction` field. This handler takes three arguments, as follows:

```c
void handler(int sig, siginfo_t *info, void *ucontext) {
    ...
}
```

These three arguments are as follows:

- `sig`: The number of the signal that caused invocation of the handler.
- `info`: A pointer to a `siginfo_t`, which is a structure containing further information about the signal, as described below.
- `ucontext`: This is a pointer to a `ucontext_t` structure, cast to `void *`. The structure pointed to by this field contains signal context information that was saved on the user-space stack by the kernel; for details, see `sigreturn(2)`. Further information about the `ucontext_t` structure can be found in `getcontext(3)` and `signal(7)`. Commonly, the handler function doesn't make any use of the third argument.

## Advanced Uses of the `ucontext` Field

The `ucontext` field in the signal handler can be used for several advanced purposes beyond debugging. Here are a few examples:

1. **Inspecting and Modifying Register Values**:
   You can inspect and modify the CPU registers at the time the signal was delivered. This can be useful for implementing custom context switching, exception handling, or even for creating a user-level threading library.

2. **Custom Signal Handling**:
   You can use the `ucontext` to implement custom signal handling logic, such as modifying the program counter to skip over a faulty instruction or to retry an operation.

3. **User-Level Checkpointing and Restart**:
   You can save the `ucontext` to create a checkpoint of the program's state and later restore it to resume execution from that point. This can be useful for fault tolerance and recovery.

4. **Stack Unwinding**:
   You can use the `ucontext` to perform stack unwinding, which is useful for implementing custom exception handling mechanisms or for generating stack traces.

Here's an example of how you might modify the program counter to skip over a faulty instruction:

```c
#include <signal.h>
#include <ucontext.h>
#include <stdio.h>

void handler(int sig, siginfo_t *info, void *ucontext) {
    ucontext_t *context = (ucontext_t *)ucontext;

    // Print the instruction pointer (program counter) at the time of the signal
    printf("Signal %d received. Instruction pointer: %p\n", sig, (void *)context->uc_mcontext.gregs[REG_RIP]);

    // Skip over the faulty instruction by incrementing the program counter
    context->uc_mcontext.gregs[REG_RIP] += 2; // Assuming the instruction is 2 bytes long

    // Continue with your signal handling logic...
}

int main() {
    struct sigaction sa;
    sa.sa_sigaction = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO;
    if (sigaction(SIGSEGV, &sa, NULL) == -1)
        exit(EXIT_FAILURE);
    if (sigaction(SIGUSR1, &sa, NULL) == -1)
        exit(EXIT_FAILURE);
    while (1)
        pause();
    return 0;
}
```

In this example, when a `SIGSEGV` (segmentation fault) signal is received, the handler prints the current instruction pointer and then increments it to skip over the faulty instruction. This is a simplistic example and may not be safe or appropriate for all situations, but it demonstrates the kind of control you can achieve with the `ucontext` parameter.
```

## Understanding Unicode & write

```c
#include <unistd.h>
int main()
{
	char	c1;
	char	c2;
	char	*s = "ğ";
	c1 = *s;
	s++;
	c2 = *s;
	write(1, &c1, 1);
	write(1, &c2, 1);
}
```