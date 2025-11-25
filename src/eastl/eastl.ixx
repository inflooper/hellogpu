module;

#define eastl secreteastl
#include <EASTL/numeric_limits.h>
#include <EASTL/unique_ptr.h>
#undef eastl

export module hellogpu.eastl;

export namespace eastl
{

template<typename T, typename Deleter = secreteastl::default_delete<T>>
using unique_ptr = secreteastl::unique_ptr<T, Deleter>;

template<typename T, typename... Args>
inline typename secreteastl::enable_if<!secreteastl::is_array<T>::value, secreteastl::unique_ptr<T>>::type make_unique(
    Args&&... args)
{
    return secreteastl::make_unique<T>(args...);
}

template<typename T>
inline typename secreteastl::enable_if<secreteastl::is_unbounded_array<T>::value, eastl::unique_ptr<T>>::type
make_unique(size_t n)
{
    return secreteastl::make_unique<T>(n);
}

template<typename T>
using numeric_limits = secreteastl::numeric_limits<T>;
} // namespace eastl