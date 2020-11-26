//
// Created by 蒋志平 on 2019/12/18.
//

#include "gtest/gtest.h"
#include "../SignalMatrix.hxx"


namespace PicoScenesFrameTest {
    TEST(ExtraInfo, IO) {
        std::vector<int8_t> a;
        for (auto i = 0; i < 24; i++)
            a.emplace_back(i);
        SignalMatrix sm(a, std::vector<int16_t>{4, 3, 2}, SignalMatrixStorageMajority::RowMajor);
        auto vout = sm.toBuffer();
        auto vout2 = sm.toBuffer(SignalMatrixStorageMajority::RowMajor);
        auto vout3 = sm.toBuffer(SignalMatrixStorageMajority::ColumnMajor);
        auto vout4 = sm.toBuffer(SignalMatrixStorageMajority::ColumnMajor);
    }
}