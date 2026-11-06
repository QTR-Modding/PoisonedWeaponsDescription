#pragma once

#include <cstdint>
#include <string_view>

namespace RE {
    class GFxMovieView;
    class GFxValue;
}

namespace PoisonDescriptions {
    class ItemCardRequestContext final {
    public:
        explicit ItemCardRequestContext(bool a_listMetadata) noexcept : previous_(listMetadata_) {
            listMetadata_ = a_listMetadata;
        }

        ~ItemCardRequestContext() { listMetadata_ = previous_; }

        ItemCardRequestContext(const ItemCardRequestContext&) = delete;
        ItemCardRequestContext& operator=(const ItemCardRequestContext&) = delete;

        [[nodiscard]] static bool ShouldAppendPoison() noexcept { return !listMetadata_; }

        [[nodiscard]] static bool IsListMetadataCallback(std::string_view a_method, std::string_view a_callback) noexcept {
            return a_method == "RequestItemCardInfo" && a_callback == "updateItemInfo";
        }

        [[nodiscard]] static bool IsListMetadataRequest(RE::GFxMovieView* a_view, const char* a_method, const RE::GFxValue* a_args, std::uint32_t a_argCount) noexcept;

    private:
        static inline thread_local bool listMetadata_{false};
        bool previous_;
    };
}
