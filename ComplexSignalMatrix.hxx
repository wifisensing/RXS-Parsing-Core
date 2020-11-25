//
// Created by csi on 11/25/20.
//

#ifndef PICOSCENES_PLATFORM_COMPLEXSIGNALMATRIX_HXX
#define PICOSCENES_PLATFORM_COMPLEXSIGNALMATRIX_HXX

#include <vector>
#include <cstdint>
#include <complex>


template<typename ValueType, typename = std::enable_if<std::is_signed_v<ValueType>>>
class ComplexSignalMatrix {
public:
    std::vector<std::complex<ValueType>> array;
    std::vector<int32_t> dimensions;

    template<typename DumpType = ValueType, typename = std::enable_if<std::is_signed_v<DumpType>>>
    std::vector<uint8_t> toBuffer() {
        std::vector<uint8_t> outputArray;

        if constexpr (std::is_same_v<ValueType, DumpType>) {
            for (const auto &v: array) {
                std::copy((uint8_t *) &v, (uint8_t *) &v + sizeof(DumpType), std::back_inserter(outputArray));
            }
        } else {
            if constexpr (std::is_floating_point_v<ValueType> && std::is_floating_point_v<DumpType>) {
                for (const auto &v: array) {
                    std::complex<DumpType> nv(v.real(), v.imag());
                    std::copy((uint8_t *) &nv, (uint8_t *) &nv + sizeof(DumpType), std::back_inserter(outputArray));
                }
            } else {
                if constexpr (std::is_floating_point_v<ValueType> && !std::is_floating_point_v<DumpType>) {
                    auto scaleRatio = std::numeric_limits<DumpType>::max();
                    for (const auto &v: array) {
                        std::complex<DumpType> nv(scaleRatio * v.real(), scaleRatio * v.imag());
                        std::copy((uint8_t *) &nv, (uint8_t *) &nv + sizeof(DumpType), std::back_inserter(outputArray));
                    }
                } else {
                    for (const auto &v: array) {
                        std::complex<DumpType> nv(1.0 * v.real() / std::numeric_limits<ValueType>::max(), 1.0 * v.imag() / std::numeric_limits<ValueType>::max());
                        std::copy((uint8_t *) &nv, (uint8_t *) &nv + sizeof(DumpType), std::back_inserter(outputArray));
                    }
                }
            }
        }
    }
};


#endif //PICOSCENES_PLATFORM_COMPLEXSIGNALMATRIX_HXX
