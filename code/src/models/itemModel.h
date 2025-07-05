#pragma once

#include "entityModel.h"

enum class ItemType {
    none,
    heart,
    power
};

class ItemModel : public EntityModel {
public:
    ItemModel(ItemType itemType = ItemType::none) : EntityModel(EntityModelType::item), m_itemType(itemType) {}

    void setItemType(const ItemType& type) { m_itemType = type; }

    ItemType getItemType() const override { return m_itemType; }

private:
    ItemType m_itemType;
};