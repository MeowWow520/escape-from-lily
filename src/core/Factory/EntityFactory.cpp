//
// Created by MeowWow520 on 2026/5/20.
//

#include "EntityFactory.h"

#include "../Config/ConfigManager.h"
#include "../Logger/Log.h"
#include "../Font/EFL_FontDef.h"


EntityPtr<Player> EntityFactory::createPlayer(const PlayerParams& params) {
    auto player = EntityPtr<Player>(new Player());
    const auto& config = ConfigManager::getInstance().getDefaultJson().player_params;

    // player_name (Default)
    switch (params.player_name.source) {
        case ParamSource::Custom:
            player->setName(params.player_name.value);
            break;
        case ParamSource::Default:
            player->setName(config.player_name);
            break;
        case ParamSource::Asked:
            EFL_LOGGER_ERROR(LogCategory::Factory, "createPlayer: player_name is required but not provided");
            return nullptr;
        case ParamSource::Determined:
            break;
    }

    // texture_path (Asked)
    if (params.texture_path.source == ParamSource::Custom) {
        player->setPath(params.texture_path.value);
    } else {
        EFL_LOGGER_ERROR(LogCategory::Factory, "createPlayer: texture_path is required but not provided");
        return nullptr;
    }

    // world_pos (Determined)
    if (params.world_pos.source == ParamSource::Custom) {
        player->setWorldPos(params.world_pos.value);
    }

    // screen_pos (Determined)
    if (params.screen_pos.source == ParamSource::Custom) {
        player->setScreenPos(params.screen_pos.value);
    }

    // texture_size (Determined)
    if (params.texture_size.source == ParamSource::Custom) {
        player->setTextureSize(params.texture_size.value);
    }

    // scale (Default)
    switch (params.scale.source) {
        case ParamSource::Custom:
            player->setScale(params.scale.value);
            break;
        case ParamSource::Default:
            player->setScale(config.scale);
            break;
        case ParamSource::Asked:
            EFL_LOGGER_ERROR(LogCategory::Factory, "createPlayer: scale is required but not provided");
            return nullptr;
        case ParamSource::Determined:
            break;
    }

    // hitbox (Determined)
    if (params.hitbox.source == ParamSource::Custom) {
        player->setHitbox(params.hitbox.value);
    }

    // pivot (Default)
    switch (params.pivot.source) {
        case ParamSource::Custom:
            player->setPivot(params.pivot.value);
            break;
        case ParamSource::Default:
            player->setPivot(config.pivot);
            break;
        case ParamSource::Asked:
            EFL_LOGGER_ERROR(LogCategory::Factory, "createPlayer: pivot is required but not provided");
            return nullptr;
        case ParamSource::Determined:
            break;
    }

    // max_speed (Default)
    switch (params.max_speed.source) {
        case ParamSource::Custom:
            player->setMaxSpeed(params.max_speed.value);
            break;
        case ParamSource::Default:
            player->setMaxSpeed(config.max_speed);
            break;
        case ParamSource::Asked:
            EFL_LOGGER_ERROR(LogCategory::Factory, "createPlayer: max_speed is required but not provided");
            return nullptr;
        case ParamSource::Determined:
            break;
    }

    // acceleration (Default)
    switch (params.acceleration.source) {
        case ParamSource::Custom:
            player->setAcceleration(params.acceleration.value);
            break;
        case ParamSource::Default:
            player->setAcceleration(config.acceleration);
            break;
        case ParamSource::Asked:
            EFL_LOGGER_ERROR(LogCategory::Factory, "createPlayer: acceleration is required but not provided");
            return nullptr;
        case ParamSource::Determined:
            break;
    }

    // health (Default)
    switch (params.health.source) {
        case ParamSource::Custom:
            player->setHealth(params.health.value);
            break;
        case ParamSource::Default:
            player->setHealth(config.health);
            break;
        case ParamSource::Asked:
            EFL_LOGGER_ERROR(LogCategory::Factory, "createPlayer: health is required but not provided");
            return nullptr;
        case ParamSource::Determined:
            break;
    }

    // visible (Default)
    switch (params.visible.source) {
        case ParamSource::Custom:
            player->setVisible(params.visible.value);
            break;
        case ParamSource::Default:
            player->setVisible(config.visible);
            break;
        case ParamSource::Asked:
            EFL_LOGGER_ERROR(LogCategory::Factory, "createPlayer: visible is required but not provided");
            return nullptr;
        case ParamSource::Determined:
            break;
    }

    // rotation (Default)
    switch (params.rotation.source) {
        case ParamSource::Custom:
            player->setRotation(params.rotation.value);
            break;
        case ParamSource::Default:
            player->setRotation(config.rotation);
            break;
        case ParamSource::Asked:
            EFL_LOGGER_ERROR(LogCategory::Factory, "createPlayer: rotation is required but not provided");
            return nullptr;
        case ParamSource::Determined:
            break;
    }

    // color_mod (Default)
    switch (params.color_mod.source) {
        case ParamSource::Custom:
            player->setColor(params.color_mod.value);
            break;
        case ParamSource::Default:
            player->setColor(config.color);
            break;
        case ParamSource::Asked:
            EFL_LOGGER_ERROR(LogCategory::Factory, "createPlayer: color_mod is required but not provided");
            return nullptr;
        case ParamSource::Determined:
            break;
    }

    // blend_mode (Default)
    switch (params.blend_mode.source) {
        case ParamSource::Custom:
            player->setBlendMode(params.blend_mode.value);
            break;
        case ParamSource::Default:
            player->setBlendMode(stringToBlendMode(config.blend_mode));
            break;
        case ParamSource::Asked:
            EFL_LOGGER_ERROR(LogCategory::Factory, "createPlayer: blend_mode is required but not provided");
            return nullptr;
        case ParamSource::Determined:
            break;
    }

    // rect (Default)
    switch (params.rect.source) {
        case ParamSource::Custom:
            player->setRect(params.rect.value);
            break;
        case ParamSource::Default:
            player->setRect(config.rect);
            break;
        case ParamSource::Asked:
            EFL_LOGGER_ERROR(LogCategory::Factory, "createPlayer: rect is required but not provided");
            return nullptr;
        case ParamSource::Determined:
            break;
    }

    if (player->Initialize() != 0) {
        EFL_LOGGER_ERROR(LogCategory::Factory, "createPlayer: Player::Initialize() failed");
        return nullptr;
    }

    return player;
}

