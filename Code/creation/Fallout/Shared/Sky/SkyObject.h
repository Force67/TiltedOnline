#pragma once

namespace creation
{
class NiNode;

class SkyObject
{
  public:
    SkyObject();

    virtual ~SkyObject() = default;
    virtual NiNode* GetRootNode() = 0;
    virtual void Initialize(int param) = 0;

  private:
    // actually a nipointer
    NiNode* m_pRootNode;
};
} // namespace creation
