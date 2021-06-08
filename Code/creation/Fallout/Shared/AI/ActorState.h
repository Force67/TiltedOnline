#pragma once

#include "Movement/IMovementState.h"

namespace creation
{
    class ActorState : public IMovementState
    {
    public:
        virtual void sub_21();
        virtual void sub_22();
        virtual void sub_23();
        virtual void sub_24();
        virtual void sub_25();
        virtual void sub_26();

        uint32_t m_uiFlags1;
        uint32_t m_uiFlags2;
    };
}
