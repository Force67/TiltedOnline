#pragma once

#include <Games/Primitives.h>
#include <NetImmerse/NiNode.h>

class Sky;

class SkyObject
{
public:
    virtual ~SkyObject() = default;

    virtual NiNode* GetRootNode() = 0;
    virtual void Initialize(int param) = 0;
    virtual void Update(Sky* sky, float tick) = 0;

private:
    NiNode* skyNode;
};
