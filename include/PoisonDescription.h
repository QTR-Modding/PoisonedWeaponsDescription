#pragma once

#include <string>

namespace RE {
    class InventoryEntryData;
}

class PoisonDescription final {
public:
    [[nodiscard]] static std::string Build(const RE::InventoryEntryData* a_entry);
};
