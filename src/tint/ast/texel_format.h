// Copyright 2022 The Tint Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

////////////////////////////////////////////////////////////////////////////////
// File generated by tools/src/cmd/gen
// using the template:
//   src/tint/ast/texel_format.h.tmpl
//
// Do not modify this file directly
////////////////////////////////////////////////////////////////////////////////

#ifndef SRC_TINT_AST_TEXEL_FORMAT_H_
#define SRC_TINT_AST_TEXEL_FORMAT_H_

#include <ostream>

namespace tint::ast {

/// Enumerator of texel formats
enum class TexelFormat {
    kInvalid,
    kRgba8Unorm,
    kRgba8Snorm,
    kRgba8Uint,
    kRgba8Sint,
    kRgba16Uint,
    kRgba16Sint,
    kRgba16Float,
    kR32Uint,
    kR32Sint,
    kR32Float,
    kRg32Uint,
    kRg32Sint,
    kRg32Float,
    kRgba32Uint,
    kRgba32Sint,
    kRgba32Float,
};

/// @param out the std::ostream to write to
/// @param value the TexelFormat
/// @returns `out` so calls can be chained
std::ostream& operator<<(std::ostream& out, TexelFormat value);

/// ParseTexelFormat parses a TexelFormat from a string.
/// @param str the string to parse
/// @returns the parsed enum, or TexelFormat::kInvalid if the string could not be parsed.
TexelFormat ParseTexelFormat(std::string_view str);

}  // namespace tint::ast

#endif  // SRC_TINT_AST_TEXEL_FORMAT_H_
