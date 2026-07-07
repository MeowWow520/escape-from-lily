//
// Created by MeowWow520 on 2026/5/20.
//

#include "EntityFactory.h"
#include "../Logger/Log.h"



EntityPtr<Camera> EntityFactory::createCamera(const EntityParams &params) {
    // 检查是否符合条件
    if (!std::holds_alternative<CameraParams>(params)) {
        EFL_LOGGER_ERROR(LogCategory::Factory, "Wrong params pass to EntityType::CameraParams");
        return nullptr;
    }
    // 创建实体
    const auto&[world_pos, m_border] = std::get<CameraParams>(params);
    EntityPtr<Camera> bg(new Camera());
    // 设置初始参数
    bg->setWorldPos(world_pos);
    bg->setBorder(m_border);
    EntityPtr<Camera> entity = std::move(bg);
    // 初始化
    if (entity->Initialize() != 0) {
        EFL_LOGGER_ERROR(LogCategory::Factory, "Initialize entity failed");
        return nullptr;
    }
    EFL_LOGGER_INFO(LogCategory::Factory, "Entity initialized: Type = EntityType::Camera");
    return entity;
}

EntityPtr<Background> EntityFactory::createBackground(const EntityParams &params) {
    // 检查是否符合条件
    if (!std::holds_alternative<BackgroundParams>(params)) {
        EFL_LOGGER_ERROR(LogCategory::Factory, "Wrong params pass to EntityType::BackgroundParams");
        return nullptr;
    }
    // 创建实体
    const auto&[texture_path, world_pos] = std::get<BackgroundParams>(params);
    EntityPtr<Background> bg(new Background());
    // 设置初始参数
    bg->setPath(texture_path);
    bg->setWorldPos(world_pos);
    EntityPtr<Background> entity = std::move(bg);
    // 初始化
    if (entity->Initialize() != 0) {
        EFL_LOGGER_ERROR(LogCategory::Factory, "Initialize entity failed");
        return nullptr;
    }
    EFL_LOGGER_INFO(LogCategory::Factory, "Entity initialized: Type = EntityType::Background");
    return entity;
}

EntityPtr<Player> EntityFactory::createPlayer(const EntityParams &params) {
    // 检查是否符合条件
    if (!std::holds_alternative<PlayerParams>(params)) {
        EFL_LOGGER_ERROR(LogCategory::Factory, "Wrong params pass to EntityType::PlayerParams");
        return nullptr;
    }
    // 创建实体
    const auto&[m_player_name, texture_path] = std::get<PlayerParams>(params);
    EntityPtr<Player> bg(new Player());
    // 设置初始参数
    bg->setName(m_player_name);
    bg->setPath(texture_path);
    EntityPtr<Player> entity = std::move(bg);
    // 初始化
    if (entity->Initialize() != 0) {
        EFL_LOGGER_ERROR(LogCategory::Factory, "Initialize entity failed");
        return nullptr;
    }
    EFL_LOGGER_INFO(LogCategory::Factory, "Entity initialized: Type = EntityType::Player");
    return entity;
}

EntityPtr<TextStatic> EntityFactory::createTextStatic(const EntityParams &params) {
    // 检查是否符合条件
    if (!std::holds_alternative<TextStaticParams>(params)) {
        EFL_LOGGER_ERROR(LogCategory::Factory, "Wrong params pass to EntityType::TextStaticParams");
        return nullptr;
    }
    // 创建实体
    const auto&[text, font_size, color, screen_pos, display_time] = std::get<TextStaticParams>(params);
    EntityPtr<TextStatic> bg(new TextStatic());
    // 设置初始参数
    bg->setText(text);
    bg->setTextFontSize(font_size);
    bg->setTextColor(color);
    bg->setScreenPos(screen_pos);
    bg->setTextDisplayTime(display_time);
    EntityPtr<TextStatic> entity = std::move(bg);
    // 初始化
    if (entity->Initialize() != 0) {
        EFL_LOGGER_ERROR(LogCategory::Factory, "Initialize entity failed");
        return nullptr;
    }
    EFL_LOGGER_INFO(LogCategory::Factory, "Entity initialized: Type = EntityType::TextStatic");
    return entity;
}