SDL_BlendMode EntityFactory::stringToBlendMode(const std::string& str) {
    if (str == "SDL_BLENDMODE_NONE")  return SDL_BLENDMODE_NONE;
    if (str == "SDL_BLENDMODE_BLEND") return SDL_BLENDMODE_BLEND;
    if (str == "SDL_BLENDMODE_ADD")   return SDL_BLENDMODE_ADD;
    if (str == "SDL_BLENDMODE_MOD")   return SDL_BLENDMODE_MOD;
    if (str == "SDL_BLENDMODE_MUL")   return SDL_BLENDMODE_MUL;
    EFL_LOGGER_ERROR(LogCategory::Factory, "stringToBlendMode: unknown blend mode \"{}\", falling back to SDL_BLENDMODE_NONE", str);
    return SDL_BLENDMODE_NONE;
}

EntityPtr<Camera> EntityFactory::createCamera(const CameraParams& params) {
    auto camera = EntityPtr<Camera>(new Camera());

    // world_pos (Determined)
    if (params.world_pos.source == ParamSource::Custom) {
        camera->setWorldPos(params.world_pos.value);
    }

    // screen_pos (Determined)
    if (params.screen_pos.source == ParamSource::Custom) {
        camera->setScreenPos(params.screen_pos.value);
    }

    // scale (Default)
    if (params.scale.source == ParamSource::Custom) {
        camera->setScale(params.scale.value);
    }

    // pivot (Default)
    if (params.pivot.source == ParamSource::Custom) {
        camera->setPivot(params.pivot.value);
    }

    // max_speed (Default)
    if (params.max_speed.source == ParamSource::Custom) {
        camera->setMaxSpeed(params.max_speed.value);
    }

    // acceleration (Default)
    if (params.acceleration.source == ParamSource::Custom) {
        camera->setAcceleration(params.acceleration.value);
    }

    // visible (Default)
    if (params.visible.source == ParamSource::Custom) {
        camera->setVisible(params.visible.value);
    }

    // rotation (Default)
    if (params.rotation.source == ParamSource::Custom) {
        camera->setRotation(params.rotation.value);
    }

    // color_mod (Default)
    if (params.color_mod.source == ParamSource::Custom) {
        camera->setColor(params.color_mod.value);
    }

    // blend_mode (Default)
    if (params.blend_mode.source == ParamSource::Custom) {
        camera->setBlendMode(params.blend_mode.value);
    } else if (params.blend_mode.source == ParamSource::Default) {
        camera->setBlendMode(stringToBlendMode(
            ConfigManager::getInstance().getDefaultJson().player_params.blend_mode));
    }

    // rect (Default)
    if (params.rect.source == ParamSource::Custom) {
        camera->setRect(params.rect.value);
    }

    // border (Default)
    if (params.border.source == ParamSource::Custom) {
        camera->setBorder(params.border.value);
    }

    if (camera->Initialize() != 0) {
        EFL_LOGGER_ERROR(LogCategory::Factory, "createCamera: Camera::Initialize() failed");
        return nullptr;
    }

    // re-apply visible after Initialize if Custom (Initialize sets it to false)
    if (params.visible.source == ParamSource::Custom) {
        camera->setVisible(params.visible.value);
    }

    return camera;
}

