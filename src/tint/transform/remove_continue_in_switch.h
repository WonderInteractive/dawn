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

#ifndef SRC_TINT_TRANSFORM_REMOVE_CONTINUE_IN_SWITCH_H_
#define SRC_TINT_TRANSFORM_REMOVE_CONTINUE_IN_SWITCH_H_

#include "src/tint/transform/transform.h"

namespace tint::transform {

/// This transform replaces continue statements in switch cases with setting a
/// bool variable, and checking if the variable is set after the switch to
/// continue. It is necessary to work around FXC "error X3708: continue cannot
/// be used in a switch". See crbug.com/tint/1080.
class RemoveContinueInSwitch final : public Castable<RemoveContinueInSwitch, Transform> {
  public:
    /// Constructor
    RemoveContinueInSwitch();

    /// Destructor
    ~RemoveContinueInSwitch() override;

  protected:
    /// @param program the program to inspect
    /// @param data optional extra transform-specific input data
    /// @returns true if this transform should be run for the given program
    bool ShouldRun(const Program* program, const DataMap& data = {}) const override;

    /// Runs the transform using the CloneContext built for transforming a
    /// program. Run() is responsible for calling Clone() on the CloneContext.
    /// @param ctx the CloneContext primed with the input program and
    /// ProgramBuilder
    /// @param inputs optional extra transform-specific input data
    /// @param outputs optional extra transform-specific output data
    void Run(CloneContext& ctx, const DataMap& inputs, DataMap& outputs) const override;
};

}  // namespace tint::transform

#endif  // SRC_TINT_TRANSFORM_REMOVE_CONTINUE_IN_SWITCH_H_
