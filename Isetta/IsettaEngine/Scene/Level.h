/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include <list>
#include <set>
#include <stack>
#include <string>
#include "ISETTA_API.h"

#define CREATE_LEVEL(NAME)                                         \
  class NAME : public Level, public LevelRegistry<NAME> {          \
   public:                                                         \
    bool IsRegisteredInLevelManager() const { return registered; } \
    static inline Func<NAME*> CreateMethod = []() {                \
      return MemoryManager::NewOnStack<NAME>();                    \
    };                                                             \
    static std::string GetLevelName() { return #NAME; }            \
                                                                   \
   private:
#define CREATE_LEVEL_END \
  }                      \
  ;

namespace Isetta {
class ISETTA_API Level {
  std::list<class Entity*> entitiesToRemove;
  void AddComponentToStart(class Component* component);
  void StartComponents();

 protected:
  std::list<class Entity*> entities;
  std::stack<class Component*> componentsToStart;
  std::set<class Component*> componentsToDestroy;

 public:
  class Entity* levelRoot;
  Level();
  virtual ~Level() = default;
  class Entity* GetEntityByName(const std::string&);
  class std::list<class Entity*> GetEntitiesByName(const std::string&);

  virtual void LoadLevel() {}
  virtual void UnloadLevel();

  class Entity* AddEntity(std::string name);
  class Entity* AddEntity(std::string name, class Entity* parent);

  void Update();
  void FixedUpdate();
  void GUIUpdate();
  void LateUpdate();

  friend class Entity;
};
}  // namespace Isetta
