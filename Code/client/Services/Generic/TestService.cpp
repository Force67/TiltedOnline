#include <TiltedOnlinePCH.h>

#include <Havok/hkbStateMachine.h>
#include <Structs/AnimationGraphDescriptorManager.h>

#include <Havok/BShkbHkxDB.h>
#include <Havok/hkbBehaviorGraph.h>
#include <Havok/BShkbAnimationGraph.h>

#include <Services/TestService.h>
#include <Services/ImguiService.h>
#include <Services/TransportService.h>

#include <Events/UpdateEvent.h>

#include <Games/References.h>

#include <Forms/TESQuest.h>
#include <BSAnimationGraphManager.h>
#include <Forms/TESFaction.h>

///#include <Camera/PlayerCamera.h>
#include <NetImmerse/NiCamera.h>

#include <Forms/TESNPC.h>
#include <Sky/Sky.h>

#include <Components.h>
#include <World.h>

#include <imgui.h>

extern thread_local bool g_overrideFormId;

void __declspec(noinline) TestService::PlaceActorInWorld() noexcept
{
    const auto pPlayerBaseForm = static_cast<TESNPC*>(PlayerCharacter::Get()->baseForm);

    //const auto pNpc = TESNPC::Create(data, pPlayerBaseForm->GetChangeFlags());
    auto pActor = Actor::Create(pPlayerBaseForm);

    pActor->SetInventory(PlayerCharacter::Get()->GetInventory());

    m_actors.emplace_back(pActor);
}

TestService::TestService(entt::dispatcher& aDispatcher, World& aWorld, TransportService& aTransport, ImguiService& aImguiService)
    : m_dispatcher(aDispatcher)
    , m_transport(aTransport)
    , m_world(aWorld)
{
    m_updateConnection = m_dispatcher.sink<UpdateEvent>().connect<&TestService::OnUpdate>(this);
    m_drawImGuiConnection = aImguiService.OnDraw.connect<&TestService::OnDraw>(this);
}

void TestService::RunDiff()
{
    BSAnimationGraphManager* pManager = nullptr;
    BSAnimationGraphManager* pActorManager = nullptr;

    static Map<uint32_t, uint32_t> s_values;

    if (m_actors.empty())
        return;

    auto pActor = m_actors[0];

    AnimationVariables vars;

    PlayerCharacter::Get()->SaveAnimationVariables(vars);
    pActor->LoadAnimationVariables(vars);

    if (PlayerCharacter::Get()->animationGraphHolder.GetBSAnimationGraph(&pManager) && pActor->animationGraphHolder.GetBSAnimationGraph(&pActorManager))
    {
        if (pManager->animationGraphIndex < pManager->animationGraphs.size)
        {
            const auto pGraph = pManager->animationGraphs.Get(pManager->animationGraphIndex);
            const auto pActorGraph = pActorManager->animationGraphs.Get(pActorManager->animationGraphIndex);
            if (pGraph && pActorGraph)
            {
                const auto pDb = pGraph->hkxDB;
                const auto pBuckets = pDb->animationVariables.buckets;
                const auto pVariableSet = pGraph->behaviorGraph->animationVariables;
                const auto pActorVariableSet = pActorGraph->behaviorGraph->animationVariables;

                auto pDescriptor =
                    AnimationGraphDescriptorManager::Get().GetDescriptor(pGraph->behaviorGraph->stateMachine->name);

                if (pBuckets && pVariableSet && pActorVariableSet)
                {
                    for (auto i = 0u; i < pVariableSet->size; ++i)
                    {
                        if (pVariableSet->data[i] != pActorVariableSet->data[i])
                            spdlog::info("Diff {} expected: {} got: {}", i, pVariableSet->data[i], pActorVariableSet->data[i]);

                        /*auto itor = s_values.find(i);
                        if (itor == std::end(s_values))
                        {
                            s_values[i] = pVariableSet->data[i];

                            if (!pDescriptor->IsSynced(i))
                            {
                                spdlog::info("Variable {} initialized to f: {} i: {}", i, *(float*)&pVariableSet->data[i],
                                             *(int32_t*)&pVariableSet->data[i]);
                            }
                        }
                        else if (itor->second != pVariableSet->data[i] && !pDescriptor->IsSynced(i))
                        {
                            spdlog::info("Variable {} changed to f: {} i: {}", i, *(float*)&pVariableSet->data[i],
                                         *(int32_t*)&pVariableSet->data[i]);

                            itor->second = pVariableSet->data[i];
                        }*/


                    }
                }
            }
        }

        pManager->Release();
    }
}

