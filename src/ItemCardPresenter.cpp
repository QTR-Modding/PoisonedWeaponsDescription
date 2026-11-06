#include "ItemCardPresenter.h"

#include "DescriptionFormatter.h"
#include "ItemCardRequestContext.h"
#include "PoisonDescription.h"

namespace PoisonDescriptions {
    void ItemCardPresenter::Update(RE::ItemCard* a_card, const RE::InventoryEntryData* a_entry) noexcept {
        if (!ItemCardRequestContext::ShouldAppendPoison() || !a_card || !a_card->view || !a_card->obj.IsObject()) {
            return;
        }

        try {
            const auto poison = PoisonDescription::Build(a_entry);
            if (poison.empty()) {
                return;
            }

            RE::GFxValue effects;
            std::string existing;
            if (a_card->obj.GetMember("effects", &effects) && effects.IsString()) {
                const auto* text = effects.GetString();
                if (text) {
                    existing = text;
                }
            }
            if (existing.empty()) {
                auto* weapon = a_entry->object->As<RE::TESObjectWEAP>();
                RE::BSString description;
                weapon->GetDescription(description, weapon);
                existing = description.c_str();
            }

            const auto description = DescriptionFormatter::AppendPoison(existing, poison);
            RE::GFxValue text;
            a_card->view->CreateString(&text, description.c_str());
            if (!a_card->obj.SetMember("effects", text)) {
                logger::warn("Could not update the poison description on the item card");
            }
        } catch (const std::exception& error) {
            logger::error("Could not build poison description: {}", error.what());
        }
    }
}
