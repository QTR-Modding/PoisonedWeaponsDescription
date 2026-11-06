#include "PoisonDescription.h"
#include "DescriptionFormatter.h"

#include "RE/A/AlchemyItem.h"
#include "RE/B/BSString.h"
#include "RE/E/Effect.h"
#include "RE/E/EffectSetting.h"
#include "RE/E/ExtraPoison.h"
#include "RE/I/InventoryEntryData.h"
#include "RE/M/MagicSystem.h"

namespace {
    [[nodiscard]] RE::AlchemyItem* FindAppliedPoison(const RE::InventoryEntryData* a_entry) {
        if (!a_entry || !a_entry->object || !a_entry->object->IsWeapon() || !a_entry->extraLists) {
            return nullptr;
        }

        for (const auto* extraList : *a_entry->extraLists) {
            if (!extraList) {
                continue;
            }

            const auto* extraPoison = extraList->GetByType<RE::ExtraPoison>();
            if (extraPoison && extraPoison->poison && extraPoison->count > 0) {
                return extraPoison->poison;
            }
        }

        return nullptr;
    }

    [[nodiscard]] std::string DescribeWithoutEffectText(const RE::AlchemyItem& a_poison) {
        std::string result;
        for (const auto* effect : a_poison.effects) {
            if (!effect || !effect->baseEffect) {
                continue;
            }

            const auto* name = effect->baseEffect->GetName();
            if (!name || !*name) {
                continue;
            }

            if (!result.empty()) {
                result += "; ";
            }
            result += name;
        }

        if (result.empty()) {
            const auto* name = a_poison.GetName();
            result = name && *name ? name : "Poison";
        }
        return result;
    }
}

std::string PoisonDescription::Build(const RE::InventoryEntryData* a_entry) {
    auto* poison = FindAppliedPoison(a_entry);
    if (!poison) {
        return {};
    }

    RE::BSString description;
    RE::MagicSystem::GetMagicItemDescription(description, poison, PoisonDescriptions::DescriptionFormatter::kHighlightBegin.data(),
                                           PoisonDescriptions::DescriptionFormatter::kHighlightEnd.data());
    if (!description.empty()) {
        return std::string(description.c_str(), description.size());
    }

    return DescribeWithoutEffectText(*poison);
}
