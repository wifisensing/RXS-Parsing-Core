//
// Created by Zhiping Jiang on 10/4/22.
//

#ifndef PICOSCENES_PLATFORM_DYNAMICFIELDINTERPRETATION_HXX
#define PICOSCENES_PLATFORM_DYNAMICFIELDINTERPRETATION_HXX

#include <map>
#include <vector>
#include <memory>
#include <optional>
#include <string>
#include <algorithm>
#include <stdexcept>

class DynamicContentTypeDictionary;

/**
 * @brief Enumeration of primitive field types supported by dynamic content
 */
enum class DynamicContentFieldPrimitiveType : uint8_t {
    Int8 = 0,
    Uint8,
    Int16,
    Uint16,
    Int32,
    Uint32,
    Int64,
    Uint64,
    Single,
    Double
};

/**
 * @brief Structure describing a dynamic content field
 */
struct DynamicContentField {
    std::string fieldName;
    DynamicContentFieldPrimitiveType fieldType;
    int32_t fieldOffset;
    int32_t arraySize{1};

    bool operator==(const DynamicContentField &rhs) const {
        return std::tie(fieldName, fieldType, fieldOffset) == std::tie(rhs.fieldName, rhs.fieldType, rhs.fieldOffset);
    }

    bool operator<(const DynamicContentField &rhs) const {
        return fieldOffset < rhs.fieldOffset;
    }
};

/**
 * @brief Class representing a dynamic content type
 *
 * Contains type information including name, version, and field definitions
 */
class DynamicContentType {
public:
    std::string name{};
    uint16_t version{0};
    std::vector<DynamicContentField> fields;

    /**
     * @brief Constructor for dynamic content type
     * @param name Type name
     * @param version Type version
     * @param fields Vector of field definitions
     */
    DynamicContentType(const std::string &name, uint16_t version, const std::vector<DynamicContentField> &fields);

    /**
     * @brief Query field by name
     * @param fieldName Name of field to find
     * @return Optional containing field if found
     */
    std::optional<DynamicContentField> queryFieldOffset(const std::string &fieldName);

    /**
     * @brief Equality comparison operator
     * @param rhs Right hand side operand
     * @return true if types are equal
     */
    bool operator==(const DynamicContentType &rhs) const {
        return std::tie(name, version) == std::tie(rhs.name, rhs.version);
    }

private:
    friend DynamicContentTypeDictionary;

    /** @brief Default constructor */
    DynamicContentType() = default;

    /** @brief Cache for quick field lookups */
    std::map<std::string, DynamicContentField> quickQueryMap;
};

/**
 * @brief Singleton dictionary for managing dynamic content types
 */
class DynamicContentTypeDictionary {
public:
    /**
     * @brief Get singleton instance
     * @return Shared pointer to dictionary instance
     */
    static std::shared_ptr<DynamicContentTypeDictionary> getInstance();

    /**
     * @brief Register a new type
     * @param type Type to register
     */
    void registerType(const DynamicContentType &type);

    /**
     * @brief Query type by name and version
     * @param name Type name
     * @param version Type version
     * @return Shared pointer to type if found
     */
    std::shared_ptr<DynamicContentType> queryType(const std::string &name, uint16_t version);

private:
    DynamicContentTypeDictionary() = default;

    /** @brief Map storing registered types */
    std::map<std::string, std::shared_ptr<DynamicContentType>> dictionary;
};

/**
 * @brief Class for interpreting dynamic fields in content
 */
class DynamicFieldInterpreter {
public:
    /** @brief Default constructor */
    DynamicFieldInterpreter() = default;

    /**
     * @brief Constructor with type information and content
     * @param typeName Name of content type
     * @param version Version of content type
     * @param content Pointer to content data
     */
    DynamicFieldInterpreter(const std::string &typeName, uint16_t version, const uint8_t *content) : typeName(typeName), version(version), content(content) {}

    /**
     * @brief Query field information
     * @param fieldName Name of field to query
     * @return Optional containing field information if found
     */
    std::optional<DynamicContentField> queryField(const std::string &fieldName);

    /**
     * @brief Const version of query field
     * @param fieldName Name of field to query
     * @return Optional containing field information if found
     */
    std::optional<DynamicContentField> queryField(const std::string &fieldName) const;

    /**
     * @brief Get pointer to field data
     * @tparam OutputType Type to cast field data to
     * @param fieldName Name of field
     * @return Pointer to field data
     * @throw std::invalid_argument if field not found
     */
    template<typename OutputType>
    const OutputType *getFieldPointer(const std::string &fieldName) const {
        if (auto queryResult = queryField(fieldName)) {
            return (OutputType *) (content + queryResult->fieldOffset);
        } else
            throw std::invalid_argument("Field not existent: " + fieldName);
    }

    /**
     * @brief Get field value
     * @tparam OutputType Type of field value
     * @param fieldName Name of field
     * @return Field value
     * @throw std::invalid_argument if field not found
     */
    template<typename OutputType>
    OutputType getField(const std::string &fieldName) const {
        return *getFieldPointer<OutputType>(fieldName);
    }

    /**
     * @brief Get array field values
     * @tparam OutputType Type of array elements
     * @param fieldName Name of field
     * @return Vector containing array values
     * @throw std::invalid_argument if field not found
     */
    template<typename OutputType>
    std::vector<OutputType> getArray(const std::string &fieldName) const {
        if (auto queryResult = queryField(fieldName)) {
            auto numElement = queryResult->arraySize;
            return std::vector<OutputType>((OutputType *) (content + queryResult->fieldOffset), (OutputType *) (content + queryResult->fieldOffset) + numElement);
        } else
            throw std::invalid_argument("Field not existent: " + fieldName);
    }

private:
    std::string typeName;              ///< Name of content type
    uint16_t version{0};              ///< Version of content type
    const uint8_t *content;           ///< Pointer to content data
    std::shared_ptr<DynamicContentType> typeCache{};  ///< Cached type information
};


#endif //PICOSCENES_PLATFORM_DYNAMICFIELDINTERPRETATION_HXX
