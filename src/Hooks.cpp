#include "Hooks.h"
#include "ItemCardPresenter.h"
#include "ItemCardRequestContext.h"
#include "Detour.h"

namespace {
    struct ItemCardRequestHook {
        static void thunk(RE::FxDelegate* a_delegate, RE::GFxMovieView* a_view, const char* a_method, const RE::GFxValue* a_args, std::uint32_t a_argCount) {
            const PoisonDescriptions::ItemCardRequestContext context{
                PoisonDescriptions::ItemCardRequestContext::IsListMetadataRequest(a_view, a_method, a_args, a_argCount)};
            originalFunction(a_delegate, a_view, a_method, a_args, a_argCount);
        }

        static inline REL::Relocation<decltype(thunk)> originalFunction;

        static void Install() {
            originalFunction = REL::Relocation<std::uintptr_t>(RE::VTABLE_FxDelegate[0]).write_vfunc(0x01, thunk);
        }
    };

    struct ItemCardSetItemHook {
        static void thunk(RE::ItemCard* a_card, const RE::InventoryEntryData* a_entry, bool a_ignoreStolen) {
            originalFunction(a_card, a_entry, a_ignoreStolen);
            PoisonDescriptions::ItemCardPresenter::Update(a_card, a_entry);
        }

        static inline REL::Relocation<decltype(thunk)> originalFunction;

        static void Install() {
            const REL::Relocation<std::uintptr_t> target{REL::RelocationID(51019, 51897)};
            originalFunction = Detour::write_prologue_hook(target.address(), thunk);
        }
    };
}

void Hooks::Install() {
    ItemCardSetItemHook::Install();
    ItemCardRequestHook::Install();
}
