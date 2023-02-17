//    Copyright 2022, farm-ng inc.
//
//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
//
//        http://www.apache.org/licenses/LICENSE-2.0
//
//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.

#pragma once

#include "farm_ng/core/enum/enum.h"
#include "farm_ng/core/logging/format.h"
#include "farm_ng/core/logging/logger.h"

#include <farm_pp/preprocessor/comma.hpp>
#include <farm_pp/preprocessor/empty.hpp>

#include <filesystem>
#include <iostream>

#define FARM_LOG_LEVEL_TRACE 0
#define FARM_LOG_LEVEL_DEBUG 1
#define FARM_LOG_LEVEL_INFO 2

static_assert(FARM_LOG_LEVEL_TRACE == int(::farm_ng::LogLevel::trace));
static_assert(FARM_LOG_LEVEL_DEBUG == int(::farm_ng::LogLevel::debug));
static_assert(FARM_LOG_LEVEL_INFO == int(::farm_ng::LogLevel::info));

// All other log levels (INFO, WARN, CRITICAL,...) and corresponding log macros
// LOG_INFO, LOG_WARN are always compiled (unless globally disabled).
// In addition for beeing defined at compile-time, logging must also be enabled
// at run-time be specifying the appropriate run-time log level using the
// corresponding logger, such as
// ``defaultLogger().setLogLevel(LogLevel::trace);``

#ifdef FARM_ONLY_INCLUDE_ME_IN_CPP_FILE
static_assert(
    false,
    "Shoot FARM_ONLY_INCLUDE_ME_IN_CPP_FILE is already define, but it should "
    "not! You must include trace_debug_log.h only from a *.cpp file and you "
    "must "
    "define "
    "FARM_LOG_LEVEL right above the include.");
#endif
#define FARM_ONLY_INCLUDE_ME_IN_CPP_FILE 1

#ifndef FARM_LOG_LEVEL
static_assert(
    false,
    "You must include log_macros.h only from a *.cpp file and you must define "
    "FARM_LOG_LEVEL right above the include.");
#else
static_assert(
    FARM_LOG_LEVEL == FARM_LOG_LEVEL_TRACE ||
        FARM_LOG_LEVEL == FARM_LOG_LEVEL_DEBUG ||
        FARM_LOG_LEVEL == FARM_LOG_LEVEL_INFO,
    "specified level: " FARM_PP_STRINGIZE(FARM_LOG_LEVEL));
#endif

#define FARM_TRACE(...)
#define FARM_DEBUG(...)

#ifdef FARM_LOG_ENABLED
#if FARM_LOG_LEVEL <= FARM_LOG_LEVEL_TRACE
#undef FARM_TRACE
#define FARM_TRACE(...)         \
  farm_ng::defaultLogger().log( \
      farm_ng::LogLevel::trace, \
      "TRACE",                  \
      __FILE__,                 \
      __LINE__,                 \
      __func__,                 \
      __VA_ARGS__)
#endif
#if FARM_LOG_LEVEL <= FARM_LOG_LEVEL_DEBUG
#undef FARM_DEBUG
#define FARM_DEBUG(...)         \
  farm_ng::defaultLogger().log( \
      farm_ng::LogLevel::debug, \
      "DEBUG",                  \
      __FILE__,                 \
      __LINE__,                 \
      __func__,                 \
      __VA_ARGS__)
#endif
#endif  // FARM_LOG_ENABLED
