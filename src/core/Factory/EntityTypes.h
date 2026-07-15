//
// Created by MeowWow520 on 2026/5/20.
//

#ifndef ESCAPE_FROM_LILY_ENTITYTYPES_H
#define ESCAPE_FROM_LILY_ENTITYTYPES_H
#include <memory>
#include <string>
#include <glm/vec2.hpp>

#include "../Object/Object.h"
#include "Param.h"

enum class EntityType : uint8_t {
    Player,
    Camera,
    Background,
    UserInterface,
    TextStatic
    // upcoming
};

struct PlayerParams {
    Param  <std::string> player_name  {DEFAULT};
    Param  <std::string> texture_path {ASKED};
    Param    <glm::vec2> world_pos    {DETERMINED};
    Param    <glm::vec2> screen_pos   {DETERMINED};
    Param    <glm::vec2> texture_size {DETERMINED};
    Param    <glm::vec2> scale        {DEFAULT};
    Param    <glm::vec2> hitbox       {DETERMINED};
    Param    <glm::vec2> pivot        {DEFAULT};
    Param        <float> max_speed    {DEFAULT};
    Param        <float> acceleration {DEFAULT};
    Param        <float> health       {DEFAULT};
    Param         <bool> visible      {DEFAULT};
    Param        <float> rotation     {DEFAULT};
    Param    <SDL_Color> color_mod    {DEFAULT};
    Param<SDL_BlendMode> blend_mode   {DEFAULT};
    Param     <SDL_Rect> rect         {DEFAULT};
};

struct CameraParams {
    Param    <glm::vec2> world_pos    {DETERMINED};
    Param    <glm::vec2> screen_pos   {DETERMINED};
    Param    <glm::vec2> scale        {DEFAULT};
    Param    <glm::vec2> pivot        {DEFAULT};
    Param        <float> max_speed    {DEFAULT};
    Param        <float> acceleration {DEFAULT};
    Param         <bool> visible      {DEFAULT};
    Param        <float> rotation     {DEFAULT};
    Param    <SDL_Color> color_mod    {DEFAULT};
    Param<SDL_BlendMode> blend_mode   {DEFAULT};
    Param     <SDL_Rect> rect         {DEFAULT};
    Param        <float> border       {DEFAULT};
};

struct BackgroundParams {
    Param  <std::string> texture_path {ASKED};
    Param    <glm::vec2> world_pos    {DETERMINED};
    Param    <glm::vec2> screen_pos   {DETERMINED};
    Param    <glm::vec2> texture_size {DETERMINED};
    Param    <glm::vec2> scale        {DEFAULT};
    Param    <glm::vec2> pivot        {DEFAULT};
    Param         <bool> visible      {DEFAULT};
    Param        <float> rotation     {DEFAULT};
    Param    <SDL_Color> color_mod    {DEFAULT};
    Param<SDL_BlendMode> blend_mode   {DEFAULT};
    Param     <SDL_Rect> rect         {DEFAULT};
};

struct TextStaticParams {
    Param  <std::string> text         {ASKED};
    Param  <std::string> font_path    {DEFAULT};
    Param        <float> font_size    {DEFAULT};
    Param    <SDL_Color> color        {DEFAULT};
    Param        <float> rotation     {DEFAULT};
    Param    <glm::vec2> pivot        {DEFAULT};
    Param    <glm::vec2> scale        {DEFAULT};
    Param        <float> display_time {DEFAULT};
};


struct EntityDeleter {
    void operator()(Object* p) const noexcept {
        if (p) {
            p->Quit();
            delete p;
        }
    }
};

template <typename T = Object>
using EntityPtr = std::unique_ptr<T, EntityDeleter>;



#endif //ESCAPE_FROM_LILY_ENTITYTYPES_H
