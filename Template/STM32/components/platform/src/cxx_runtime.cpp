// Minimal C++ runtime support for embedded systems without standard library

#include <stddef.h>

//operator new and delete are required forC++ with virtual functions
//even when execptions and RTTI are disabled. These operators are used by the compiler to allocate and deallocate memory for objects created with new and delete expressions. In embedded systems, we often don't have a standard library, so we need to provide our own implementations of these operators.    

//Minimal malloc/free implementation for embedded systems without standard library
static unsigned char heap[1024]; // 1KB heap
static size_t heap_pos = 0;

extern "C"
{
    void __libc_init_array()
    {
        // No static constructors are used in this minimal embedded setup.
    }

    void *malloc(size_t size)
    {
        if (heap_pos + size > sizeof(heap))
        {
            return nullptr; // Out of memory
        }
        void *ptr = &heap[heap_pos];
        heap_pos += size;
        return ptr;
    }

    void free(void *ptr)
    {
        // In this simple implementation, we don't actually free memory
        // You can implement a more sophisticated memory management if needed
        (void)ptr; // Avoid unused parameter warning
    }
}

void *operator new(size_t size)
{
    return malloc(size);
}

void operator delete(void *ptr) noexcept
{
    free(ptr);
}

void operator delete(void *ptr, size_t size) noexcept
{
    free(ptr);
}

void *operator new[](size_t size)
{
    return malloc(size);
}

void operator delete[](void *ptr) noexcept
{
    free(ptr);
}

void operator delete[](void *ptr, size_t size) noexcept
{
    (void)size; // Avoid unused parameter warning
    free(ptr);
}

// Pure virtual function call handler
extern "C" void __cxa_pure_virtual()
{
    // Handle pure virtual function call error
    while (1)
    {
        // Infinite loop to indicate an error
    }
}