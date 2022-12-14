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
//   src/tint/ast/builtin_value_test.cc.tmpl
//
// Do not modify this file directly
////////////////////////////////////////////////////////////////////////////////

#include "src/tint/ast/builtin_value.h"

#include <string>

#include "src/tint/ast/test_helper.h"
#include "src/tint/utils/string.h"

namespace tint::ast {
namespace {

namespace parse_print_tests {

struct Case {
    const char* string;
    BuiltinValue value;
};

inline std::ostream& operator<<(std::ostream& out, Case c) {
    return out << "'" << std::string(c.string) << "'";
}

static constexpr Case kValidCases[] = {
    {"position", BuiltinValue::kPosition},
    {"vertex_index", BuiltinValue::kVertexIndex},
    {"instance_index", BuiltinValue::kInstanceIndex},
    {"front_facing", BuiltinValue::kFrontFacing},
    {"frag_depth", BuiltinValue::kFragDepth},
    {"local_invocation_id", BuiltinValue::kLocalInvocationId},
    {"local_invocation_index", BuiltinValue::kLocalInvocationIndex},
    {"global_invocation_id", BuiltinValue::kGlobalInvocationId},
    {"workgroup_id", BuiltinValue::kWorkgroupId},
    {"num_workgroups", BuiltinValue::kNumWorkgroups},
    {"sample_index", BuiltinValue::kSampleIndex},
    {"sample_mask", BuiltinValue::kSampleMask},
};

static constexpr Case kInvalidCases[] = {
    {"pccsitin", BuiltinValue::kInvalid},
    {"oiti3", BuiltinValue::kInvalid},
    {"positVon", BuiltinValue::kInvalid},
    {"1ertex_index", BuiltinValue::kInvalid},
    {"vertex_Jnqex", BuiltinValue::kInvalid},
    {"velltex_inde77", BuiltinValue::kInvalid},
    {"inpptanceqHHindx", BuiltinValue::kInvalid},
    {"cnsanvendex", BuiltinValue::kInvalid},
    {"istancG_index", BuiltinValue::kInvalid},
    {"front_facvnii", BuiltinValue::kInvalid},
    {"frWWnt_faci8g", BuiltinValue::kInvalid},
    {"fxxonM_facig", BuiltinValue::kInvalid},
    {"fXag_detgg", BuiltinValue::kInvalid},
    {"fag_XuVh", BuiltinValue::kInvalid},
    {"frag_dept3", BuiltinValue::kInvalid},
    {"local_Envocation_id", BuiltinValue::kInvalid},
    {"localiPPvocatioTT_id", BuiltinValue::kInvalid},
    {"localxxnvocationddid", BuiltinValue::kInvalid},
    {"loca44_invocation_index", BuiltinValue::kInvalid},
    {"local_invocSStionVVindex", BuiltinValue::kInvalid},
    {"locRR_invocat22n_index", BuiltinValue::kInvalid},
    {"globalFinvoction_id", BuiltinValue::kInvalid},
    {"gloal_invocation_id", BuiltinValue::kInvalid},
    {"RRlHOOaV_invoction_id", BuiltinValue::kInvalid},
    {"workgyoup_i", BuiltinValue::kInvalid},
    {"wnrrrkg77loup_Gd", BuiltinValue::kInvalid},
    {"00orkgr4up_id", BuiltinValue::kInvalid},
    {"numwroogrops", BuiltinValue::kInvalid},
    {"nzm_wokgroups", BuiltinValue::kInvalid},
    {"uippworkgro11ps", BuiltinValue::kInvalid},
    {"sample_iXXdex", BuiltinValue::kInvalid},
    {"5nnample_99IIdex", BuiltinValue::kInvalid},
    {"samYlaaHHrrndeSS", BuiltinValue::kInvalid},
    {"aHkk_mask", BuiltinValue::kInvalid},
    {"jRRmpl_gsk", BuiltinValue::kInvalid},
    {"smple_mbk", BuiltinValue::kInvalid},
};

using BuiltinValueParseTest = testing::TestWithParam<Case>;

TEST_P(BuiltinValueParseTest, Parse) {
    const char* string = GetParam().string;
    BuiltinValue expect = GetParam().value;
    EXPECT_EQ(expect, ParseBuiltinValue(string));
}

INSTANTIATE_TEST_SUITE_P(ValidCases, BuiltinValueParseTest, testing::ValuesIn(kValidCases));
INSTANTIATE_TEST_SUITE_P(InvalidCases, BuiltinValueParseTest, testing::ValuesIn(kInvalidCases));

using BuiltinValuePrintTest = testing::TestWithParam<Case>;

TEST_P(BuiltinValuePrintTest, Print) {
    BuiltinValue value = GetParam().value;
    const char* expect = GetParam().string;
    EXPECT_EQ(expect, utils::ToString(value));
}

INSTANTIATE_TEST_SUITE_P(ValidCases, BuiltinValuePrintTest, testing::ValuesIn(kValidCases));

}  // namespace parse_print_tests

}  // namespace
}  // namespace tint::ast
