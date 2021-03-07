#include <TiltedOnlinePCH.h>

#include <Services/OverlayService.h>

#include <OverlayApp.hpp>

#include <D3D11Hook.hpp>
#include <OverlayRenderHandlerD3D11.hpp>

#include <Systems/RenderSystemD3D11.h>

using TiltedPhoques::OverlayRenderHandlerD3D11;
using TiltedPhoques::OverlayRenderHandler;


struct D3D11RenderProvider final : OverlayApp::RenderProvider, OverlayRenderHandlerD3D11::Renderer
{
    explicit D3D11RenderProvider(RenderSystemD3D11* apRenderSystem) : m_pRenderSystem(apRenderSystem) {}

    OverlayRenderHandler* Create() override
    {
        return new OverlayRenderHandlerD3D11(this);
    }

    [[nodiscard]] HWND GetWindow() override
    {
        return m_pRenderSystem->GetWindow();
    }

    [[nodiscard]] IDXGISwapChain* GetSwapChain() const noexcept override
    {
        return m_pRenderSystem->GetSwapChain();
    }

private:

    RenderSystemD3D11* m_pRenderSystem;
};

OverlayService::OverlayService()
{
}

OverlayService::~OverlayService() noexcept
{
}

void OverlayService::Create(RenderSystemD3D11* apRenderSystem)
{
    //m_pOverlay = new OverlayApp(std::make_unique<D3D11RenderProvider>(apRenderSystem));
    //m_pOverlay->Initialize();
    //m_pOverlay->GetClient()->Create();
}

void OverlayService::Render() const
{
    //m_pOverlay->GetClient()->Render();
}

void OverlayService::Reset() const
{
    //m_pOverlay->GetClient()->Reset();
}

using TSimulateTime = void(__int64, __int64);
static TSimulateTime* RealSimulateTime;

// in SP mode we let the client handle its own time simulation
void HookSimulateTime(__int64 apData, __int64 aMultiplier)
{

    void* rsp = _ReturnAddress();
    spdlog::warn("Playmovie: {}, RSP {}\n", *(const char**)(aMultiplier + 8), fmt::ptr(rsp));

    RealSimulateTime(apData, aMultiplier);
}

static TiltedPhoques::Initializer s_init([]() {
    
    #if 1
    TiltedPhoques::Write<BYTE>(0x1405AED24 - 0x140000000, 0xEB);
    TiltedPhoques::Write<BYTE>(0x1405B52BA - 0x140000000, 0xEB);

    TiltedPhoques::Nop(0x1405AF34C - 0x140000000, 5);
    TiltedPhoques::Nop(0x1405AF392 - 0x140000000, 5);
    TiltedPhoques::Nop(0x1405AF384 - 0x140000000, 5);
    TiltedPhoques::Nop(0x1405AEC16 - 0x140000000, 7);

        TiltedPhoques::Write<DWORD>((0x14134B18E) - 0x140000000, 0x90909090);
    TiltedPhoques::Write<BYTE>((0x14134B18E + 4) - 0x140000000, 0x90);

    #else

    //TiltedPhoques::Write<BYTE>(0x1412A0F11 - 0x140000000, 0xCC);
        
    // crt exit remove
    TiltedPhoques::Write<DWORD>((0x1412A0F11) - 0x140000000, 0x90909090);
    TiltedPhoques::Write<BYTE>((0x1412A0F11 + 4) - 0x140000000, 0x90);

    //TiltedPhoques::Nop(0x1412A0F11 - 0x140000000, 5);

    //TiltedPhoques::Write<uint16_t>(0x1412A0EB8 - 0x140000000, 0x90E9);

    POINTER_FALLOUT4(TSimulateTime, s_SimulateTime, 0x142483F20 - 0x140000000);
    RealSimulateTime = s_SimulateTime.Get();
    TP_HOOK(&RealSimulateTime, HookSimulateTime);

    //TiltedPhoques::Write<BYTE>(0x142483F20 - 0x140000000, 0xC3);

    // delete intro movie
    //TiltedPhoques::Write<uint16_t>(0x1412A0301 - 0x140000000, 0x90E9);

    //TiltedPhoques::Nop(0x140BC5AD3 - 0x140000000, 5);
    // 
   // TiltedPhoques::Write<uint16_t>(0x1412A0452 - 0x140000000, 0x90E9);
   // TiltedPhoques::Write<uint16_t>(0x1412A020C - 0x140000000, 0x90E9);
   // TiltedPhoques::Write<uint8_t>(0x1412A0301 - 0x140000000, 0xFF);
   // TiltedPhoques::Write<uint8_t>((0x1412A0301 + 5) - 0x140000000, 0x90);
    //TiltedPhoques::Write<uint16_t>(0x1412A0301 - 0x140000000, 0x90E9);
    #endif
});
