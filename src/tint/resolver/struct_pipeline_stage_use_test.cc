// Copyright 2021 The Tint Authors.
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

#include "src/tint/resolver/resolver.h"

#include "gmock/gmock.h"
#include "src/tint/ast/stage_attribute.h"
#include "src/tint/resolver/resolver_test_helper.h"
#include "src/tint/sem/struct.h"

using ::testing::UnorderedElementsAre;

using namespace tint::number_suffixes;  // NOLINT

namespace tint::resolver {
namespace {

using ResolverPipelineStageUseTest = ResolverTest;

TEST_F(ResolverPipelineStageUseTest, UnusedStruct) {
    auto* s = Structure("S", utils::Vector{Member("a", ty.f32(), utils::Vector{Location(0)})});

    ASSERT_TRUE(r()->Resolve()) << r()->error();

    auto* sem = TypeOf(s)->As<sem::Struct>();
    ASSERT_NE(sem, nullptr);
    EXPECT_TRUE(sem->PipelineStageUses().empty());
}

TEST_F(ResolverPipelineStageUseTest, StructUsedAsNonEntryPointParam) {
    auto* s = Structure("S", utils::Vector{Member("a", ty.f32(), utils::Vector{Location(0)})});

    Func("foo", utils::Vector{Param("param", ty.Of(s))}, ty.void_(), utils::Empty, utils::Empty);

    ASSERT_TRUE(r()->Resolve()) << r()->error();

    auto* sem = TypeOf(s)->As<sem::Struct>();
    ASSERT_NE(sem, nullptr);
    EXPECT_TRUE(sem->PipelineStageUses().empty());
}

TEST_F(ResolverPipelineStageUseTest, StructUsedAsNonEntryPointReturnType) {
    auto* s = Structure("S", utils::Vector{Member("a", ty.f32(), utils::Vector{Location(0)})});

    Func("foo", utils::Empty, ty.Of(s), utils::Vector{Return(Construct(ty.Of(s), Expr(0_f)))},
         utils::Empty);

    ASSERT_TRUE(r()->Resolve()) << r()->error();

    auto* sem = TypeOf(s)->As<sem::Struct>();
    ASSERT_NE(sem, nullptr);
    EXPECT_TRUE(sem->PipelineStageUses().empty());
}

TEST_F(ResolverPipelineStageUseTest, StructUsedAsVertexShaderParam) {
    auto* s = Structure("S", utils::Vector{Member("a", ty.f32(), utils::Vector{Location(0)})});

    Func("main", utils::Vector{Param("param", ty.Of(s))}, ty.vec4<f32>(),
         utils::Vector{Return(Construct(ty.vec4<f32>()))},
         utils::Vector{Stage(ast::PipelineStage::kVertex)},
         utils::Vector{Builtin(ast::BuiltinValue::kPosition)});

    ASSERT_TRUE(r()->Resolve()) << r()->error();

    auto* sem = TypeOf(s)->As<sem::Struct>();
    ASSERT_NE(sem, nullptr);
    EXPECT_THAT(sem->PipelineStageUses(),
                UnorderedElementsAre(sem::PipelineStageUsage::kVertexInput));
}

TEST_F(ResolverPipelineStageUseTest, StructUsedAsVertexShaderReturnType) {
    auto* s =
        Structure("S", utils::Vector{Member("a", ty.vec4<f32>(),
                                            utils::Vector{Builtin(ast::BuiltinValue::kPosition)})});

    Func("main", utils::Empty, ty.Of(s), utils::Vector{Return(Construct(ty.Of(s)))},
         utils::Vector{Stage(ast::PipelineStage::kVertex)});

    ASSERT_TRUE(r()->Resolve()) << r()->error();

    auto* sem = TypeOf(s)->As<sem::Struct>();
    ASSERT_NE(sem, nullptr);
    EXPECT_THAT(sem->PipelineStageUses(),
                UnorderedElementsAre(sem::PipelineStageUsage::kVertexOutput));
}

TEST_F(ResolverPipelineStageUseTest, StructUsedAsFragmentShaderParam) {
    auto* s = Structure("S", utils::Vector{Member("a", ty.f32(), utils::Vector{Location(0)})});

    Func("main", utils::Vector{Param("param", ty.Of(s))}, ty.void_(), utils::Empty,
         utils::Vector{Stage(ast::PipelineStage::kFragment)});

    ASSERT_TRUE(r()->Resolve()) << r()->error();

    auto* sem = TypeOf(s)->As<sem::Struct>();
    ASSERT_NE(sem, nullptr);
    EXPECT_THAT(sem->PipelineStageUses(),
                UnorderedElementsAre(sem::PipelineStageUsage::kFragmentInput));
}

TEST_F(ResolverPipelineStageUseTest, StructUsedAsFragmentShaderReturnType) {
    auto* s = Structure("S", utils::Vector{Member("a", ty.f32(), utils::Vector{Location(0)})});

    Func("main", utils::Empty, ty.Of(s), utils::Vector{Return(Construct(ty.Of(s), Expr(0_f)))},
         utils::Vector{Stage(ast::PipelineStage::kFragment)});

    ASSERT_TRUE(r()->Resolve()) << r()->error();

    auto* sem = TypeOf(s)->As<sem::Struct>();
    ASSERT_NE(sem, nullptr);
    EXPECT_THAT(sem->PipelineStageUses(),
                UnorderedElementsAre(sem::PipelineStageUsage::kFragmentOutput));
}

TEST_F(ResolverPipelineStageUseTest, StructUsedAsComputeShaderParam) {
    auto* s = Structure(
        "S", utils::Vector{Member(
                 "a", ty.u32(), utils::Vector{Builtin(ast::BuiltinValue::kLocalInvocationIndex)})});

    Func("main", utils::Vector{Param("param", ty.Of(s))}, ty.void_(), utils::Empty,
         utils::Vector{Stage(ast::PipelineStage::kCompute), WorkgroupSize(1_i)});

    ASSERT_TRUE(r()->Resolve()) << r()->error();

    auto* sem = TypeOf(s)->As<sem::Struct>();
    ASSERT_NE(sem, nullptr);
    EXPECT_THAT(sem->PipelineStageUses(),
                UnorderedElementsAre(sem::PipelineStageUsage::kComputeInput));
}

TEST_F(ResolverPipelineStageUseTest, StructUsedMultipleStages) {
    auto* s =
        Structure("S", utils::Vector{Member("a", ty.vec4<f32>(),
                                            utils::Vector{Builtin(ast::BuiltinValue::kPosition)})});

    Func("vert_main", utils::Empty, ty.Of(s), utils::Vector{Return(Construct(ty.Of(s)))},
         utils::Vector{Stage(ast::PipelineStage::kVertex)});

    Func("frag_main", utils::Vector{Param("param", ty.Of(s))}, ty.void_(), utils::Empty,
         utils::Vector{Stage(ast::PipelineStage::kFragment)});

    ASSERT_TRUE(r()->Resolve()) << r()->error();

    auto* sem = TypeOf(s)->As<sem::Struct>();
    ASSERT_NE(sem, nullptr);
    EXPECT_THAT(sem->PipelineStageUses(),
                UnorderedElementsAre(sem::PipelineStageUsage::kVertexOutput,
                                     sem::PipelineStageUsage::kFragmentInput));
}

TEST_F(ResolverPipelineStageUseTest, StructUsedAsShaderParamViaAlias) {
    auto* s = Structure("S", utils::Vector{Member("a", ty.f32(), utils::Vector{Location(0)})});
    auto* s_alias = Alias("S_alias", ty.Of(s));

    Func("main", utils::Vector{Param("param", ty.Of(s_alias))}, ty.void_(), utils::Empty,
         utils::Vector{Stage(ast::PipelineStage::kFragment)});

    ASSERT_TRUE(r()->Resolve()) << r()->error();

    auto* sem = TypeOf(s)->As<sem::Struct>();
    ASSERT_NE(sem, nullptr);
    EXPECT_THAT(sem->PipelineStageUses(),
                UnorderedElementsAre(sem::PipelineStageUsage::kFragmentInput));
}

TEST_F(ResolverPipelineStageUseTest, StructUsedAsShaderReturnTypeViaAlias) {
    auto* s = Structure("S", utils::Vector{Member("a", ty.f32(), utils::Vector{Location(0)})});
    auto* s_alias = Alias("S_alias", ty.Of(s));

    Func("main", utils::Empty, ty.Of(s_alias),
         utils::Vector{Return(Construct(ty.Of(s_alias), Expr(0_f)))},
         utils::Vector{Stage(ast::PipelineStage::kFragment)});

    ASSERT_TRUE(r()->Resolve()) << r()->error();

    auto* sem = TypeOf(s)->As<sem::Struct>();
    ASSERT_NE(sem, nullptr);
    EXPECT_THAT(sem->PipelineStageUses(),
                UnorderedElementsAre(sem::PipelineStageUsage::kFragmentOutput));
}

}  // namespace
}  // namespace tint::resolver