EntityPtr<Background> EntityFactory::createBackground(const BackgroundParams& params) {
    // texture_path (Asked)
    if (params.texture_path.source != ParamSource::Custom) {
        EFL_LOGGER_ERROR(LogCategory::Factory, "createBackground: texture_path is required but not provided");
        return nullptr;
    }

    auto bg = new Background();
    bg->setPath(params.texture_path.value);
    auto background = EntityPtr<Background>(bg);

    // world_pos (Determined)
    if (params.world_pos.source == ParamSource::Custom) {
        background->setWorldPos(params.world_pos.value);
    }

    // screen_pos (Determined)
    if (params.screen_pos.source == ParamSource::Custom) {
        background->setScreenPos(params.screen_pos.value);
    }

    // texture_size (Determined)
    if (params.texture_size.source == ParamSource::Custom) {
        background->setTextureSize(params.texture_size.value);
    }

    // scale (Default)
    if (params.scale.source == ParamSource::Custom) {
        background->setScale(params.scale.value);
    }

    // pivot (Default)
    if (params.pivot.source == ParamSource::Custom) {
        background->setPivot(params.pivot.value);
    }

    // visible (Default)
    if (params.visible.source == ParamSource::Custom) {
        background->setVisible(params.visible.value);
    }

    // rotation (Default)
    if (params.rotation.source == ParamSource::Custom) {
        background->setRotation(params.rotation.value);
    }

    // color_mod (Default)
    if (params.color_mod.source == ParamSource::Custom) {
        background->setColor(params.color_mod.value);
    }

    // blend_mode (Default)
    if (params.blend_mode.source == ParamSource::Custom) {
        background->setBlendMode(params.blend_mode.value);
    } else if (params.blend_mode.source == ParamSource::Default) {
        background->setBlendMode(stringToBlendMode(
            ConfigManager::getInstance().getDefaultJson().player_params.blend_mode));
    }

    // rect (Default)
    if (params.rect.source == ParamSource::Custom) {
        background->setRect(params.rect.value);
    }

    if (background->Initialize() != 0) {
        EFL_LOGGER_ERROR(LogCategory::Factory, "createBackground: Background::Initialize() failed");
        return nullptr;
    }

    return background;
}

EntityPtr<TextStatic> EntityFactory::createTextStatic(const TextStaticParams& params) {
    // text (Asked)
    if (params.text.source != ParamSource::Custom) {
        EFL_LOGGER_ERROR(LogCategory::Factory, "createTextStatic: text is required but not provided");
        return nullptr;
    }

    auto ts = new TextStatic();
    ts->setText(params.text.value);
    auto text_static = EntityPtr<TextStatic>(ts);

    // font_path (Default)
    if (params.font_path.source == ParamSource::Custom) {
        text_static->setTextFontPath(params.font_path.value);
    }

    // font_size (Default)
    if (params.font_size.source == ParamSource::Custom) {
        text_static->setTextFontSize(params.font_size.value);
    } else {
        text_static->setTextFontSize(DEFAULT_FONT_SIZE);
    }

    // color (Default)
    if (params.color.source == ParamSource::Custom) {
        text_static->setTextColor(params.color.value);
    } else {
        text_static->setTextColor(DEFAULT_TEXT_COLOR);
    }

    // rotation (Default)
    if (params.rotation.source == ParamSource::Custom) {
        text_static->setTextRotation(params.rotation.value);
    }

    // pivot (Default)
    if (params.pivot.source == ParamSource::Custom) {
        text_static->setTextPivot(params.pivot.value);
    }

    // scale (Default)
    if (params.scale.source == ParamSource::Custom) {
        text_static->setTextScale(params.scale.value);
    }

    // display_time (Default)
    if (params.display_time.source == ParamSource::Custom) {
        text_static->setTextDisplayTime(params.display_time.value);
    }

    if (text_static->Initialize() != 0) {
        EFL_LOGGER_ERROR(LogCategory::Factory, "createTextStatic: TextStatic::Initialize() failed");
        return nullptr;
    }

    return text_static;
}
