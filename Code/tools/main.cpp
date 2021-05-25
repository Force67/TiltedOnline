
#include <Fallout/FileIO/TESFile.h>
#include <Fallout/FileIO/TESFile2.h>

int main(int argc, char** argv)
{
    TESFile2 file(R"(D:\Games\steamapps\common\Fallout 4\Data\Fallout4.esm)");
    TESFile2::Result result = file.Load();
    if (result != TESFile2::Result::kSuccess)
    {
        __debugbreak();
    }

    result = file.LoadMetaData();
    if (result != TESFile2::Result::kSuccess)
    {
        __debugbreak();
    }
 
    // TesClimate::Load
    if (file.FindFormByType(FormEnum::CLMT_ID))
    {
        const FormHeader& formRecord = file.GetCurrentFormHeader();

        // TESClimate::Load
    }
}
