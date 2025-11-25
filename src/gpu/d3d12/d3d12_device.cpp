module;

#include <d3d12.h>
#include <dxgi.h>
#include <dxgi1_4.h>
#include <dxgi1_5.h>
#include <dxgi1_6.h>
#include <SDL3/SDL_log.h>
#include <wrl/client.h>

#include <numeric>

module hellogpu.gpu;

import hellogpu.eastl;

using namespace Microsoft::WRL;

#ifdef HELLOGPU_COMPILER_CLANG
#    pragma clang diagnostic push
#    pragma clang diagnostic ignored "-Wlanguage-extension-token"
#endif

namespace il::gpu
{

namespace d3d12
{
    class Device : public IDevice
    {
    public:
        ComPtr<IDXGIAdapter4> mAdapter;
    };
} // namespace d3d12

constexpr DXGI_GPU_PREFERENCE getDxgiGpuPreference(GpuDevicePreference devicePreference)
{
    switch(devicePreference)
    {
    case GpuDevicePreference::HighPerformance: return DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE;
    case GpuDevicePreference::MinimumPower:    return DXGI_GPU_PREFERENCE_MINIMUM_POWER;
    case GpuDevicePreference::MaximumMemory:   [[fallthrough]];
    case GpuDevicePreference::MinimumMemory:   [[fallthrough]];
    case GpuDevicePreference::None:            [[fallthrough]];
    default:                                   return DXGI_GPU_PREFERENCE_UNSPECIFIED;
    }
}

ComPtr<IDXGIAdapter4> getHardwareAdapter(GpuDevicePreference devicePreference,
                                         [[maybe_unused]] D3D_FEATURE_LEVEL featureLevel,
                                         [[maybe_unused]] IDXGIFactory4& dxgiFactory4)
{
    const DXGI_GPU_PREFERENCE dxgiGpuPreference = getDxgiGpuPreference(devicePreference);

    // spdlog::info("Enumerating adapters with preference '{}'", gpuPreference);

    ComPtr<IDXGIAdapter4> dxgiAdapter4;

    ComPtr<IDXGIFactory6> dxgiFactory6;
    if(dxgiGpuPreference != DXGI_GPU_PREFERENCE_UNSPECIFIED &&
       SUCCEEDED(dxgiFactory4.QueryInterface(IID_PPV_ARGS(&dxgiFactory6))))
    {
        ComPtr<IDXGIAdapter1> dxgiAdapter1;

        for(UINT adapterIndex = 0;
            dxgiFactory6->EnumAdapterByGpuPreference(adapterIndex, dxgiGpuPreference, IID_PPV_ARGS(&dxgiAdapter1)) !=
            DXGI_ERROR_NOT_FOUND;
            ++adapterIndex)
        {
            DXGI_ADAPTER_DESC1 dxgiAdapterDesc1;
            dxgiAdapter1->GetDesc1(&dxgiAdapterDesc1);

            if((dxgiAdapterDesc1.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) != 0)
            {
                continue;
            }

            // LogAdapterDesc(dxgiAdapterDesc1, adapterIndex);

            HRESULT hr = D3D12CreateDevice(dxgiAdapter1.Get(), featureLevel, __uuidof(ID3D12Device), nullptr);
            if(FAILED(hr))
            {
                // spdlog::info("Adapter {} does not support feature level {}. Error: {}",
                //              adapterIndex,
                //              featureLevel,
                //              HRESULT_t(hr));
                continue;
            }

            if(FAILED(dxgiAdapter1.As(&dxgiAdapter4)))
            {
                continue;
            }

            // The adapters are ordered based on dxgiGpuPrefence. The first one should be good.
            break;
        }
    }
    else
    {
        ComPtr<IDXGIAdapter1> dxgiAdapter1;

        SIZE_T selectedDedicatedVideoMemory =
            (devicePreference == GpuDevicePreference::MaximumMemory) ? 0u : eastl::numeric_limits<SIZE_T>::max();

        for(UINT adapterIndex = 0; dxgiFactory4.EnumAdapters1(adapterIndex, &dxgiAdapter1) != DXGI_ERROR_NOT_FOUND;
            ++adapterIndex)
        {
            DXGI_ADAPTER_DESC1 dxgiAdapterDesc1;
            dxgiAdapter1->GetDesc1(&dxgiAdapterDesc1);

            // Check if this is a software adapter. If it is, just skip it.
            if((dxgiAdapterDesc1.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) != 0)
            {
                continue;
            }

            // LogAdapterDesc(dxgiAdapterDesc1, adapterIndex);

            HRESULT hr = D3D12CreateDevice(dxgiAdapter1.Get(), featureLevel, __uuidof(ID3D12Device), nullptr);
            if(FAILED(hr))
            {
                // spdlog::info("Adapter {} does not support feature level {}. Error: {}",
                //              adapterIndex,
                //              featureLevel,
                //              HRESULT_t(hr));
                continue;
            }

            if((devicePreference == GpuDevicePreference::MaximumMemory &&
                dxgiAdapterDesc1.DedicatedVideoMemory < selectedDedicatedVideoMemory) ||
               (devicePreference == GpuDevicePreference::MinimumMemory &&
                dxgiAdapterDesc1.DedicatedVideoMemory > selectedDedicatedVideoMemory))
            {
                continue;
            }

            if(FAILED(dxgiAdapter1.As(&dxgiAdapter4)))
            {
                continue;
            }

            selectedDedicatedVideoMemory = dxgiAdapterDesc1.DedicatedVideoMemory;
        }
    }

    return dxgiAdapter4;
}

eastl::unique_ptr<IDevice> createDevice(GpuDevicePreference devicePreference)
{
    ComPtr<IDXGIFactory4> dxgiFactory4;
    UINT createFactoryFlags{};

    if(const HRESULT hr = CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&dxgiFactory4)); FAILED(hr))
    {
        SDL_LogCritical(SDL_LOG_CATEGORY_GPU, "Failed to create dxgi factory");
        return nullptr;
    }

    constexpr D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_12_0;

    ComPtr<IDXGIAdapter4> hardwareAdapter = getHardwareAdapter(devicePreference, featureLevel, *dxgiFactory4.Get());

    auto ilDevice = eastl::make_unique<d3d12::Device>();
    ilDevice->mAdapter = hardwareAdapter;

    return ilDevice;
}
} // namespace il::gpu