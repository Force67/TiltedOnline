
#include "TiltedOnlineApp.h"

struct ComInitializer
{
    ComInitializer()
    {
        HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
        TP_UNUSED(hr);
    }
    ~ComInitializer()
    {
        CoUninitialize();
    }
};

int main(int argc, char** argv)
{
    ComInitializer comInit;
    TP_UNUSED(comInit);

    auto appInstance = TiltedPhoques::MakeUnique<TiltedOnlineApp>(argc, argv);
    if (!appInstance->Initialize())
    {
        return -1;
    }

    return appInstance->Exec();
}
