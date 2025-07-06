#pragma once

#include "entityModel.h"

enum class ItemType {
    none,
    heart,
    power
};

class ItemModel : public EntityModel {
public:
    ItemModel(ItemType itemType = ItemType::none) 
        : EntityModel(EntityModelType::item), m_itemType(itemType) {}

    // Setter 方法
    void setItemType(const ItemType& type) { m_itemType = type; }

    // Getter 方法
    ItemType getItemType() const override { return m_itemType; }

private:
    ItemType m_itemType;
};