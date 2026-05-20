//
// Created by MeowWow520 on 2026/5/20.
//

#ifndef ESCAPE_FROM_LILY_ENTITYFACTORY_H
#define ESCAPE_FROM_LILY_ENTITYFACTORY_H
#include "EntityTypes.h"


class EntityFactory {
    public:
        EntityFactory() = default;
        ~EntityFactory() = default;

        EntityFactory(const EntityFactory&) = default;
        EntityFactory& operator=(const EntityFactory&) = default;

        /**
         * 创建实体，为其分配内存设置属性，返回 EntityPtr
         *
         * @param type 实体的类型，见 EntityTypes.h
         * @param params 实体的初始参数
         * @return 创建成功返回 unique_ptr, 失败返回 nullptr
         */
        [[nodiscard]] EntityPtr<Object> Create(EntityType type, const EntityParams& params) const;
};


#endif //ESCAPE_FROM_LILY_ENTITYFACTORY_H
