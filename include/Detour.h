#pragma once

#include <Windows.h>
#include <detours/detours.h>

#include <cstdint>
#include <stdexcept>
#include <string>

namespace Detour {
    template <class Function>
    std::uintptr_t write_prologue_hook(std::uintptr_t a_source, Function* a_destination) {
        const auto check = [](LONG a_result, const char* a_operation) {
            if (a_result != NO_ERROR) {
                throw std::runtime_error(std::string(a_operation) + " failed (Windows error " + std::to_string(a_result) + ")");
            }
        };

        auto original = reinterpret_cast<PVOID>(a_source);
        check(DetourTransactionBegin(), "DetourTransactionBegin");
        auto result = DetourUpdateThread(GetCurrentThread());
        if (result == NO_ERROR) {
            result = DetourAttach(&original, reinterpret_cast<PVOID>(a_destination));
        }
        if (result != NO_ERROR) {
            DetourTransactionAbort();
            check(result, "Prologue hook attachment");
        }
        check(DetourTransactionCommit(), "DetourTransactionCommit");
        return reinterpret_cast<std::uintptr_t>(original);
    }
}
