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
#include <numeric>

enum class SignalMatrixStorageMajority : char {
    RowMajor = 'R',
    ColumnMajor = 'C',
    UndefinedMajority = 'U',
};

//std::ostream &operator<<(std::ostream &os, SignalMatrixStorageMajority majority) {
//    switch (majority) {
//        case SignalMatrixStorageMajority::RowMajor :
//            return os << "ARP";
//        case SignalMatrixStorageMajority::ColumnMajor:
//            return os << "IPv4";
//        case SignalMatrixStorageMajority::UndefinedMajority:
//            return os << "VLAN";
//        case SignalMatrixStorageMajority::Default:
//            return os << "IPv6";
//    };
//    return os << static_cast<uint8_t>(majority);
//}

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
    SignalMatrixStorageMajority majority = SignalMatrixStorageMajority::UndefinedMajority;

    SignalMatrix() = default;

    template<typename DimensionContainerType>
    SignalMatrix(const std::vector<InputType> &array, const DimensionContainerType &dimensionsV, SignalMatrixStorageMajority majority = SignalMatrixStorageMajority::UndefinedMajority) : array(array), majority(majority) {
        for (auto it = std::cbegin(dimensionsV); it != std::cend(dimensionsV); it++) {
            dimensions.emplace_back(*it);
        }
        auto sum = 1;
        for (const auto &dimension : dimensions)
            sum *= dimension;

        if (sum != array.size())
            throw std::invalid_argument("SignalMatrix creation failed due to the inconsistent dimensions.");
    }

    std::vector<uint8_t> toBuffer(SignalMatrixStorageMajority outputMajority = SignalMatrixStorageMajority::UndefinedMajority) {
        auto getTypeChar = [] {
            if (std::is_same_v<ElementType, float>) { return 'F'; }
            else if (std::is_same_v<ElementType, double>) { return 'D'; }
            else if (std::is_same_v<ElementType, uint8_t>) { return 'U'; }
            else if (std::is_same_v<ElementType, uint16_t>) { return 'U'; }
            else if (std::is_same_v<ElementType, uint32_t>) { return 'U'; }
            else if (std::is_same_v<ElementType, uint64_t>) { return 'U'; }
            else if (std::is_same_v<ElementType, int8_t>) { return 'I'; }
            else if (std::is_same_v<ElementType, int16_t>) { return 'I'; }
            else if (std::is_same_v<ElementType, int32_t>) { return 'I'; }
            else if (std::is_same_v<ElementType, int64_t>) { return 'I'; }
            else if (std::is_same_v<ElementType, char>) { return 'C'; }
            else if (std::is_same_v<ElementType, bool>) { return 'L'; }
            throw std::runtime_error("BBSignalsFileWriter: unsupported element type.");
        };

        auto computePositionUnderInversedMajority = [&](uint32_t newPos) -> uint32_t {
            auto revD = dimensions;
            std::reverse(revD.begin(), revD.end());
            std::vector<uint32_t> coordinates(revD.size(), 0);
            auto inputPos = newPos;
            for (auto i = 1; i <= revD.size(); i++) {
                auto numElementsPerDimension = std::accumulate(revD.cbegin(), revD.cend() - i, 1, std::multiplies<>());
                coordinates[dimensions.size() - i] = inputPos / numElementsPerDimension;
                inputPos %= numElementsPerDimension;
            }
            auto oldCoordinates = coordinates;
            std::reverse(oldCoordinates.begin(), oldCoordinates.end());

            auto oldPos = 0;
            for (auto i = 0; i < coordinates.size(); i++) {
                auto numElementsPerDimension = std::accumulate(dimensions.cbegin(), dimensions.cbegin() + i, 1, std::multiplies<>());
                oldPos += numElementsPerDimension * oldCoordinates[i];
            }

            return oldPos;
        };

        std::vector<uint8_t> vout;
        vout.reserve(array.size() * sizeof(InputType) + 50);
        std::string header_version("BBv1");
        std::copy(header_version.cbegin(), header_version.cend(), std::back_inserter(vout));
        vout.emplace_back(dimensions.size());
        for (const auto &dimension: dimensions) {
            std::copy((uint8_t *) &dimension, ((uint8_t *) &dimension) + sizeof(dimension), std::back_inserter(vout));
        }

        vout.emplace_back(is_std_complex<InputType>::value ? 'C' : 'R');
        vout.emplace_back(getTypeChar());
        vout.emplace_back(sizeof(ElementType) * 8);

        outputMajority = outputMajority == SignalMatrixStorageMajority::UndefinedMajority ? majority : outputMajority;
        switch (outputMajority) {
            case SignalMatrixStorageMajority::RowMajor:
                vout.emplace_back('R');
                break;
            case SignalMatrixStorageMajority::ColumnMajor:
                vout.emplace_back('C');
                break;
            case SignalMatrixStorageMajority::UndefinedMajority:
                throw std::runtime_error("SignalMatrix::toBuffer failed due to unsupported storage majority.");
        }

        if (majority == outputMajority) {
            for (const auto &value : array) {
                std::copy((uint8_t *) &value, (uint8_t *) &value + sizeof(InputType), std::back_inserter(vout));
            }
        } else {
            for (auto i = 0; i < array.size(); i++) {
                auto pos = computePositionUnderInversedMajority(i);
                std::copy((uint8_t *) &array[pos], ((uint8_t *) &array[pos]) + sizeof(InputType), std::back_inserter(vout));
            }
        }

        return vout;
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
