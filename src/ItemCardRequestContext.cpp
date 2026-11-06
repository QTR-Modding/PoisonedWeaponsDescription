#include "ItemCardRequestContext.h"

#include <array>
#include <charconv>
#include <cmath>

namespace PoisonDescriptions {
    bool ItemCardRequestContext::IsListMetadataRequest(RE::GFxMovieView* a_view, const char* a_method, const RE::GFxValue* a_args, std::uint32_t a_argCount) noexcept {
        if (!a_view || !a_method || std::string_view{a_method} != "RequestItemCardInfo" || !a_args || !a_argCount || !a_args[0].IsNumber()) {
            return false;
        }

        try {
            const auto responseID = a_args[0].GetNumber();
            constexpr double maxExactInteger = 9007199254740991.0;
            if (!std::isfinite(responseID) || responseID < 0 || responseID > maxExactInteger || std::trunc(responseID) != responseID) {
                return false;
            }
            std::array<char, 32> key{};
            const auto converted = std::to_chars(key.data(), key.data() + key.size() - 1, static_cast<std::uint64_t>(responseID));
            if (converted.ec != std::errc{}) {
                return false;
            }

            RE::GFxValue responses;
            RE::GFxValue response;
            RE::GFxValue callback;
            if (!a_view->GetVariable(&responses, "_global.gfx.io.GameDelegate.responseHash") || !responses.IsObject() ||
                !responses.GetMember(key.data(), &response) || !response.IsArray() || !response.GetElement(1, &callback) || !callback.IsString()) {
                return false;
            }
            const auto* callbackName = callback.GetString();
            return callbackName && IsListMetadataCallback(a_method, callbackName);
        } catch (const std::exception& error) {
            logger::warn("Could not identify item-card metadata request: {}", error.what());
            return false;
        }
    }
}
