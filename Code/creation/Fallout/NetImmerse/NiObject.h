#pragma once

#include "NetImmerse/NiRTTI.h"
#include "NetImmerse/NiRefObject.h"

namespace creation
{
    class NiNode;
    class NiSwitchNode;
    class BSFadeNode;
    class BSMultiBoundNode;
    class BSGeometry;
    class NiTriStrips;
    class BSTriShape;
    class BSDynamicTriShape;
    class BSSegmentedTriShape;
    class BSSubIndexTriShape;
    class NiGeometry;
    class NiTriBasedGeom;
    class NiTriShape;
    class NiParticles;
    class NiParticleSystem;
    class BSLines;
    class NiLight;
    struct bhkNiCollisionObject;
    struct bhkBlendCollisionObject;
    struct bhkRigidBody;
    struct bhkLimitedHingeConstraint;
    struct bhkNPCollisionObject;

    class NiCloningProcess;
    class NiStream;
    class NiObjectGroup;
    class NiControllerManager;

    class NiObject : public NiRefObject
    {
    public:
        NiObject() = default;
        virtual ~NiObject() = default;

        virtual NiRTTI* GetRTTI() const
        {
            return &m_sRTTI;
        }

        // casting prototypes:
        virtual NiNode* IsNode()
        {
            return nullptr;
        }
        virtual NiNode* IsNode2()
        {
            return nullptr;
        }
        virtual NiSwitchNode* IsSwitchNode()
        {
            return nullptr;
        }
        virtual BSFadeNode* IsFadeNode()
        {
            return nullptr;
        }
        virtual BSMultiBoundNode* IsMultiBoundNode()
        {
            return nullptr;
        }
        virtual BSGeometry* IsGeometry()
        {
            return nullptr;
        }
        virtual NiTriStrips* IsTriStrips()
        {
            return nullptr;
        }
        virtual BSTriShape* IsTriShape()
        {
            return nullptr;
        }
        virtual BSDynamicTriShape* IsDynamicTriShape()
        {
            return nullptr;
        }
        virtual BSSegmentedTriShape* IsIsSegmentedTriShape()
        {
            return nullptr;
        }
        virtual BSSubIndexTriShape* IsSubIndexTriShape()
        {
            return nullptr;
        }
        virtual NiGeometry* IsNiGeometry()
        {
            return nullptr;
        }
        virtual NiTriBasedGeom* IsNiTriBasedGeom()
        {
            return nullptr;
        }
        virtual NiTriShape* IsNiTriShape()
        {
            return nullptr;
        }
        virtual NiParticles* IsNiParticles()
        {
            return nullptr;
        }
        virtual NiParticleSystem* IsNiParticleSystem()
        {
            return nullptr;
        }
        virtual BSLines* IsLinesGeom()
        {
            return nullptr;
        }
        virtual NiLight* IsLight()
        {
            return nullptr;
        }
        virtual bhkNiCollisionObject* IsBhkNiCollisionObject()
        {
            return nullptr;
        }
        virtual bhkBlendCollisionObject* IsBhkBlendCollisionObject()
        {
            return nullptr;
        }
        virtual bhkRigidBody* IsBhkRigidBody()
        {
            return nullptr;
        }
        virtual bhkLimitedHingeConstraint* IsBhkLimitedHingeConstraint()
        {
            return nullptr;
        }
        virtual bhkNPCollisionObject* IsbhkNPCollisionObject()
        {
            return nullptr;
        }

        // Duplicate current object
        virtual NiObject* CreateClone(NiCloningProcess& aCloningProcess)
        {
            return this;
        }

        virtual void LoadBinary(NiStream&) {};
        virtual void LinkObject(NiStream&) {};
        virtual bool RegisterStreamables(NiStream&); // TODO: default impl!
        virtual void SaveBinary(NiStream&) {};
        virtual bool IsEqual(NiObject* apRhs);
        virtual void ProcessClone(NiCloningProcess* apCloningProcess); // TODO: proper impl
        virtual void PostLinkObject(NiStream&) {};
        virtual bool StreamCanSkip()
        {
            return nullptr;
        }
        virtual const NiRTTI* GetStreamableRTTI();
        virtual uint32_t GetBlockAllocationSize()
        {
            return 0;
        }
        virtual NiObjectGroup* GetGroup()
        {
            return nullptr;
        }
        virtual void SetGroup(NiObjectGroup*) {};
        virtual NiControllerManager* IsNiControllerManager()
        {
            return nullptr;
        }

    private:
        // disable copy
        NiObject(const NiObject& kObject) = delete;
        NiObject& operator=(const NiObject& kObject) = delete;

    private:
        static NiRTTI m_sRTTI;
    };

    static_assert(sizeof(NiObject) == 0x10);
} // namespace creation
