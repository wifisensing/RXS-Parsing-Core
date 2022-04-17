//
// Created by 蒋志平 on 2020/11/5.
//

#ifndef PICOSCENES_PLATFORM_ABSTRACTPICOSCENESFRAMESEGMENT_HXX
#define PICOSCENES_PLATFORM_ABSTRACTPICOSCENESFRAMESEGMENT_HXX

#include <algorithm>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <type_traits>
#include <tuple>
#include <memory>
#include <cstring>
/**
 * @brief PicoScenes Frame Segment is a typed, versioned and named data structure used to carrier various of data from the Tx end, PicoScenes driver or SDR baseband to the PicoScenes Platform.
 * @details AbstractPicoScenesFrameSegment is the root class for various of Frame segment classes.
 */
class AbstractPicoScenesFrameSegment {
public:
    std::string segmentName; ///< The name of the segment
    uint16_t segmentVersionId; ///< The version of the segment
    uint32_t segmentLength = 0; ///< Length of the entire segment data, excluding the 4-byte length itself.
    std::vector<uint8_t> rawBuffer; ///< The raw buffer of the segment

    AbstractPicoScenesFrameSegment(std::string segmentName, uint16_t segmentVersionId);

    template<typename ValueType, typename = std::enable_if<std::is_fundamental_v<ValueType>>>
    void addField(const std::string &fieldName, ValueType value) {
        addField(fieldName, (uint8_t *) &value, sizeof(value));
    }

    void addField(const std::string &fieldName, const std::vector<uint8_t> &data);

    void addField(const std::string &fieldName, const uint8_t *buffer, uint32_t bufferLength);

    void addField(const std::string &fieldName, const std::pair<const uint8_t *, uint32_t> &buffer);

    void addField(const std::string &fieldName, const std::pair<std::shared_ptr<uint8_t>, uint32_t> &buffer);

    std::vector<uint8_t> getField(const std::string &fieldName) const;

    uint32_t getField(const std::string &fieldName, uint8_t *buffer, std::optional<uint32_t> capacity) const;

    void removeField(const std::string &fieldName);

    /**
     * @brief Return the total length of the segment buffer
     * 
     * @return uint32_t 
     */
    uint32_t totalLength() const;

    /**
     * @brief ModularPicoScenesTxFrame and ModularPicoScenesRxFrame call this method to get the whole data of this segment
     * @details By default, this method performs 3 things:
     * 1. call totalLength method to obtain the length of the raw buffer;
     * 2. pre-allocate a std::vector<uint8_t> buffer;
     * 3. fill the buffer with the other AbstractPicoScenesFrameSegment#toBuffer method
     * 
     * @param totalLengthIncluded where the returned buffer includes the leading 4-byte segment length field
     */
    virtual std::vector<uint8_t> toBuffer() const = 0;

    virtual std::vector<uint8_t> toBuffer(bool totalLengthIncluded) const;

    virtual uint32_t toBuffer(bool totalLengthIncluded, uint8_t *buffer, std::optional<uint32_t> capacity = std::nullopt) const;

    virtual void fromBuffer(const uint8_t *buffer, uint32_t bufferLength) = 0;

    void clearFieldCache();

    bool isSuccessfullyDecoded() const;

    static std::tuple<std::string, uint32_t, uint16_t, uint32_t> extractSegmentMetaData(const uint8_t *buffer, uint32_t bufferLength);

    /**
     * @brief Return a short description for this segment
     */
    virtual std::string toString() const;

    virtual ~AbstractPicoScenesFrameSegment() {};

protected:
    std::vector<std::string> fieldNames; // fieldMap is not ordered by the insertion, so we need another list to hold the input order.
    std::map<std::string, std::vector<uint8_t>> fieldMap;
    std::map<std::string, std::pair<uint32_t, uint32_t>> fieldIndices;
    bool successfullyDecoded = false;
};


#endif //PICOSCENES_PLATFORM_ABSTRACTPICOSCENESFRAMESEGMENT_HXX
