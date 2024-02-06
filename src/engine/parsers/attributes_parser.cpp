//
// Created by Patryk Szczypień on 31/01/2024.
//
#include <parsers/attributes_parser.hpp>

using namespace attributes_schema;

nlohmann::json AttributesParser::parse_json(const nlohmann::json &json) {
    _validate(json);
    return json;
}

components::battle::Attributes AttributesParser::parse_attributes(const nlohmann::json &json) {
    components::battle::Attributes attributes;
    for (const auto attribute : json.items()) {
        attributes.attributes[types::character::string_to_attribute[attribute.key()]] = attribute.value();
    }
    return attributes;
}

void AttributesParser::_validate(const nlohmann::json &json) {
    //
}
