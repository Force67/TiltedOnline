#pragma once

#include "BSTEventSource.h"

enum class BSTEventResult
{
    kOk,
    kAbort
};

namespace creation
{
    template<class T>
    class BSTEventSink
    {
        virtual ~BSTEventSink() {}
        virtual BSTEventResult OnEvent(const T* apEvent, const BSTEventSource<T>* apSender)
        {
            return BSTEventResult::kOk;
        }
    };
}
