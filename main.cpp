#include <iostream>

void testArraySize(size_t sizeInMB) {
    const int size = sizeInMB * 1024 * 1024 / sizeof(int); // Convert MB to element count

    std::cout << "Trying to allocate " << sizeInMB << "MB on stack... ";

    // Use volatile to prevent optimization
    volatile int* arr = (int*)alloca(size * sizeof(int));
    
    // Do something with arr to prevent unused warning
    arr[0] = 1;
    arr[size - 1] = 1;

    std::cout << "Success!\n";
}

int main() {
    size_t sizeMB = 0.5;
    while (true) {
        testArraySize(sizeMB);
        sizeMB *= 2; // Double the size
    }

    return 0;
}
