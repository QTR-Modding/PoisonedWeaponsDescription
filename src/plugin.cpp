#include "Hooks.h"
#include "logger.h"

SKSEPluginLoad(const SKSE::LoadInterface* skse) {
    SKSE::Init(skse);
    SetupLog();
    try {
        Hooks::Install();
    } catch (const std::exception& error) {
        logger::critical("Cannot install poison description hook: {}", error.what());
        return false;
    }
    logger::info("PoisonDescription loaded");
    return true;
}
