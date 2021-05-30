#pragma once

#include <stdafx.h>

#include "Shared/Events/BSTEventSink.h"
#include "BSSystem/BSTSmartPointer.h"
#include "BSCore/BSTSmallArray.h"
#include "BSCore/BSSpinLock.h"
#include "BShkbAnimationGraph.h"

namespace creation
{
    class BSAnimationGraphManager : public BSTEventSink<BSAnimationGraphEvent>, public BSIntrusiveRefCounted
    {
        virtual ~BSAnimationGraphManager();
        virtual void sub_1(void* apUnk1);

        void Release()
        {
            DecRef();
            if (uiRefCount.load() == 0)
                this->~BSAnimationGraphManager();
        }

        void* pad_ptrs[6];
        BSTSmallArray<BShkbAnimationGraph> animationGraphs;
        uint8_t pad54[0xC8 - 0x58];
        BSSpinLock lock; // C8
        void* unkD0; // D0
        uint32_t animationGraphIndex; // D8

        void DumpAnimationVariables();
    };
}
