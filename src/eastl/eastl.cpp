module;

#include <new>

module hellogpu.eastl;

void* operator new[](size_t size,
                     [[maybe_unused]] const char* pName,
                     [[maybe_unused]] int flags,
                     [[maybe_unused]] unsigned debugFlags,
                     [[maybe_unused]] const char* file,
                     [[maybe_unused]] int line)
{
    return ::operator new[](size);
}

void* operator new[](size_t size,
                     size_t alignment,
                     [[maybe_unused]] size_t alignmentOffset,
                     [[maybe_unused]] const char* pName,
                     [[maybe_unused]] int flags,
                     [[maybe_unused]] unsigned debugFlags,
                     [[maybe_unused]] const char* file,
                     [[maybe_unused]] int line)
{
    return ::operator new[](size, std::align_val_t(alignment));
}