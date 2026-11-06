#pragma once

namespace RE {
    struct ItemCard;
    class InventoryEntryData;
}

namespace PoisonDescriptions {
    class ItemCardPresenter final {
    public:
        static void Update(RE::ItemCard* a_card, const RE::InventoryEntryData* a_entry) noexcept;
    };
}
