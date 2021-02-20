#include <TiltedOnlinePCH.h>

#include <Systems/InterpolationSystem.h>
#include <Components.h>

#include <Misc/ActorProcessManager.h>
#include <Misc/MiddleProcess.h>

#include <Games/References.h>
#include <World.h>

void InterpolationSystem::Update(Actor* apActor, InterpolationComponent& aInterpolationComponent, const uint64_t aTick) noexcept
{
    auto& movements = aInterpolationComponent.TimePoints;

    if (movements.size() < 2)
        return;

    while (movements.size() > 2)
    {
        const auto second = *(++movements.begin());
        if (aTick > second.Tick)
            movements.pop_front();
        else
            break;
    }

    const auto& first = *(movements.begin());
    const auto& second = *(++movements.begin());

    // Calculate delta movement since last update
    auto delta = 0.0001f;
    const auto tickDelta = static_cast<float>(second.Tick - first.Tick);
    if (tickDelta > 0.f)
    {
        delta = 1.f / tickDelta * static_cast<float>(aTick - first.Tick);
    }

    delta = TiltedPhoques::Min(delta, 1.0f);

    const NiPoint3 position{Lerp(first.Position, second.Position, delta)};

    aInterpolationComponent.Position = position;

    // Don't try to move a null actor
    if (!apActor)
        return;

    apActor->ForcePosition(position);
    apActor->LoadAnimationVariables(second.Variables);

    if (apActor->processManager && apActor->processManager->middleProcess)
    {
        apActor->processManager->middleProcess->direction = second.Direction;
    }

    float firstX, firstY, firstZ;
    float secondX, secondY, secondZ;

    first.Rotation.Decompose(firstX, firstY, firstZ);
    second.Rotation.Decompose(secondX, secondY, secondZ);

    const auto deltaX = TiltedPhoques::DeltaAngle(firstX, secondX, true) * delta;
    const auto deltaY = TiltedPhoques::DeltaAngle(firstY, secondY, true) * delta;
    const auto deltaZ = TiltedPhoques::DeltaAngle(firstZ, secondZ, true) * delta;

#if TP_FALLOUT4
    const auto finalX = 0.f;
#else
    const auto finalX = TiltedPhoques::Mod(firstX + deltaX, float(TiltedPhoques::Pi * 2));
#endif

    const auto finalY = TiltedPhoques::Mod(firstY + deltaY, float(TiltedPhoques::Pi * 2));
    const auto finalZ = TiltedPhoques::Mod(firstZ + deltaZ, float(TiltedPhoques::Pi * 2));

    apActor->SetRotation(finalX, finalY, finalZ);
}

void InterpolationSystem::AddPoint(InterpolationComponent& aInterpolationComponent,
    const InterpolationComponent::TimePoint& acPoint) noexcept
{
    auto itor = std::begin(aInterpolationComponent.TimePoints);
    const auto end = std::cend(aInterpolationComponent.TimePoints);

    while(itor != end)
    {
        if(itor->Tick > acPoint.Tick)
        {
            aInterpolationComponent.TimePoints.insert(itor, acPoint);

            return;
        }

        ++itor;
    }

    aInterpolationComponent.TimePoints.push_back(acPoint);
}

InterpolationComponent& InterpolationSystem::Setup(World& aWorld, const entt::entity aEntity) noexcept
{
    return aWorld.emplace_or_replace<InterpolationComponent>(aEntity);
}

void InterpolationSystem::Clean(World& aWorld, const entt::entity aEntity) noexcept
{
    aWorld.remove_if_exists<InterpolationComponent>(aEntity);
}
