# Detecting_Stack_Overflow


# What is Stack Overflow?
A stack overflow occurs when a program attempts to use more memory on the call stack than is allocated, causing the program to crash or behave unpredictably. The call stack is a region of memory  in RAM used to store function call information, local variables, and control flow data. Each thread in a program has its own stack, and its size is limited. Exceeding this limit, often through large local variable allocations or deep recursion, triggers a stack overflow. This program tests the stack size limit by incrementally allocating arrays until a stack overflow occurs or the maximum stack size is reached.


# Code Explanation
This C++ program measures the stack size of the current thread and tests how much memory can be allocated on the stack before a stack overflow occurs. It supports both Windows and Linux/Unix-like systems using platform-specific APIs and error handling.


### Unix-like:
Uses ```getrlimit(RLIMIT_STACK)``` to retrieve the stack size limit.
Prints size in KB and returns it.
Returns 0 if the operation fails.
main Function:
- Purpose: Orchestrates the stack overflow test.
- Logic:
On Unix-like systems, calls ```setup_segfault_handler```.
Declares ```stack_guard[1024]``` to reserve stack space for the signal handler (avoids compiler warnings with ```(void)stack_guard```).
Retrieves the maximum stack size using ```StackSize```.
Starts with a small allocation (startSize = 100 bytes) and iteratively increases it by a factor of 1.5.
Calls testArraySize for each size, printing the result.
Stops if a stack overflow occurs or the maximum stack size is reached.
```setup_segfault_handler``` (Unix-like only):
- Purpose: Configures a signal handler for SIGSEGV (segmentation fault) to gracefully exit on stack overflow.
- Logic:
Disables core dumps using ```setrlimit(RLIMIT_CORE, {0, 0}).```
Sets up a ```SIGSEGV``` handler that exits the program with ```EXIT_FAILURE```.
Ensures the program terminates cleanly if a stack overflow triggers a segmentation fault.

### Windows:
Uses structured exception handling ```(__try/__except)``` to detect stack overflows hardware and software specific error handiling system.
Allocates memory with ```_malloca```, which uses the stack.
Retrieves stack size via the TIB whitch is a struct called ``` NT_TIB``` holding specific information aout threads adn their stack size.
```StackSize```Function:
- Purpose: Retrieves the stack size of the current thread.
- Logic:
- Windows:
Uses ```NtCurrentTeb()``` often treated as a macro to access the Thread Information Block (TIB).
Calculates stack size as the difference between StackBase and StackLimit.
Prints size in KB and returns it.


Example Output
```
Stack size (Linux): 16 KB
Allocated 19381 bytes
Trying 29071 bytes...
Allocated 29071 bytes
....
Trying 331129 bytes...
Allocated 331129 bytes
....
Allocated 3771757 bytes
Trying 5657635 bytes...
Allocated 5657635 bytes
Trying 8486452 bytes...
Allocated 8486452 bytes
Trying 12729678 bytes...
Allocated 12729678 bytes
```

## Build Instructions

1. **Clone the repository**:
    ```bash
    git clone https://github.com/marybadalyan/Detecting_Stack_Overflow
    ```

2. **Navigate to the repository**:
    ```bash
    cd Detecting_Stack_Overflow
    ```

3. **Generate build files**:
    ```bash
    cmake -DCMAKE_BUILD_TYPE=Release -S . -B build
    ```

4. **Build the project**:
    ```bash
    cmake --build build --config Release
    ```

5. **Run the executable** from the build directory:
    ```bash
    ./build/Detecting_Stack_Overflow
    ```
