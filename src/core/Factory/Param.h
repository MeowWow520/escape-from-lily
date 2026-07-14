//
// Created by MeowWow520 on 2026/7/8.
//

#ifndef ESCAPE_FROM_LILY_PARAM_H
#define ESCAPE_FROM_LILY_PARAM_H

#include <memory>
#include <cstdint>



struct DefaultTag {};
inline constexpr DefaultTag DEFAULT{};

struct AskedTag {};
inline constexpr AskedTag ASKED{};

struct DeterminedTag {};
inline constexpr DeterminedTag DETERMINED{};

enum class ParamSource : uint8_t {
    Default,
    Determined,
    Custom,
    Asked
};

template<typename T>
struct Param {
    ParamSource source = ParamSource::Default;
    T value{};

    Param() = default;

    explicit Param(DefaultTag) {}
    explicit Param(AskedTag)       : source(ParamSource::Asked) {}
    explicit Param(DeterminedTag)  : source(ParamSource::Determined) {}
    Param(const T& v)     : source(ParamSource::Custom), value(v) {}
    Param(T&& v) noexcept : source(ParamSource::Custom), value(std::move(v)) {}
};

#endif // !ESCAPE_FROM_LILY_PARAM_H