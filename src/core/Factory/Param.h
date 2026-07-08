//
// Created by MeowWow520 on 2026/7/8.
//

#ifndef ESCAPE_FROM_LILY_PARAM_H
#define ESCAPE_FROM_LILY_PARAM_H

#include <memory>
#include <cstdint>



struct DefaultTag {};
inline constexpr DefaultTag DEFAULT{};

struct TextureTag {};
inline constexpr TextureTag Determined{};

enum class ParamSource : uint8_t { Default, FromTexture, Custom };

template<typename T>
struct Param {
    ParamSource source = ParamSource::Default;
    T value{};

    Param() = default;

    explicit Param(DefaultTag) : source(ParamSource::Default) {}
    explicit Param(TextureTag) : source(ParamSource::FromTexture) {}
    explicit Param(const T& v) : source(ParamSource::Custom), value(v) {}
    explicit Param(T&& v) noexcept : source(ParamSource::Custom), value(std::move(v)) {}
};

#endif // !ESCAPE_FROM_LILY_PARAM_H