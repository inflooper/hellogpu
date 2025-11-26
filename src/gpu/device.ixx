export module hellogpu.gpu:device;

import hellogpu.eastl;

export namespace il::gpu
{

enum class GpuDevicePreference
{
    None,
    HighPerformance,
    MinimumPower,
    MaximumMemory,
    MinimumMemory
};

class IDevice
{
public:
    virtual ~IDevice() = default;
};

eastl::uptr<IDevice> createDevice(GpuDevicePreference devicePreference);

} // namespace il::gpu