//
// Created by 蒋志平 on 2019/12/18.
//

#include "gtest/gtest.h"
#include "../SignalMatrix.hxx"


namespace PicoScenesFrameTest {
    TEST(ExtraInfo, IO) {
        std::vector<int8_t> a = {1, 2, 3, 45, 6, 7, 8, 9};
        std::string ab("BBv1");
        SignalMatrix<double>::loadFromBuffer(&a[0], &a[4]);
    }
}