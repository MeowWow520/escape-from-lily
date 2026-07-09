//
// Created by MeowWow520 on 2026/5/20.
//

#include "EntityFactory.h"
#include "../Logger/Log.h"



EntityPtr<Camera> EntityFactory::createCamera(const CameraParams &params) {
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

EntityPtr<Background> EntityFactory::createBackground(const BackgroundParams &params) {
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

EntityPtr<Player> EntityFactory::createPlayer(const PlayerParams& params) {
    // 创建实体
    EntityPtr<Player> entity(new Player());

    if (params.player_name.source == ParamSource::Custom)
        entity->setName(params.player_name.value);
    if (params.texture_path.source == ParamSource::Custom)
        entity->setPath(params.texture_path.value);
    else if (params.texture_path.source == ParamSource::Asked)
        EFL_LOGGER_ERROR(LogCategory::Factory, "Created Player warning: must set a texture path");
    if (params.max_speed.source == ParamSource::Custom)
        entity->setMaxSpeed(params.max_speed.value);
    else if (params.max_speed.source == ParamSource::Default)
        entity->setMaxSpeed(m_configManager.getDefaultJson().max_speed.value);


    // 初始化
    if (entity->Initialize() != 0) {
        EFL_LOGGER_ERROR(LogCategory::Factory, "Initialize entity failed");
        return nullptr;
    }
    EFL_LOGGER_INFO(LogCategory::Factory, "Entity initialized: Type = EntityType::Player");
    return entity;
}

EntityPtr<TextStatic> EntityFactory::createTextStatic(const TextStaticParams &params) {
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
