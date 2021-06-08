#pragma once

namespace creation
{
    namespace GameScript
    {
        class QuestCallbackMgr
        {
            static QuestCallbackMgr* Get() noexcept;

            void RegisterQuest(uint32_t formId);
            //void NotifyStage(uint32_t formid, uint16_t stage);
            //void ResetStage(uint32_t formid, uint16_t stage);

            virtual void sub_1();

            uint8_t unk8[0xC0];
        };

        static_assert(sizeof(QuestCallbackMgr) == 0xC8);
    }
}
