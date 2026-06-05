//
// Created by MeowWow520 on 2026/6/5.
//

#ifndef ESCAPE_FROM_LILY_BASEJSON_H
#define ESCAPE_FROM_LILY_BASEJSON_H
#include <variant>
#include "DefaultStruct.h"
#include "PlayerStruct.h"



using JsonStruct = std::variant<
    std::monostate,
    DefaultJson,
    PlayerJson
>;

#endif //ESCAPE_FROM_LILY_BASEJSON_H
