#include <iostream>
#include <cstdlib>

#ifdef _WIN32
    #include <windows.h>
    #include <processthreadsapi.h>
#else
    #include <sys/resource.h>
    #include <pthread.h>
#endif

int StackSize(){

    #ifdef _WIN32
        // On Windows, you need to read the TIB (Thread Information Block)
        NT_TIB* tib = (NT_TIB*)NtCurrentTeb();
        SIZE_T stackSize = (SIZE_T)tib->StackBase - (SIZE_T)tib->StackLimit;
        std::cout << "Stack size (Windows): " << stackSize / 1024 << " KB\n";
        return stackSize;
    #else
        struct rlimit rl;
        if (getrlimit(RLIMIT_STACK, &rl) == 0) {
            std::cout << "Stack size (Linux): " << (rl.rlim_cur / (1024 * 1024)) << " MB\n";
        } else {
            std::perror("getrlimit");
        }
    #endif

    return 0;
}
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
                    // Only catch and handle the exception if it’s a stack overflow. 
                    // If it’s anything else, just ignore this handler and let the system handle it (or crash)
            std::cout << "Stack overflow at " << sizeBytes << " bytes!" << std::endl;
            return false; // Signal failure to stop loop
        }
   #endif
}

int main() {
    size_t startSize = 100;  
    size_t maxSize = StackSize() * sizeof(int);
    size_t sizeBytes = startSize;

    while (sizeBytes <= maxSize) {
        std::cout << "Trying " << sizeBytes << " bytes..." << std::endl;
        if (!testArraySize(sizeBytes)) {
            std::cout << "Program terminated due to stack overflow." << std::endl;
            break;
        }
        #ifdef _WIN32
            Sleep(200);
        #endif  
        sizeBytes = static_cast<size_t>(sizeBytes * 1.5); // Gradual increase
    }

    return 0;
}