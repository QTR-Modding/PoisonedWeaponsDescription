#include "DescriptionFormatter.h"

namespace PoisonDescriptions {
    std::string DescriptionFormatter::AppendPoison(std::string_view a_existing, std::string_view a_poison) {
        std::string description{a_existing};
        if (a_poison.empty()) {
            return description;
        }
        if (!description.empty()) {
            description += "<br>";
        }
        description += "<font color='#4EDB43'>";
        bool highlighted = false;
        for (const auto character : a_poison) {
            if (character == kHighlightBegin.front()) {
                if (!highlighted) {
                    description += "</font><font color='#A3CFA0'>";
                    highlighted = true;
                }
                continue;
            }
            if (character == kHighlightEnd.front()) {
                if (highlighted) {
                    description += "</font><font color='#4EDB43'>";
                    highlighted = false;
                }
                continue;
            }
            switch (character) {
                case '&': description += "&amp;"; break;
                case '<': description += "&lt;"; break;
                case '>': description += "&gt;"; break;
                case '\n': description += "<br>"; break;
                case '\r': break;
                default: description += character; break;
            }
        }
        description += "</font>";
        return description;
    }
}
