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

//
// https://panthema.net/2008/0901-stacktrace-demangled/stacktrace.h
// stacktrace.h (c) 2008, Timo Bingmann from http://idlebox.net/
// published under the WTFPL v2.0

#include <cxxabi.h>
#include <execinfo.h>

#include <cstdio>
#include <cstdlib>

/** Print a demangled stack backtrace of the caller function to FILE* out. */
inline static void printStacktrace(
    FILE* out = stderr, unsigned int max_frames = 63) {
  fprintf(out, "stack trace:\n");

  // storage array for stack trace address data
  void* addrlist[max_frames + 1];

  // retrieve current stack addresses
  int addrlen = backtrace(addrlist, sizeof(addrlist) / sizeof(void*));

  if (addrlen == 0) {
    fprintf(out, "  <empty, possibly corrupt>\n");
    return;
  }

  // resolve addresses into strings containing "filename(function+address)",
  // this array must be free()-ed
  char** symbollist = backtrace_symbols(addrlist, addrlen);

  // allocate string which will be filled with the demangled function name
  size_t funcnamesize = 256;
  char* funcname = (char*)malloc(funcnamesize);  // NOLINT

  // iterate over the returned symbol lines. skip the first, it is the
  // address of this function.
  for (int i = 1; i < addrlen; i++) {
    char* begin_name = 0;
    char* begin_offset = 0;
    char* end_offset = 0;

    // find parentheses and +address offset surrounding the mangled name:
    // ./module(function+0x15c) [0x8048a6d]
    for (char* p = symbollist[i]; *p != 0; ++p) {
      if (*p == '(') {
        begin_name = p;
      } else if (*p == '+') {
        begin_offset = p;
      } else if (*p == ')' && (begin_offset != nullptr)) {
        end_offset = p;
        break;
      }
    }

    if ((begin_name != nullptr) && (begin_offset != nullptr) &&
        (end_offset != nullptr) && begin_name < begin_offset) {
      *begin_name++ = '\0';
      *begin_offset++ = '\0';
      *end_offset = '\0';

      // mangled name is now in [begin_name, begin_offset) and caller
      // offset in [begin_offset, end_offset). now apply
      // __cxa_demangle():

      int status = 0;
      char* ret =
          abi::__cxa_demangle(begin_name, funcname, &funcnamesize, &status);
      if (status == 0) {
        funcname = ret;  // use possibly realloc()-ed string
        fprintf(out, "  %s : %s+%s\n", symbollist[i], funcname, begin_offset);
      } else {
        // demangling failed. Output function name as a C function with
        // no arguments.
        fprintf(
            out, "  %s : %s()+%s\n", symbollist[i], begin_name, begin_offset);
      }
    } else {
      // couldn't parse the line? print the whole line.
      fprintf(out, "  %s\n", symbollist[i]);
    }
  }

  free(funcname);    // NOLINT
  free(symbollist);  // NOLINT
}

namespace farm_ng {
void printBacktrace() { printStacktrace(); }
}  // namespace farm_ng
