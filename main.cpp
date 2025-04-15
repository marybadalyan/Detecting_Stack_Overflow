#include <iostream>
#include <cstdlib>
#include <windows.h>  // For Sleep

void testArraySize(size_t sizeInMB) {
    size_t count = sizeInMB * 1024 * 1024 / sizeof(int);
    volatile int* arr = (int*)_alloca(count * sizeof(int)); // MSVC version of alloca
    arr[0] = 1;
    arr[count - 1] = 1;
}

int main() {
    for (size_t sizeMB = 1;; sizeMB *= 2) {
        std::cout << "Trying " << sizeMB << "MB..." << std::endl;
        Sleep(200); // To see output before crash
        testArraySize(sizeMB);
        std::cout << "Success\n";
    }

    return 0;
}