TestService::~TestService() noexcept = default;

#include <cmath>

void TestService::OnUpdate(const UpdateEvent& acUpdateEvent) noexcept
{
    static std::atomic<bool> s_f8Pressed = false;
    static std::atomic<bool> s_f7Pressed = false;

    RunDiff();

    if (GetAsyncKeyState(VK_F7))
    {
        if (!s_f7Pressed)
        {
            s_f7Pressed = true;

            static char s_address[256] = "127.0.0.1:10578";
            m_transport.Connect(s_address);
        }
    }

    if (GetAsyncKeyState(VK_F8))
    {
        if (!s_f8Pressed)
        {
            s_f8Pressed = true;

            PlaceActorInWorld();
        }
    }
    else
        s_f8Pressed = false;

    if (GetAsyncKeyState(VK_F9))
        ShowCursor(TRUE);
}

static float (*guimatrix)[4][4] = nullptr;
static NiRect<float> *guiport = nullptr;

static TiltedPhoques::Initializer s_Init([]() { 
    POINTER_FALLOUT4(float[4][4], s_matrix, 0x145A66AA0 - 0x140000000);
    POINTER_FALLOUT4(NiRect<float>, s_port, 0x145A66B30 - 0x140000000);

    guimatrix = s_matrix.Get();
    guiport = s_port.Get();
});

#if 0
using TSeek = __int64 (BSFile*, __int64, int);
static TSeek* RealSimulateTime;

static int c = 0;

__int64 HookSeek(BSFile* apData, __int64 aiOffset, int aiWhence)
{
    if (strstr(apData->GetFileName(), "Fallout4.esm"))
    {
        std::printf("Fallout4.esm: Seek to : %lld, (whence: %d), RSP: %p\n", aiOffset, aiWhence, _ReturnAddress());

        if (c > 20)
            __debugbreak();

        c++;
    }

   return RealSimulateTime(apData, aiOffset, aiWhence);
}

static TiltedPhoques::Initializer s_LMAO([]() {
    POINTER_FALLOUT4(TSeek, s_SimulateTime, 0x141B558F0 - 0x140000000);

    RealSimulateTime = s_SimulateTime.Get();
    TP_HOOK(&RealSimulateTime, HookSeek);
});
#endif

#include <DirectXMath.h>


bool HUD_WorldPtToScreenPt3(NiPoint3* in, NiPoint3* out)
{
    return NiCamera::WorldPtToScreenPt3((float*)guimatrix, guiport, in, &out->x, &out->y, &out->z, 1e-5f);
}


void TestService::OnDraw() noexcept
{
    const auto view = m_world.view<FormIdComponent>();
    if (view.empty())
        return;

    ImGui::Begin("Server");

    static char s_address[256] = "127.0.0.1:10578";
    ImGui::InputText("Address", s_address, std::size(s_address));

    if (m_transport.IsOnline())
    {
        if (ImGui::Button("Disconnect"))
            m_transport.Close();
    }
    else
    {
        if (ImGui::Button("Connect"))
        {
            m_transport.Connect(s_address);
        }
    }

    ImGui::End();


    if (PlayerCharacter* pPlayer = PlayerCharacter::Get())
    {
        auto* pDrawList = ImGui::GetBackgroundDrawList();

        auto position = pPlayer->position;

        // test scale up to head.
        position.y -= 200.f;

        NiPoint3 out{};
        HUD_WorldPtToScreenPt3(&pPlayer->position, &out);

        RECT rect{};
        GetWindowRect(GetForegroundWindow(), &rect);

        // transpose to screen
        ImVec2 screenPos = ImVec2{
            ((rect.right - rect.left) * out.x) + rect.left,
            ((rect.bottom - rect.top) * (1.0f - out.y)) + rect.top,
        };

        // IT WORKS!
        ImGui::GetBackgroundDrawList()->AddText(screenPos, ImColor::ImColor(255.f, 0.f, 0.f),
                                                "Forseeee to the moon");
    }
}


