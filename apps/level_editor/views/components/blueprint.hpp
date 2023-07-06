//
// Created by Patryk Szczypień on 14/04/2023.
//

#ifndef DUNGEON_CRAWLER_BLUEPRINT_HPP
#define DUNGEON_CRAWLER_BLUEPRINT_HPP
#include <component_renderer.hpp>
#include <core.hpp>
#include <cstdint>
#include <events.hpp>
#include <unordered_map>
#include <typeindex>

enum class FieldComponentType : uint8_t {
    None,
    Floor,
    EncounterChance,
    Walkability
};
static std::unordered_map<FieldComponentType, std::string> field_component_type_to_string = {
        {FieldComponentType::Floor, "Floor"},
        {FieldComponentType::EncounterChance, "EncounterChance"},
        {FieldComponentType::Walkability, "Walkability"}};

struct FieldComponentSelected {
    FieldComponentType type;
};
struct FieldComponentAdded {
    FieldComponentType type;
};
struct FieldComponentRemoved {
    FieldComponentType type;
};

template<typename... T>
struct FieldComponent {
    std::tuple<T...> components;
};

using namespace components::fields;
using namespace components::values;
class Blueprint {
public:
    explicit Blueprint(std::shared_ptr<Core> core) : _core{core}, _component_renderer{core} {
        _core->dispatcher.sink<FieldComponentSelected>().connect<&Blueprint::_field_component_selected>(this);
        _core->dispatcher.sink<FieldComponentAdded>().connect<&Blueprint::_field_component_added>(this);
        _core->dispatcher.sink<FieldComponentRemoved>().connect<&Blueprint::_field_component_removed>(this);

        _initialize();
    };
    Blueprint(const Blueprint &) = delete;
    Blueprint(Blueprint &&) = default;
    Blueprint &operator=(const Blueprint &) = delete;
    Blueprint &operator=(Blueprint &&) = delete;
    ~Blueprint() = default;
    void render();

private:
    ComponentRenderer _component_renderer;
    std::shared_ptr<Core> _core;
    std::vector<FieldComponentType> _all_field_component_types;
    std::vector<FieldComponentType> _available_field_component_types;
    FieldComponentType _selected_field_component_type{FieldComponentType::None};
    void _initialize();
    void _field_component_selected(FieldComponentSelected type);
    void _field_component_added(FieldComponentAdded type);
    void _field_component_removed(FieldComponentRemoved type);
    void _add_component_to_context(FieldComponentType type);
    void _remove_component_from_context(FieldComponentType type);
    FieldComponent<Floor, Walkability, EncounterChance> _field_components;
};
#endif//DUNGEON_CRAWLER_BLUEPRINT_HPP
