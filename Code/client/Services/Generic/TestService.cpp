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

#include <Camera/PlayerCamera.h>

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

void TestService::OnDraw() noexcept
{
    const auto view = m_world.view<FormIdComponent>();
    if (view.empty())
        return;

    #if 0
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
    #endif

    #if 0
    ImGui::Begin("Player");

    auto pPlayer = PlayerCharacter::Get();
    if (pPlayer)
    {
        auto pLeftWeapon = pPlayer->GetEquippedWeapon(0);
        auto pRightWeapon = pPlayer->GetEquippedWeapon(1);

        uint32_t leftId = pLeftWeapon ? pLeftWeapon->formID : 0;
        uint32_t rightId = pRightWeapon ? pRightWeapon->formID : 0;

        ImGui::InputScalar("Left Item", ImGuiDataType_U32, (void*)&leftId, nullptr, nullptr, nullptr, ImGuiInputTextFlags_ReadOnly);
        ImGui::InputScalar("Right Item", ImGuiDataType_U32, (void*)&rightId, nullptr, nullptr, nullptr, ImGuiInputTextFlags_ReadOnly);

        leftId = pPlayer->magicItems[0] ? pPlayer->magicItems[0]->formID : 0;
        rightId = pPlayer->magicItems[1] ? pPlayer->magicItems[1]->formID : 0;

        ImGui::InputScalar("Right Magic", ImGuiDataType_U32, (void*)&rightId, nullptr, nullptr, nullptr, ImGuiInputTextFlags_ReadOnly);
        ImGui::InputScalar("Left Magic", ImGuiDataType_U32, (void*)&leftId, nullptr, nullptr, nullptr, ImGuiInputTextFlags_ReadOnly);

#if TP_SKYRIM
        uint32_t shoutId = pPlayer->equippedShout ? pPlayer->equippedShout->formID : 0;

        ImGui::InputScalar("Shout", ImGuiDataType_U32, (void*)&shoutId, nullptr, nullptr, nullptr, ImGuiInputTextFlags_ReadOnly);
#endif  
 
    }

    ImGui::End();
    #endif

    #if 0
    ImGui::Begin("Weather");
    if (ImGui::Button("Clear Sky"))
        Sky::Get()->Reset();

    if (ImGui::Button("None"))
        Sky::Get()->SetSkyMode(Sky::SkyMode::kNone);
    if (ImGui::Button("Interior"))
        Sky::Get()->SetSkyMode(Sky::SkyMode::kInterior);
    if (ImGui::Button("SkydomeOnly"))
        Sky::Get()->SetSkyMode(Sky::SkyMode::kSkydomeOnly);
    if (ImGui::Button("FullSky"))
        Sky::Get()->SetSkyMode(Sky::SkyMode::kFullSky);

    ImGui::End();
    #endif

    if (auto* pPlayer = PlayerCharacter::Get())
    {
        if (auto* pCam = PlayerCamera::Get())
        {
            /*
                // Compute the projection of the input world point to a viewport point
    // (fBx,fBy), with port.L <= fBx <= port.R and port.B <= fBy <= port.T.
    bool WorldPtToScreenPt(const NiPoint3& kPt, float& fBx, float& fBy,
        float fZeroTolerance = 1e-5f) const;

            */

            //https://github.com/expired6978/SKSE64Plugins/blob/master/hudextension/HUDExtension.cpp#L285
            //https://github.com/SlavicPotato/CBPSSE/blob/5cc5a56e075dc77d96163ca1c3dc18df6d9973f3/CBP/CBP/Renderer.cpp

            // ´NO... YOU HAVE TO USE THE GLOBAL STUFF
            // WITH THE INTERNAL FUNCTION U MORON
            // _WorldPtToScreenPt3_Internal(GLOBALMATRIX, GLOBALRECT, ...)

            NiPoint3 screenPos;
            pCam->WorldPtToScreenPt3(pPlayer->position, screenPos);
            {
                auto* pDrawList = ImGui::GetBackgroundDrawList();

                #if 0

                constexpr auto avatarRadius = 13.0f;
                constexpr auto triangleSize = 10.0f;
                constexpr auto triangleColor = 0xFFFFFFF;

                const auto pos = ImGui::GetIO().DisplaySize / 2 + ImVec2{x, y} * 200;
                const auto trianglePos = pos + ImVec2{x, y} * (avatarRadius + 3);

                const ImVec2 trianglePoints[]{trianglePos + ImVec2{0.4f * y, -0.4f * x} * triangleSize,
                                              trianglePos + ImVec2{1.0f * x, 1.0f * y} * triangleSize,
                                              trianglePos + ImVec2{-0.4f * y, 0.4f * x} * triangleSize};
                pDrawList->AddConvexPolyFilled(trianglePoints, 3, triangleColor);
                #endif


                #if 0
                NiPoint3 to;
                pCam->WorldPtToScreenPt3(
                    NiPoint3(pPlayer->position.x + 10.f, pPlayer->position.y + 10.f, pPlayer->position.z + 10.f), to);

                pDrawList->AddLine({screenPos.x, screenPos.y}, {to.x, to.y}, ImColor(0, 230, 64));
                #endif

                float kWidth = 10.f;
                float kHeight = 10.f;

                screenPos.y = (10.f + kWidth) * screenPos.y;
                screenPos.x = (10.f + kHeight) * screenPos.x;
                

                pDrawList->AddCircleFilled({screenPos.x, screenPos.y}, 100.f, ImColor(0, 230, 64));
            }
        }
    
    }
}


