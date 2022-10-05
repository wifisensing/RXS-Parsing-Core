//
// Created by Zhiping Jiang on 10/4/22.
//

#include "DynamicFieldInterpration.hxx"

std::optional<DynamicContentField> DynamicContentType::queryFieldOffset(const std::string &fieldName) {
    if (quickQueryMap.contains(fieldName)) {
        return quickQueryMap[fieldName];
    }

    if (auto result = std::find_if(fields.cbegin(), fields.cend(), [=](const DynamicContentField &currentField) {
            return currentField.fieldName == fieldName;
        }); result != fields.cend()) {
        quickQueryMap[fieldName] = *result;
        return *result;
    }
    return std::nullopt;
}

std::shared_ptr<DynamicContentTypeDictionary> DynamicContentTypeDictionary::getInstance() {
    static auto instance = std::shared_ptr<DynamicContentTypeDictionary>(new DynamicContentTypeDictionary());
    return instance;
}

std::shared_ptr<DynamicContentType> DynamicContentTypeDictionary::queryType(const std::string &name, uint16_t version) {
    auto queryString = name + std::to_string(version);
    return dictionary.contains(queryString) ? dictionary[queryString] : nullptr;
}

void DynamicContentTypeDictionary::registerType(const std::string &name, uint16_t version, const DynamicContentType &type) {
    auto queryString = name + std::to_string(version);
    dictionary[queryString] = std::make_shared<DynamicContentType>(type);
}

std::optional<DynamicContentField> DynamicFieldInterpreter::queryField(const std::string &fieldName) {
    if (!typeCache) {
        if (typeCache = DynamicContentTypeDictionary::getInstance()->queryType(typeName, version); !typeCache)
            throw std::invalid_argument("Field Type not existent: " + typeName);
    }

    return typeCache->queryFieldOffset(fieldName);
}
