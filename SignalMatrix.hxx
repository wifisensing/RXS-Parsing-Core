//
// Created by csi on 11/25/20.
//

#ifndef PICOSCENES_PLATFORM_SIGNALMATRIX_HXX
#define PICOSCENES_PLATFORM_SIGNALMATRIX_HXX

#include <utility>
#include <vector>
#include <cstdint>
#include <complex>
#include <iterator>
#include <string>
#include <cstring>

enum class SignalMatrixStorageMajority : char {
    RowMajor = 'R',
    ColumnMajor = 'C',
    UndefinedMajority = 'U'
};

template<typename>
struct is_std_complex : std::false_type {
};

template<typename ValueType>
struct is_std_complex<std::complex<ValueType>> : std::true_type {
};

template<typename InputType = std::complex<double>, typename = std::enable_if<std::is_arithmetic_v<InputType> || is_std_complex<InputType>::value>>
class SignalMatrix {
    using ElementType = typename std::conditional<std::is_arithmetic_v<InputType>, InputType, typename std::remove_reference<decltype(std::declval<typename std::conditional<std::is_arithmetic_v<InputType>, std::complex<InputType>, InputType>::type>().real())>::type>::type;
public:

    std::vector<InputType> array;
    std::vector<int32_t> dimensions;
    SignalMatrixStorageMajority major = SignalMatrixStorageMajority::UndefinedMajority;

    SignalMatrix() = default;

    template<typename DimensionContainerType>
    SignalMatrix(const std::vector<InputType> &array, const DimensionContainerType &dimensionsV, SignalMatrixStorageMajority majority) : array(array), major(majority) {
        for (auto it = std::cbegin(dimensionsV); it != std::cend(dimensionsV); it++) {
            dimensions.emplace_back(*it);
        }
        auto sum = 1;
        for (const auto &dimension : dimensions)
            sum *= dimension;

        if (sum != array.size())
            throw std::invalid_argument("SignalMatrix creation failed due to the inconsistent dimensions.");
    }


//    template<typename Iterator>
//    static SignalMatrix<typename std::iterator_traits<Iterator>::value_type> loadFromBuffer(Iterator begin, Iterator end) {
//        using value_type = typename std::iterator_traits<Iterator>::value_type;
//        if (!verifyCompatibility(begin)) {
//            throw std::runtime_error("Incompatible SignalMatrix format");
//        }

//    }

//    template<typename Iterator>
//    static bool verifyCompatibility(Iterator begin) {
//        char fileHeader[3];
//        char fileVersion[3];
//        memset(fileHeader, 0, sizeof(fileHeader));
//        memset(fileVersion, 0, sizeof(fileVersion));
//        std::copy(begin, begin + 2, std::begin(fileHeader));
//        std::copy(begin, begin + 2, std::begin(fileVersion));
//
//        return std::strcmp(fileHeader, "BB") == 0 && std::strcmp(fileVersion, "v1") == 0;
//    }

//    template<typename DumpType = ValueType, typename = std::enable_if<std::is_signed_v<DumpType>>>
//    std::vector<uint8_t> toBuffer() {
//        std::vector<uint8_t> outputArray;
//
//        if constexpr (std::is_same_v<ValueType, DumpType>) {
//            for (const auto &v: array) {
//                std::copy((uint8_t *) &v, (uint8_t *) &v + sizeof(DumpType), std::back_inserter(outputArray));
//            }
//        } else {
//            if constexpr (std::is_floating_point_v<ValueType> && std::is_floating_point_v<DumpType>) {
//                for (const auto &v: array) {
//                    std::complex<DumpType> nv(v.real(), v.imag());
//                    std::copy((uint8_t *) &nv, (uint8_t *) &nv + sizeof(DumpType), std::back_inserter(outputArray));
//                }
//            } else {
//                if constexpr (std::is_floating_point_v<ValueType> && !std::is_floating_point_v<DumpType>) {
//                    auto scaleRatio = std::numeric_limits<DumpType>::max();
//                    for (const auto &v: array) {
//                        std::complex<DumpType> nv(scaleRatio * v.real(), scaleRatio * v.imag());
//                        std::copy((uint8_t *) &nv, (uint8_t *) &nv + sizeof(DumpType), std::back_inserter(outputArray));
//                    }
//                } else {
//                    for (const auto &v: array) {
//                        std::complex<DumpType> nv(1.0 * v.real() / std::numeric_limits<ValueType>::max(), 1.0 * v.imag() / std::numeric_limits<ValueType>::max());
//                        std::copy((uint8_t *) &nv, (uint8_t *) &nv + sizeof(DumpType), std::back_inserter(outputArray));
//                    }
//                }
//            }
//        }
//    }
};


#endif //PICOSCENES_PLATFORM_SIGNALMATRIX_HXX
