//
// Created by 蒋志平 on 2019/12/18.
//

#include "gtest/gtest.h"
#include "../SignalMatrix.hxx"


namespace PicoScenesFrameTest {
    TEST(SignalMatrix, T1) {
        std::vector<int8_t> a;
        for (auto i = 0; i < 24; i++)
            a.emplace_back(i);
        SignalMatrix sm(a, std::vector<int16_t>{4, 3, 2}, SignalMatrixStorageMajority::ColumnMajor);
        for(auto i = 0 ; i < a.size(); i ++) {
            auto coordinates = sm.getCoordinate4Index(i);
            auto newPos = sm.getIndex4Coordinates(coordinates);
            EXPECT_EQ(i, newPos);
        }
        auto index = sm.getCoordinate4Index(20);
        auto element50 = sm.valueAt({4, 2});
        auto vout = sm.toBuffer(SignalMatrixStorageMajority::ColumnMajor);
        auto signal = SignalMatrix<int8_t>::fromBuffer(vout, SignalMatrixStorageMajority::RowMajor);
        auto reout = signal.toBuffer(SignalMatrixStorageMajority::RowMajor);
        auto reout2 = signal.toBuffer(SignalMatrixStorageMajority::RowMajor);

    }

    TEST(SignalMatrix, T2) {
        std::vector<int8_t> a;
        for (auto i = 0; i < 24; i++)
            a.emplace_back(i);
        SignalMatrix sm(a, std::vector<int16_t>{4, 3, 2}, SignalMatrixStorageMajority::ColumnMajor);
        auto smOut = sm.toBuffer();
        sm >> "test.bbsignals";

        SignalMatrix<int8_t> sm2;
        sm2 << "test1.bbsignals";

        SignalMatrix<int8_t> sm3;
        sm3 << "test1.bbsignals";
    }
}