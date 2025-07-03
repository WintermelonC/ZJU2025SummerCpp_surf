#pragma once

#include "entityModel.h"

enum class ItemType {
    heart,
    power
};

class ItemModel : public EntityModel {
public:
    ItemModel() : EntityModel(EntityModelType::item) {}

    const ItemType& getItemType() const { return m_itemType; }

    void setItemType(const ItemType& type) { m_itemType = type; }

private:
    ItemType m_itemType;
};