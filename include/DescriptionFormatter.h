#pragma once

#include <string>
#include <string_view>

namespace PoisonDescriptions {
    class DescriptionFormatter final {
    public:
        static constexpr std::string_view kHighlightBegin = "\x1E";
        static constexpr std::string_view kHighlightEnd = "\x1F";

        [[nodiscard]] static std::string AppendPoison(std::string_view a_existing, std::string_view a_poison);
    };
}
