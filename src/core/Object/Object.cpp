//
// Created by MeowWow520 on 2026/5/19.
//

#include "Object.h"

#include "../Logger/Log.h"


Object::Object(Game &m_game_instance, const char *m_entity_name) noexcept
    : m_game_instance(m_game_instance),
      m_entity_name(m_entity_name)
{
    EFL_LOG_ENTITY_CREATED(m_entity_name);
}

Object::~Object() {
    EFL_LOG_ENTITY_DESTROYED(m_entity_name);
}
