#include "ItemCardRequestContext.h"

#include <iostream>
#include <stdexcept>
#include <thread>

int main() {
    using PoisonDescriptions::ItemCardRequestContext;
    int failures = 0;
    const auto expect = [&failures](bool a_condition, const char* a_case) {
        if (!a_condition) {
            std::cerr << a_case << '\n';
            ++failures;
        }
    };

    expect(ItemCardRequestContext::IsListMetadataCallback("RequestItemCardInfo", "updateItemInfo"), "SkyUI list metadata request must keep native effects");
    expect(!ItemCardRequestContext::IsListMetadataCallback("RequestItemCardInfo", "UpdateItemCardInfo"), "visible card must receive poison text");
    expect(!ItemCardRequestContext::IsListMetadataCallback("OtherRequest", "updateItemInfo"), "unrelated requests must not be classified by callback name alone");
    expect(ItemCardRequestContext::ShouldAppendPoison(), "default context must display poison");
    {
        const ItemCardRequestContext metadata{true};
        expect(!ItemCardRequestContext::ShouldAppendPoison(), "hidden list query must not turn poison text into an enchantment badge");
        {
            const ItemCardRequestContext visible{false};
            expect(ItemCardRequestContext::ShouldAppendPoison(), "nested visible card request must still display poison");
        }
        expect(!ItemCardRequestContext::ShouldAppendPoison(), "nested return must restore list metadata context");
        bool otherThreadDisplaysPoison = false;
        std::thread otherThread([&otherThreadDisplaysPoison] { otherThreadDisplaysPoison = ItemCardRequestContext::ShouldAppendPoison(); });
        otherThread.join();
        expect(otherThreadDisplaysPoison, "metadata context must not leak to another thread");
    }
    expect(ItemCardRequestContext::ShouldAppendPoison(), "callback return must restore ordinary card behavior");
    try {
        const ItemCardRequestContext metadata{true};
        throw std::runtime_error("test callback failure");
    } catch (const std::runtime_error&) {
    }
    expect(ItemCardRequestContext::ShouldAppendPoison(), "callback failure must restore ordinary card behavior");
    return failures ? 1 : 0;
}
