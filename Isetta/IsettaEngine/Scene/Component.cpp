/*
 * Copyright (c) 2018 Isetta
 */
#include "Scene/Component.h"
#include "Scene/Entity.h"
#include "Scene/Transform.h"

namespace Isetta {

bool Component::isFlattened = false;

bool Component::RegisterComponent(std::type_index curr, std::type_index base) {
  std::type_index baseIndex{base};
  std::type_index currIndex{curr};

  std::unordered_map<std::type_index, std::list<std::type_index>>& children =
      childrenTypes();

  if (children.count(currIndex) > 0) {
    children.at(currIndex).push_front(currIndex);
  } else {
    children.insert({currIndex, std::list<std::type_index>{currIndex}});
  }

  if (children.count(baseIndex) > 0) {
    children.at(baseIndex).emplace_back(currIndex);
  } else {
    children.insert({baseIndex, std::list<std::type_index>{currIndex}});
  }
  return true;
}

void Component::FlattenComponentList() {
  if (isFlattened) return;
  isFlattened = true;
  std::unordered_map<std::type_index, std::list<std::type_index>>& children =
      childrenTypes();
  std::type_index componentIndex{typeid(Component)};
  std::list<std::type_index>* componentList = &children.at(componentIndex);
  for (auto& childList : *componentList) {
    if (childList != componentIndex) {
      FlattenHelper(componentIndex, childList);
    }
  }
}

void Component::FlattenHelper(std::type_index parent, std::type_index curr) {
  std::unordered_map<std::type_index, std::list<std::type_index>>& children =
      childrenTypes();
  std::list<std::type_index>* parentList = &children.at(parent);
  std::list<std::type_index>* componentList = &children.at(curr);
  for (auto& childList : *componentList) {
    if (childList != curr) {
      parentList->push_back(childList);
      FlattenHelper(curr, childList);
    }
  }
}

Component::Component() : attributes{0b1001}, entity{nullptr} {
  if (!isFlattened) {
    FlattenComponentList();
  }
}

void Component::SetAttribute(ComponentAttributes attr, bool value) {
  attributes.set(static_cast<int>(attr), value);
}

bool Component::GetAttribute(ComponentAttributes attr) const {
  return attributes.test(static_cast<int>(attr));
}

void Component::SetActive(bool value) {
  bool isActive = GetAttribute(ComponentAttributes::IS_ACTIVE);
  SetAttribute(ComponentAttributes::IS_ACTIVE, value);
  if (!isActive && value) {
    OnEnable();
  } else if (isActive && !value) {
    OnDisable();
  }
}

bool Component::GetActive() const {
  return GetAttribute(ComponentAttributes::IS_ACTIVE);
}

Transform& Component::GetTransform() const { return entity->GetTransform(); }

Entity* Component::GetEntity() const { return entity; }
}  // namespace Isetta
