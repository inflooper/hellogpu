module;

#include <EASTL/numeric_limits.h>
#include <EASTL/unique_ptr.h>

export module hellogpu.eastl;

export namespace eastl
{
    using ::eastl::unique_ptr;
    using ::eastl::make_unique;
    using ::eastl::numeric_limits;

    template<typename T, typename Deleter = ::eastl::default_delete<T>>
    using uptr = unique_ptr<T, Deleter>;
}