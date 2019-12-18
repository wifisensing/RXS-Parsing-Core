//
// Created by 蒋志平 on 2019/12/18.
//

#include "gtest/gtest.h"
#include "../VariablePicoScenesFrame.hxx"


namespace PicoScenesFrameTest {
    TEST(ExtraInfo, IO) {
        ExtraInfo txExtraInfo;
        txExtraInfo.setCf(12345);
        txExtraInfo.setAgc(30);
        txExtraInfo.setChansel(123423);
        txExtraInfo.setBmode(0);
        txExtraInfo.setVersion(0x12345678);
        uint8_t buffer[1024];
        memset(buffer, 0, sizeof(buffer));
        auto getLength = txExtraInfo.calculateBufferLength();
        auto length = txExtraInfo.toBuffer(buffer);
        EXPECT_EQ(getLength + 4, length);
        auto recovered = ExtraInfo::fromBuffer(buffer);
        EXPECT_TRUE(recovered);
        return;
    }

    TEST(FrameStructure, IO) {
        PicoScenesTxFrameStructure txFrameStructure;
        txFrameStructure.frameHeader.taskId = 0x1234;
        ExtraInfo txExtraInfo;
//        txExtraInfo.setCf(12345);
//        txExtraInfo.setAgc(30);
//        txExtraInfo.setChansel(123423);
//        txExtraInfo.setBmode(0);
        txExtraInfo.setVersion(0x2323);
        txFrameStructure.addExtraInfo(txExtraInfo);
        uint8_t buffer[1024];
        memset(buffer, 0, sizeof(buffer));
        auto length = txFrameStructure.toBuffer(buffer);

        PicoScenesRxFrameStructure rxFrame;
        auto result = rxFrame.parseRxMACFramePart(buffer);
        EXPECT_TRUE(result);
        return;
    }
}