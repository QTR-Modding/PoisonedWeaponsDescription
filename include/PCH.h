#pragma once

#include <exception>
#include <format>
#include <string>
#include <string_view>

#include <spdlog/sinks/basic_file_sink.h>

#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"

namespace logger = SKSE::log;
using namespace std::literals;
