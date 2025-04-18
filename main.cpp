#include <iostream>
#include <cstdlib>

#ifdef _WIN32
    #include <windows.h>
    #include <processthreadsapi.h>
#else
    #include <stdio.h>
    #include <stdlib.h>
    #include <signal.h>
    #include <malloc.h>
    #include <sys/resource.h>   

#endif


#ifndef _WIN32
void setup_segfault_handler() {
    // Prevent core dumps
    struct rlimit core_limit = {0, 0};
    setrlimit(RLIMIT_CORE, &core_limit);

    // Set up signal handler for segmentation fault
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = [](int, siginfo_t*, void*) {
        const char* msg = "Segmentation fault detected (probably a stack overflow).\n";
        write(STDERR_FILENO, msg, strlen(msg)); // Safe in signal handler
        _exit(EXIT_FAILURE); // Do not return from signal handler
    };
    sigemptyset(&sa.sa_mask);
    sigaction(SIGSEGV, &sa, nullptr);
}
#endif


bool testArraySize(size_t sizeBytes) {
    const size_t count = sizeBytes / sizeof(int); // Bytes to number of ints

    #ifdef _WIN32
        __try { // Structured Exception Handling
            volatile int* arr = (int*)_malloca(count * sizeof(int));
            arr[0] = 1;  
            arr[count - 1] = 1;  
            std::cout << "Allocated " << sizeBytes << " bytes" << std::endl;
            return true;
        }
        __except (GetExceptionCode() == EXCEPTION_STACK_OVERFLOW ? 
                EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) { 
                    std::cout << "Stack overflow at " << sizeBytes << " bytes!" << std::endl;
                    return false; // Signal failure to stop loop
        }
    #else
        // Allocate memory and check for stack overflow
        volatile int arr[count]; // Local array on the stack
        arr[0] = 1;
        arr[count - 1] = 1;

        totalAllocated += sizeBytes; // Track total allocated bytes

        std::cout << "Allocated " << sizeBytes << " bytes" << std::endl;
        return true;
    #endif
}


int StackSize(){
    #ifdef _WIN32
        // Thread Information Block
        NT_TIB* tib = (NT_TIB*)NtCurrentTeb();
        SIZE_T stackSize = (SIZE_T)tib->StackBase - (SIZE_T)tib->StackLimit;
        std::cout << "Stack size (Windows): " << stackSize / 1024 << " KB\n";
        return stackSize;
    #else
        struct rlimit rl;
        if (getrlimit(RLIMIT_STACK, &rl) == 0) {
            std::cout << "Stack size (Linux): " << (rl.rlim_cur / (1024*1024)) << " KB\n";
            return rl.rlim_cur;
        }
    #endif

    return 0;
}

int main() {
    #ifndef _WIN32
    setup_segfault_handler(); // Linux only
    #endif
    
    size_t startSize = 100;  
    size_t maxSize = StackSize() * sizeof(int);
    size_t sizeBytes = startSize;

    while (sizeBytes <= maxSize) {
        std::cout << "Trying " << sizeBytes << " bytes..." << std::endl;
        if (!testArraySize(sizeBytes)) {
            std::cout << "Program terminated due to stack overflow." << std::endl;
            break;
        }
        
        sizeBytes = static_cast<size_t>(sizeBytes * 1.5); 
    }
    
    return 0;
}