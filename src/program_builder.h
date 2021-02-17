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

#ifndef SRC_PROGRAM_BUILDER_H_
#define SRC_PROGRAM_BUILDER_H_

#include <string>
#include <utility>

#include "src/ast/array_accessor_expression.h"
#include "src/ast/binary_expression.h"
#include "src/ast/bool_literal.h"
#include "src/ast/call_expression.h"
#include "src/ast/expression.h"
#include "src/ast/float_literal.h"
#include "src/ast/identifier_expression.h"
#include "src/ast/member_accessor_expression.h"
#include "src/ast/module.h"
#include "src/ast/scalar_constructor_expression.h"
#include "src/ast/sint_literal.h"
#include "src/ast/stride_decoration.h"
#include "src/ast/struct.h"
#include "src/ast/struct_member.h"
#include "src/ast/struct_member_offset_decoration.h"
#include "src/ast/type_constructor_expression.h"
#include "src/ast/uint_literal.h"
#include "src/ast/variable.h"
#include "src/diagnostic/diagnostic.h"
#include "src/program.h"
#include "src/semantic/info.h"
#include "src/semantic/node.h"
#include "src/symbol_table.h"
#include "src/type/alias_type.h"
#include "src/type/array_type.h"
#include "src/type/bool_type.h"
#include "src/type/f32_type.h"
#include "src/type/i32_type.h"
#include "src/type/matrix_type.h"
#include "src/type/pointer_type.h"
#include "src/type/struct_type.h"
#include "src/type/type_manager.h"
#include "src/type/u32_type.h"
#include "src/type/vector_type.h"
#include "src/type/void_type.h"

namespace tint {

// Forward declarations
namespace ast {
class VariableDeclStatement;
}  // namespace ast

class CloneContext;

/// ProgramBuilder is a mutable builder for a Program.
/// To construct a Program, populate the builder and then `std::move` it to a
/// Program.
class ProgramBuilder {
 public:
  /// ASTNodeAllocator is an alias to BlockAllocator<ast::Node>
  using ASTNodeAllocator = BlockAllocator<ast::Node>;

  /// SemNodeAllocator is an alias to BlockAllocator<semantic::Node>
  using SemNodeAllocator = BlockAllocator<semantic::Node>;

  /// `i32` is a type alias to `int`.
  /// Useful for passing to template methods such as `vec2<i32>()` to imitate
  /// WGSL syntax.
  /// Note: this is intentionally not aliased to uint32_t as we want integer
  /// literals passed to the builder to match WGSL's integer literal types.
  using i32 = decltype(1);
  /// `u32` is a type alias to `unsigned int`.
  /// Useful for passing to template methods such as `vec2<u32>()` to imitate
  /// WGSL syntax.
  /// Note: this is intentionally not aliased to uint32_t as we want integer
  /// literals passed to the builder to match WGSL's integer literal types.
  using u32 = decltype(1u);
  /// `f32` is a type alias to `float`
  /// Useful for passing to template methods such as `vec2<f32>()` to imitate
  /// WGSL syntax.
  using f32 = float;

  /// Constructor
  ProgramBuilder();

  /// Move constructor
  /// @param rhs the builder to move
  ProgramBuilder(ProgramBuilder&& rhs);

  /// Destructor
  virtual ~ProgramBuilder();

  /// Move assignment operator
  /// @param rhs the builder to move
  /// @return this builder
  ProgramBuilder& operator=(ProgramBuilder&& rhs);

  /// Wrap returns a new ProgramBuilder wrapping the Program `program` without
  /// making a deep clone of the Program contents.
  /// ProgramBuilder returned by Wrap() is intended to temporarily extend an
  /// existing immutable program.
  /// As the returned ProgramBuilder wraps `program`, `program` must not be
  /// destructed or assigned while using the returned ProgramBuilder.
  /// TODO(bclayton) - Evaluate whether there are safer alternatives to this
  /// function. See crbug.com/tint/460.
  /// @param program the immutable Program to wrap
  /// @return the ProgramBuilder that wraps `program`
  static ProgramBuilder Wrap(const Program* program);

  /// @returns a reference to the program's types
  type::Manager& Types() {
    AssertNotMoved();
    return types_;
  }

  /// @returns a reference to the program's types
  const type::Manager& Types() const {
    AssertNotMoved();
    return types_;
  }

  /// @returns a reference to the program's AST nodes storage
  ASTNodeAllocator& ASTNodes() {
    AssertNotMoved();
    return ast_nodes_;
  }

  /// @returns a reference to the program's AST nodes storage
  const ASTNodeAllocator& ASTNodes() const {
    AssertNotMoved();
    return ast_nodes_;
  }

  /// @returns a reference to the program's semantic nodes storage
  SemNodeAllocator& SemNodes() {
    AssertNotMoved();
    return sem_nodes_;
  }

  /// @returns a reference to the program's semantic nodes storage
  const SemNodeAllocator& SemNodes() const {
    AssertNotMoved();
    return sem_nodes_;
  }

  /// @returns a reference to the program's AST root Module
  ast::Module& AST() {
    AssertNotMoved();
    return *ast_;
  }

  /// @returns a reference to the program's AST root Module
  const ast::Module& AST() const {
    AssertNotMoved();
    return *ast_;
  }

  /// @returns a reference to the program's semantic info
  semantic::Info& Sem() {
    AssertNotMoved();
    return sem_;
  }

  /// @returns a reference to the program's semantic info
  const semantic::Info& Sem() const {
    AssertNotMoved();
    return sem_;
  }

  /// @returns a reference to the program's SymbolTable
  SymbolTable& Symbols() {
    AssertNotMoved();
    return symbols_;
  }

  /// @returns a reference to the program's SymbolTable
  const SymbolTable& Symbols() const {
    AssertNotMoved();
    return symbols_;
  }

  /// @returns a reference to the program's diagnostics
  diag::List& Diagnostics() {
    AssertNotMoved();
    return diagnostics_;
  }

  /// @returns a reference to the program's diagnostics
  const diag::List& Diagnostics() const {
    AssertNotMoved();
    return diagnostics_;
  }

  /// Controls whether the TypeDeterminer will be run on the program when it is
  /// built.
  /// @param enable the new flag value (defaults to true)
  void SetResolveOnBuild(bool enable) { resolve_on_build_ = enable; }

  /// @return true if the TypeDeterminer will be run on the program when it is
  /// built.
  bool ResolveOnBuild() const { return resolve_on_build_; }

  /// @returns true if the program has no error diagnostics and is not missing
  /// information
  bool IsValid() const;

  /// Writes a representation of the node to the output stream
  /// @note unlike str(), to_str() does not automatically demangle the string.
  /// @param node the AST node
  /// @param out the stream to write to
  /// @param indent number of spaces to indent the node when writing
  void to_str(const ast::Node* node, std::ostream& out, size_t indent) const {
    node->to_str(Sem(), out, indent);
  }

  /// Returns a demangled, string representation of `node`.
  /// @param node the AST node
  /// @returns a string representation of the node
  std::string str(const ast::Node* node) const;

  /// Creates a new ast::Node owned by the ProgramBuilder. When the
  /// ProgramBuilder is destructed, the ast::Node will also be destructed.
  /// @param source the Source of the node
  /// @param args the arguments to pass to the type constructor
  /// @returns the node pointer
  template <typename T, typename... ARGS>
  traits::EnableIfIsType<T, ast::Node>* create(const Source& source,
                                               ARGS&&... args) {
    AssertNotMoved();
    return ast_nodes_.Create<T>(source, std::forward<ARGS>(args)...);
  }

  /// Creates a new ast::Node owned by the ProgramBuilder, injecting the current
  /// Source as set by the last call to SetSource() as the only argument to the
  /// constructor.
  /// When the ProgramBuilder is destructed, the ast::Node will also be
  /// destructed.
  /// @returns the node pointer
  template <typename T>
  traits::EnableIfIsType<T, ast::Node>* create() {
    AssertNotMoved();
    return ast_nodes_.Create<T>(source_);
  }

  /// Creates a new ast::Node owned by the ProgramBuilder, injecting the current
  /// Source as set by the last call to SetSource() as the first argument to the
  /// constructor.
  /// When the ProgramBuilder is destructed, the ast::Node will also be
  /// destructed.
  /// @param arg0 the first arguments to pass to the type constructor
  /// @param args the remaining arguments to pass to the type constructor
  /// @returns the node pointer
  template <typename T, typename ARG0, typename... ARGS>
  traits::EnableIf</* T is ast::Node and ARG0 is not Source */
                   traits::IsTypeOrDerived<T, ast::Node>::value &&
                       !traits::IsTypeOrDerived<ARG0, Source>::value,
                   T>*
  create(ARG0&& arg0, ARGS&&... args) {
    AssertNotMoved();
    return ast_nodes_.Create<T>(source_, std::forward<ARG0>(arg0),
                                std::forward<ARGS>(args)...);
  }

  /// Creates a new semantic::Node owned by the ProgramBuilder.
  /// When the ProgramBuilder is destructed, the semantic::Node will also be
  /// destructed.
  /// @param args the arguments to pass to the type constructor
  /// @returns the node pointer
  template <typename T, typename... ARGS>
  traits::EnableIfIsType<T, semantic::Node>* create(ARGS&&... args) {
    AssertNotMoved();
    return sem_nodes_.Create<T>(std::forward<ARGS>(args)...);
  }

  /// Creates a new type::Type owned by the ProgramBuilder.
  /// When the ProgramBuilder is destructed, owned ProgramBuilder and the
  /// returned`Type` will also be destructed.
  /// Types are unique (de-aliased), and so calling create() for the same `T`
  /// and arguments will return the same pointer.
  /// @warning Use this method to acquire a type only if all of its type
  /// information is provided in the constructor arguments `args`.<br>
  /// If the type requires additional configuration after construction that
  /// affect its fundamental type, build the type with `std::make_unique`, make
  /// any necessary alterations and then call unique_type() instead.
  /// @param args the arguments to pass to the type constructor
  /// @returns the de-aliased type pointer
  template <typename T, typename... ARGS>
  traits::EnableIfIsType<T, type::Type>* create(ARGS&&... args) {
    static_assert(std::is_base_of<type::Type, T>::value,
                  "T does not derive from type::Type");
    AssertNotMoved();
    return types_.Get<T>(std::forward<ARGS>(args)...);
  }

  /// Marks this builder as moved, preventing any further use of the builder.
  void MarkAsMoved();

  //////////////////////////////////////////////////////////////////////////////
  // TypesBuilder
  //////////////////////////////////////////////////////////////////////////////

  /// TypesBuilder holds basic `tint` types and methods for constructing
  /// complex types.
  class TypesBuilder {
   public:
    /// Constructor
    /// @param builder the program builder
    explicit TypesBuilder(ProgramBuilder* builder);

    /// @return the tint AST type for the C type `T`.
    template <typename T>
    type::Type* Of() const {
      return CToAST<T>::get(this);
    }

    /// @returns a boolean type
    type::Bool* bool_() const { return builder->create<type::Bool>(); }

    /// @returns a f32 type
    type::F32* f32() const { return builder->create<type::F32>(); }

    /// @returns a i32 type
    type::I32* i32() const { return builder->create<type::I32>(); }

    /// @returns a u32 type
    type::U32* u32() const { return builder->create<type::U32>(); }

    /// @returns a void type
    type::Void* void_() const { return builder->create<type::Void>(); }

    /// @return the tint AST type for a 2-element vector of the C type `T`.
    template <typename T>
    type::Vector* vec2() const {
      return builder->create<type::Vector>(Of<T>(), 2);
    }

    /// @return the tint AST type for a 3-element vector of the C type `T`.
    template <typename T>
    type::Vector* vec3() const {
      return builder->create<type::Vector>(Of<T>(), 3);
    }

    /// @return the tint AST type for a 4-element vector of the C type `T`.
    template <typename T>
    type::Type* vec4() const {
      return builder->create<type::Vector>(Of<T>(), 4);
    }

    /// @return the tint AST type for a 2x3 matrix of the C type `T`.
    template <typename T>
    type::Matrix* mat2x2() const {
      return builder->create<type::Matrix>(Of<T>(), 2, 2);
    }

    /// @return the tint AST type for a 2x3 matrix of the C type `T`.
    template <typename T>
    type::Matrix* mat2x3() const {
      return builder->create<type::Matrix>(Of<T>(), 3, 2);
    }

    /// @return the tint AST type for a 2x4 matrix of the C type `T`.
    template <typename T>
    type::Matrix* mat2x4() const {
      return builder->create<type::Matrix>(Of<T>(), 4, 2);
    }

    /// @return the tint AST type for a 3x2 matrix of the C type `T`.
    template <typename T>
    type::Matrix* mat3x2() const {
      return builder->create<type::Matrix>(Of<T>(), 2, 3);
    }

    /// @return the tint AST type for a 3x3 matrix of the C type `T`.
    template <typename T>
    type::Matrix* mat3x3() const {
      return builder->create<type::Matrix>(Of<T>(), 3, 3);
    }

    /// @return the tint AST type for a 3x4 matrix of the C type `T`.
    template <typename T>
    type::Matrix* mat3x4() const {
      return builder->create<type::Matrix>(Of<T>(), 4, 3);
    }

    /// @return the tint AST type for a 4x2 matrix of the C type `T`.
    template <typename T>
    type::Matrix* mat4x2() const {
      return builder->create<type::Matrix>(Of<T>(), 2, 4);
    }

    /// @return the tint AST type for a 4x3 matrix of the C type `T`.
    template <typename T>
    type::Matrix* mat4x3() const {
      return builder->create<type::Matrix>(Of<T>(), 3, 4);
    }

    /// @return the tint AST type for a 4x4 matrix of the C type `T`.
    template <typename T>
    type::Matrix* mat4x4() const {
      return builder->create<type::Matrix>(Of<T>(), 4, 4);
    }

    /// @param subtype the array element type
    /// @param n the array size. 0 represents a runtime-array.
    /// @return the tint AST type for a array of size `n` of type `T`
    type::Array* array(type::Type* subtype, uint32_t n) const {
      return builder->create<type::Array>(subtype, n,
                                          ast::ArrayDecorationList{});
    }

    /// @param subtype the array element type
    /// @param n the array size. 0 represents a runtime-array.
    /// @param stride the array stride.
    /// @return the tint AST type for a array of size `n` of type `T`
    type::Array* array(type::Type* subtype, uint32_t n, uint32_t stride) const {
      return builder->create<type::Array>(
          subtype, n,
          ast::ArrayDecorationList{
              builder->create<ast::StrideDecoration>(stride),
          });
    }

    /// @return the tint AST type for an array of size `N` of type `T`
    template <typename T, int N = 0>
    type::Array* array() const {
      return array(Of<T>(), N);
    }

    /// @param stride the array stride
    /// @return the tint AST type for an array of size `N` of type `T`
    template <typename T, int N = 0>
    type::Array* array(uint32_t stride) const {
      return array(Of<T>(), N, stride);
    }
    /// Creates an alias type
    /// @param name the alias name
    /// @param type the alias type
    /// @returns the alias pointer
    type::Alias* alias(const std::string& name, type::Type* type) const {
      return builder->create<type::Alias>(builder->Symbols().Register(name),
                                          type);
    }

    /// @return the tint AST pointer to type `T` with the given
    /// ast::StorageClass.
    /// @param storage_class the storage class of the pointer
    template <typename T>
    type::Pointer* pointer(ast::StorageClass storage_class) const {
      return builder->create<type::Pointer>(Of<T>(), storage_class);
    }

    /// @param name the struct name
    /// @param impl the struct implementation
    /// @returns a struct pointer
    type::Struct* struct_(const std::string& name, ast::Struct* impl) const {
      return builder->create<type::Struct>(builder->Symbols().Register(name),
                                           impl);
    }

   private:
    /// CToAST<T> is specialized for various `T` types and each specialization
    /// contains a single static `get()` method for obtaining the corresponding
    /// AST type for the C type `T`.
    /// `get()` has the signature:
    ///    `static type::Type* get(Types* t)`
    template <typename T>
    struct CToAST {};

    ProgramBuilder* const builder;
  };

  //////////////////////////////////////////////////////////////////////////////
  // AST helper methods
  //////////////////////////////////////////////////////////////////////////////

  /// @param expr the expression
  /// @return expr
  ast::Expression* Expr(ast::Expression* expr) { return expr; }

  /// @param name the identifier name
  /// @return an ast::IdentifierExpression with the given name
  ast::IdentifierExpression* Expr(const std::string& name) {
    return create<ast::IdentifierExpression>(Symbols().Register(name));
  }

  /// @param symbol the identifier symbol
  /// @return an ast::IdentifierExpression with the given symbol
  ast::IdentifierExpression* Expr(Symbol symbol) {
    return create<ast::IdentifierExpression>(symbol);
  }

  /// @param source the source information
  /// @param name the identifier name
  /// @return an ast::IdentifierExpression with the given name
  ast::IdentifierExpression* Expr(const Source& source,
                                  const std::string& name) {
    return create<ast::IdentifierExpression>(source, Symbols().Register(name));
  }

  /// @param name the identifier name
  /// @return an ast::IdentifierExpression with the given name
  ast::IdentifierExpression* Expr(const char* name) {
    return create<ast::IdentifierExpression>(Symbols().Register(name));
  }

  /// @param value the boolean value
  /// @return a Scalar constructor for the given value
  ast::ScalarConstructorExpression* Expr(bool value) {
    return create<ast::ScalarConstructorExpression>(Literal(value));
  }

  /// @param value the float value
  /// @return a Scalar constructor for the given value
  ast::ScalarConstructorExpression* Expr(f32 value) {
    return create<ast::ScalarConstructorExpression>(Literal(value));
  }

  /// @param value the integer value
  /// @return a Scalar constructor for the given value
  ast::ScalarConstructorExpression* Expr(i32 value) {
    return create<ast::ScalarConstructorExpression>(Literal(value));
  }

  /// @param value the unsigned int value
  /// @return a Scalar constructor for the given value
  ast::ScalarConstructorExpression* Expr(u32 value) {
    return create<ast::ScalarConstructorExpression>(Literal(value));
  }

  /// Converts `arg` to an `ast::Expression` using `Expr()`, then appends it to
  /// `list`.
  /// @param list the list to append too
  /// @param arg the arg to create
  template <typename ARG>
  void Append(ast::ExpressionList& list, ARG&& arg) {
    list.emplace_back(Expr(std::forward<ARG>(arg)));
  }

  /// Converts `arg0` and `args` to `ast::Expression`s using `Expr()`,
  /// then appends them to `list`.
  /// @param list the list to append too
  /// @param arg0 the first argument
  /// @param args the rest of the arguments
  template <typename ARG0, typename... ARGS>
  void Append(ast::ExpressionList& list, ARG0&& arg0, ARGS&&... args) {
    Append(list, std::forward<ARG0>(arg0));
    Append(list, std::forward<ARGS>(args)...);
  }

  /// @return an empty list of expressions
  ast::ExpressionList ExprList() { return {}; }

  /// @param args the list of expressions
  /// @return the list of expressions converted to `ast::Expression`s using
  /// `Expr()`,
  template <typename... ARGS>
  ast::ExpressionList ExprList(ARGS&&... args) {
    ast::ExpressionList list;
    list.reserve(sizeof...(args));
    Append(list, std::forward<ARGS>(args)...);
    return list;
  }

  /// @param list the list of expressions
  /// @return `list`
  ast::ExpressionList ExprList(ast::ExpressionList list) { return list; }

  /// @param val the boolan value
  /// @return a boolean literal with the given value
  ast::BoolLiteral* Literal(bool val) {
    return create<ast::BoolLiteral>(ty.bool_(), val);
  }

  /// @param val the float value
  /// @return a float literal with the given value
  ast::FloatLiteral* Literal(f32 val) {
    return create<ast::FloatLiteral>(ty.f32(), val);
  }

  /// @param val the unsigned int value
  /// @return a ast::UintLiteral with the given value
  ast::UintLiteral* Literal(u32 val) {
    return create<ast::UintLiteral>(ty.u32(), val);
  }

  /// @param val the integer value
  /// @return the ast::SintLiteral with the given value
  ast::SintLiteral* Literal(i32 val) {
    return create<ast::SintLiteral>(ty.i32(), val);
  }

  /// @param args the arguments for the type constructor
  /// @return an `ast::TypeConstructorExpression` of type `ty`, with the values
  /// of `args` converted to `ast::Expression`s using `Expr()`
  template <typename T, typename... ARGS>
  ast::TypeConstructorExpression* Construct(ARGS&&... args) {
    return create<ast::TypeConstructorExpression>(
        ty.Of<T>(), ExprList(std::forward<ARGS>(args)...));
  }

  /// @param type the type to construct
  /// @param args the arguments for the constructor
  /// @return an `ast::TypeConstructorExpression` of `type` constructed with the
  /// values `args`.
  template <typename... ARGS>
  ast::TypeConstructorExpression* Construct(type::Type* type, ARGS&&... args) {
    return create<ast::TypeConstructorExpression>(
        type, ExprList(std::forward<ARGS>(args)...));
  }

  /// @param args the arguments for the vector constructor
  /// @return an `ast::TypeConstructorExpression` of a 2-element vector of type
  /// `T`, constructed with the values `args`.
  template <typename T, typename... ARGS>
  ast::TypeConstructorExpression* vec2(ARGS&&... args) {
    return create<ast::TypeConstructorExpression>(
        ty.vec2<T>(), ExprList(std::forward<ARGS>(args)...));
  }

  /// @param args the arguments for the vector constructor
  /// @return an `ast::TypeConstructorExpression` of a 3-element vector of type
  /// `T`, constructed with the values `args`.
  template <typename T, typename... ARGS>
  ast::TypeConstructorExpression* vec3(ARGS&&... args) {
    return create<ast::TypeConstructorExpression>(
        ty.vec3<T>(), ExprList(std::forward<ARGS>(args)...));
  }

  /// @param args the arguments for the vector constructor
  /// @return an `ast::TypeConstructorExpression` of a 4-element vector of type
  /// `T`, constructed with the values `args`.
  template <typename T, typename... ARGS>
  ast::TypeConstructorExpression* vec4(ARGS&&... args) {
    return create<ast::TypeConstructorExpression>(
        ty.vec4<T>(), ExprList(std::forward<ARGS>(args)...));
  }

  /// @param args the arguments for the matrix constructor
  /// @return an `ast::TypeConstructorExpression` of a 2x2 matrix of type
  /// `T`, constructed with the values `args`.
  template <typename T, typename... ARGS>
  ast::TypeConstructorExpression* mat2x2(ARGS&&... args) {
    return create<ast::TypeConstructorExpression>(
        ty.mat2x2<T>(), ExprList(std::forward<ARGS>(args)...));
  }

  /// @param args the arguments for the matrix constructor
  /// @return an `ast::TypeConstructorExpression` of a 2x3 matrix of type
  /// `T`, constructed with the values `args`.
  template <typename T, typename... ARGS>
  ast::TypeConstructorExpression* mat2x3(ARGS&&... args) {
    return create<ast::TypeConstructorExpression>(
        ty.mat2x3<T>(), ExprList(std::forward<ARGS>(args)...));
  }

  /// @param args the arguments for the matrix constructor
  /// @return an `ast::TypeConstructorExpression` of a 2x4 matrix of type
  /// `T`, constructed with the values `args`.
  template <typename T, typename... ARGS>
  ast::TypeConstructorExpression* mat2x4(ARGS&&... args) {
    return create<ast::TypeConstructorExpression>(
        ty.mat2x4<T>(), ExprList(std::forward<ARGS>(args)...));
  }

  /// @param args the arguments for the matrix constructor
  /// @return an `ast::TypeConstructorExpression` of a 3x2 matrix of type
  /// `T`, constructed with the values `args`.
  template <typename T, typename... ARGS>
  ast::TypeConstructorExpression* mat3x2(ARGS&&... args) {
    return create<ast::TypeConstructorExpression>(
        ty.mat3x2<T>(), ExprList(std::forward<ARGS>(args)...));
  }

  /// @param args the arguments for the matrix constructor
  /// @return an `ast::TypeConstructorExpression` of a 3x3 matrix of type
  /// `T`, constructed with the values `args`.
  template <typename T, typename... ARGS>
  ast::TypeConstructorExpression* mat3x3(ARGS&&... args) {
    return create<ast::TypeConstructorExpression>(
        ty.mat3x3<T>(), ExprList(std::forward<ARGS>(args)...));
  }

  /// @param args the arguments for the matrix constructor
  /// @return an `ast::TypeConstructorExpression` of a 3x4 matrix of type
  /// `T`, constructed with the values `args`.
  template <typename T, typename... ARGS>
  ast::TypeConstructorExpression* mat3x4(ARGS&&... args) {
    return create<ast::TypeConstructorExpression>(
        ty.mat3x4<T>(), ExprList(std::forward<ARGS>(args)...));
  }

  /// @param args the arguments for the matrix constructor
  /// @return an `ast::TypeConstructorExpression` of a 4x2 matrix of type
  /// `T`, constructed with the values `args`.
  template <typename T, typename... ARGS>
  ast::TypeConstructorExpression* mat4x2(ARGS&&... args) {
    return create<ast::TypeConstructorExpression>(
        ty.mat4x2<T>(), ExprList(std::forward<ARGS>(args)...));
  }

  /// @param args the arguments for the matrix constructor
  /// @return an `ast::TypeConstructorExpression` of a 4x3 matrix of type
  /// `T`, constructed with the values `args`.
  template <typename T, typename... ARGS>
  ast::TypeConstructorExpression* mat4x3(ARGS&&... args) {
    return create<ast::TypeConstructorExpression>(
        ty.mat4x3<T>(), ExprList(std::forward<ARGS>(args)...));
  }

  /// @param args the arguments for the matrix constructor
  /// @return an `ast::TypeConstructorExpression` of a 4x4 matrix of type
  /// `T`, constructed with the values `args`.
  template <typename T, typename... ARGS>
  ast::TypeConstructorExpression* mat4x4(ARGS&&... args) {
    return create<ast::TypeConstructorExpression>(
        ty.mat4x4<T>(), ExprList(std::forward<ARGS>(args)...));
  }

  /// @param args the arguments for the array constructor
  /// @return an `ast::TypeConstructorExpression` of an array with element type
  /// `T`, constructed with the values `args`.
  template <typename T, int N = 0, typename... ARGS>
  ast::TypeConstructorExpression* array(ARGS&&... args) {
    return create<ast::TypeConstructorExpression>(
        ty.array<T, N>(), ExprList(std::forward<ARGS>(args)...));
  }

  /// @param subtype the array element type
  /// @param n the array size. 0 represents a runtime-array.
  /// @param args the arguments for the array constructor
  /// @return an `ast::TypeConstructorExpression` of an array with element type
  /// `subtype`, constructed with the values `args`.
  template <typename... ARGS>
  ast::TypeConstructorExpression* array(type::Type* subtype,
                                        uint32_t n,
                                        ARGS&&... args) {
    return create<ast::TypeConstructorExpression>(
        ty.array(subtype, n), ExprList(std::forward<ARGS>(args)...));
  }

  /// @param name the variable name
  /// @param type the variable type
  /// @param storage the variable storage class
  /// @param constructor constructor expression
  /// @param decorations variable decorations
  /// @returns a `ast::Variable` with the given name, storage and type
  ast::Variable* Var(const std::string& name,
                     type::Type* type,
                     ast::StorageClass storage,
                     ast::Expression* constructor = nullptr,
                     ast::VariableDecorationList decorations = {}) {
    return create<ast::Variable>(Symbols().Register(name), storage, type, false,
                                 constructor, decorations);
  }

  /// @param source the variable source
  /// @param name the variable name
  /// @param type the variable type
  /// @param storage the variable storage class
  /// @param constructor constructor expression
  /// @param decorations variable decorations
  /// @returns a `ast::Variable` with the given name, storage and type
  ast::Variable* Var(const Source& source,
                     const std::string& name,
                     type::Type* type,
                     ast::StorageClass storage,
                     ast::Expression* constructor = nullptr,
                     ast::VariableDecorationList decorations = {}) {
    return create<ast::Variable>(source, Symbols().Register(name), storage,
                                 type, false, constructor, decorations);
  }

  /// @param symbol the variable symbol
  /// @param type the variable type
  /// @param storage the variable storage class
  /// @param constructor constructor expression
  /// @param decorations variable decorations
  /// @returns a `ast::Variable` with the given symbol, storage and type
  ast::Variable* Var(Symbol symbol,
                     type::Type* type,
                     ast::StorageClass storage,
                     ast::Expression* constructor = nullptr,
                     ast::VariableDecorationList decorations = {}) {
    return create<ast::Variable>(symbol, storage, type, false, constructor,
                                 decorations);
  }

  /// @param source the variable source
  /// @param symbol the variable symbol
  /// @param type the variable type
  /// @param storage the variable storage class
  /// @param constructor constructor expression
  /// @param decorations variable decorations
  /// @returns a `ast::Variable` with the given symbol, storage and type
  ast::Variable* Var(const Source& source,
                     Symbol symbol,
                     type::Type* type,
                     ast::StorageClass storage,
                     ast::Expression* constructor = nullptr,
                     ast::VariableDecorationList decorations = {}) {
    return create<ast::Variable>(source, symbol, storage, type, false,
                                 constructor, decorations);
  }

  /// @param name the variable name
  /// @param type the variable type
  /// @param constructor optional constructor expression
  /// @param decorations optional variable decorations
  /// @returns a constant `ast::Variable` with the given name, storage and type
  ast::Variable* Const(const std::string& name,
                       type::Type* type,
                       ast::Expression* constructor = nullptr,
                       ast::VariableDecorationList decorations = {}) {
    return create<ast::Variable>(Symbols().Register(name),
                                 ast::StorageClass::kNone, type, true,
                                 constructor, decorations);
  }

  /// @param source the variable source
  /// @param name the variable name
  /// @param type the variable type
  /// @param constructor optional constructor expression
  /// @param decorations optional variable decorations
  /// @returns a constant `ast::Variable` with the given name, storage and type
  ast::Variable* Const(const Source& source,
                       const std::string& name,
                       type::Type* type,
                       ast::Expression* constructor = nullptr,
                       ast::VariableDecorationList decorations = {}) {
    return create<ast::Variable>(source, Symbols().Register(name),
                                 ast::StorageClass::kNone, type, true,
                                 constructor, decorations);
  }

  /// @param symbol the variable symbol
  /// @param type the variable type
  /// @param constructor optional constructor expression
  /// @param decorations optional variable decorations
  /// @returns a constant `ast::Variable` with the given symbol, storage and
  /// type
  ast::Variable* Const(Symbol symbol,
                       type::Type* type,
                       ast::Expression* constructor = nullptr,
                       ast::VariableDecorationList decorations = {}) {
    return create<ast::Variable>(symbol, ast::StorageClass::kNone, type, true,
                                 constructor, decorations);
  }

  /// @param source the variable source
  /// @param symbol the variable symbol
  /// @param type the variable type
  /// @param constructor optional constructor expression
  /// @param decorations optional variable decorations
  /// @returns a constant `ast::Variable` with the given symbol, storage and
  /// type
  ast::Variable* Const(const Source& source,
                       Symbol symbol,
                       type::Type* type,
                       ast::Expression* constructor = nullptr,
                       ast::VariableDecorationList decorations = {}) {
    return create<ast::Variable>(source, symbol, ast::StorageClass::kNone, type,
                                 true, constructor, decorations);
  }

  /// @param args the arguments to pass to Var()
  /// @returns a `ast::Variable` constructed by calling Var() with the arguments
  /// of `args`, which is automatically registered as a global variable with the
  /// ast::Module.
  template <typename... ARGS>
  ast::Variable* Global(ARGS&&... args) {
    auto* var = Var(std::forward<ARGS>(args)...);
    AST().AddGlobalVariable(var);
    return var;
  }

  /// @param args the arguments to pass to Const()
  /// @returns a const `ast::Variable` constructed by calling Var() with the
  /// arguments of `args`, which is automatically registered as a global
  /// variable with the ast::Module.
  template <typename... ARGS>
  ast::Variable* GlobalConst(ARGS&&... args) {
    auto* var = Const(std::forward<ARGS>(args)...);
    AST().AddGlobalVariable(var);
    return var;
  }

  /// @param func the function name
  /// @param args the function call arguments
  /// @returns a `ast::CallExpression` to the function `func`, with the
  /// arguments of `args` converted to `ast::Expression`s using `Expr()`.
  template <typename NAME, typename... ARGS>
  ast::CallExpression* Call(NAME&& func, ARGS&&... args) {
    return create<ast::CallExpression>(Expr(func),
                                       ExprList(std::forward<ARGS>(args)...));
  }

  /// @param lhs the left hand argument to the addition operation
  /// @param rhs the right hand argument to the addition operation
  /// @returns a `ast::BinaryExpression` summing the arguments `lhs` and `rhs`
  template <typename LHS, typename RHS>
  ast::Expression* Add(LHS&& lhs, RHS&& rhs) {
    return create<ast::BinaryExpression>(ast::BinaryOp::kAdd,
                                         Expr(std::forward<LHS>(lhs)),
                                         Expr(std::forward<RHS>(rhs)));
  }

  /// @param lhs the left hand argument to the subtraction operation
  /// @param rhs the right hand argument to the subtraction operation
  /// @returns a `ast::BinaryExpression` subtracting `rhs` from `lhs`
  template <typename LHS, typename RHS>
  ast::Expression* Sub(LHS&& lhs, RHS&& rhs) {
    return create<ast::BinaryExpression>(ast::BinaryOp::kSubtract,
                                         Expr(std::forward<LHS>(lhs)),
                                         Expr(std::forward<RHS>(rhs)));
  }

  /// @param lhs the left hand argument to the multiplication operation
  /// @param rhs the right hand argument to the multiplication operation
  /// @returns a `ast::BinaryExpression` multiplying `rhs` from `lhs`
  template <typename LHS, typename RHS>
  ast::Expression* Mul(LHS&& lhs, RHS&& rhs) {
    return create<ast::BinaryExpression>(ast::BinaryOp::kMultiply,
                                         Expr(std::forward<LHS>(lhs)),
                                         Expr(std::forward<RHS>(rhs)));
  }

  /// @param arr the array argument for the array accessor expression
  /// @param idx the index argument for the array accessor expression
  /// @returns a `ast::ArrayAccessorExpression` that indexes `arr` with `idx`
  template <typename ARR, typename IDX>
  ast::Expression* IndexAccessor(ARR&& arr, IDX&& idx) {
    return create<ast::ArrayAccessorExpression>(Expr(std::forward<ARR>(arr)),
                                                Expr(std::forward<IDX>(idx)));
  }

  /// @param obj the object for the member accessor expression
  /// @param idx the index argument for the array accessor expression
  /// @returns a `ast::MemberAccessorExpression` that indexes `obj` with `idx`
  template <typename OBJ, typename IDX>
  ast::Expression* MemberAccessor(OBJ&& obj, IDX&& idx) {
    return create<ast::MemberAccessorExpression>(Expr(std::forward<OBJ>(obj)),
                                                 Expr(std::forward<IDX>(idx)));
  }

  /// Creates a ast::StructMemberOffsetDecoration
  /// @param val the offset value
  /// @returns the offset decoration pointer
  ast::StructMemberOffsetDecoration* MemberOffset(uint32_t val) {
    return create<ast::StructMemberOffsetDecoration>(source_, val);
  }

  /// Creates an ast::Function and registers it with the ast::Module.
  /// @param source the source information
  /// @param name the function name
  /// @param params the function parameters
  /// @param type the function return type
  /// @param body the function body
  /// @param decorations the function decorations
  /// @returns the function pointer
  ast::Function* Func(Source source,
                      std::string name,
                      ast::VariableList params,
                      type::Type* type,
                      ast::StatementList body,
                      ast::FunctionDecorationList decorations) {
    auto* func =
        create<ast::Function>(source, Symbols().Register(name), params, type,
                              create<ast::BlockStatement>(body), decorations);
    AST().AddFunction(func);
    return func;
  }

  /// Creates an ast::Function and registers it with the ast::Module.
  /// @param name the function name
  /// @param params the function parameters
  /// @param type the function return type
  /// @param body the function body
  /// @param decorations the function decorations
  /// @returns the function pointer
  ast::Function* Func(std::string name,
                      ast::VariableList params,
                      type::Type* type,
                      ast::StatementList body,
                      ast::FunctionDecorationList decorations) {
    auto* func =
        create<ast::Function>(Symbols().Register(name), params, type,
                              create<ast::BlockStatement>(body), decorations);
    AST().AddFunction(func);
    return func;
  }

  /// Creates a ast::StructMember
  /// @param source the source information
  /// @param name the struct member name
  /// @param type the struct member type
  /// @returns the struct member pointer
  ast::StructMember* Member(const Source& source,
                            const std::string& name,
                            type::Type* type) {
    return create<ast::StructMember>(source, Symbols().Register(name), type,
                                     ast::StructMemberDecorationList{});
  }

  /// Creates a ast::StructMember
  /// @param name the struct member name
  /// @param type the struct member type
  /// @returns the struct member pointer
  ast::StructMember* Member(const std::string& name, type::Type* type) {
    return create<ast::StructMember>(source_, Symbols().Register(name), type,
                                     ast::StructMemberDecorationList{});
  }

  /// Creates a ast::StructMember
  /// @param name the struct member name
  /// @param type the struct member type
  /// @param decorations the struct member decorations
  /// @returns the struct member pointer
  ast::StructMember* Member(const std::string& name,
                            type::Type* type,
                            ast::StructMemberDecorationList decorations) {
    return create<ast::StructMember>(source_, Symbols().Register(name), type,
                                     decorations);
  }

  /// Creates a ast::StructMember with the given byte offset
  /// @param offset the offset to use in the StructMemberOffsetDecoration
  /// @param name the struct member name
  /// @param type the struct member type
  /// @returns the struct member pointer
  ast::StructMember* Member(uint32_t offset,
                            const std::string& name,
                            type::Type* type) {
    return create<ast::StructMember>(
        source_, Symbols().Register(name), type,
        ast::StructMemberDecorationList{
            create<ast::StructMemberOffsetDecoration>(offset),
        });
  }

  /// Sets the current builder source to `src`
  /// @param src the Source used for future create() calls
  void SetSource(const Source& src) {
    AssertNotMoved();
    source_ = src;
  }

  /// Sets the current builder source to `loc`
  /// @param loc the Source used for future create() calls
  void SetSource(const Source::Location& loc) {
    AssertNotMoved();
    source_ = Source(loc);
  }

  /// Helper for returning the resolved semantic type of the expression `expr`.
  /// @note As the TypeDeterminator is run when the Program is built, this will
  /// only be useful for the TypeDeterminer itself and tests that use their own
  /// TypeDeterminer.
  /// @param expr the AST expression
  /// @return the resolved semantic type for the expression, or nullptr if the
  /// expression has no resolved type.
  type::Type* TypeOf(ast::Expression* expr) const;

  /// Wraps the ast::Expression in a statement. This is used by tests that
  /// construct a partial AST and require the TypeDeterminer to reach these
  /// nodes.
  /// @param expr the ast::Expression to be wrapped by an ast::Statement
  /// @return the ast::Statement that wraps the ast::Expression
  ast::Statement* WrapInStatement(ast::Expression* expr);
  /// Wraps the ast::Variable in a ast::VariableDeclStatement. This is used by
  /// tests that construct a partial AST and require the TypeDeterminer to reach
  /// these nodes.
  /// @param v the ast::Variable to be wrapped by an ast::VariableDeclStatement
  /// @return the ast::VariableDeclStatement that wraps the ast::Variable
  ast::VariableDeclStatement* WrapInStatement(ast::Variable* v);
  /// Returns the statement argument. Used as a passthrough-overload by
  /// WrapInFunction().
  /// @param stmt the ast::Statement
  /// @return `stmt`
  ast::Statement* WrapInStatement(ast::Statement* stmt);
  /// Wraps the list of arguments in a simple function so that each is reachable
  /// by the TypeDeterminer.
  /// @param args a mix of ast::Expression, ast::Statement, ast::Variables.
  template <typename... ARGS>
  void WrapInFunction(ARGS&&... args) {
    ast::StatementList stmts{WrapInStatement(std::forward<ARGS>(args))...};
    WrapInFunction(stmts);
  }
  /// @param stmts a list of ast::Statement that will be wrapped by a function,
  /// so that each statement is reachable by the TypeDeterminer.
  void WrapInFunction(ast::StatementList stmts);

  /// The builder types
  TypesBuilder const ty{this};

 protected:
  /// Asserts that the builder has not been moved.
  void AssertNotMoved() const;

 private:
  type::Manager types_;
  ASTNodeAllocator ast_nodes_;
  SemNodeAllocator sem_nodes_;
  ast::Module* ast_;
  semantic::Info sem_;
  SymbolTable symbols_;
  diag::List diagnostics_;

  /// The source to use when creating AST nodes without providing a Source as
  /// the first argument.
  Source source_;

  /// Set by SetResolveOnBuild(). If set, the TypeDeterminer will be run on the
  /// program when built.
  bool resolve_on_build_ = true;

  /// Set by MarkAsMoved(). Once set, no methods may be called on this builder.
  bool moved_ = false;
};

//! @cond Doxygen_Suppress
// Various template specializations for ProgramBuilder::TypesBuilder::CToAST.
template <>
struct ProgramBuilder::TypesBuilder::CToAST<ProgramBuilder::i32> {
  static type::Type* get(const ProgramBuilder::TypesBuilder* t) {
    return t->i32();
  }
};
template <>
struct ProgramBuilder::TypesBuilder::CToAST<ProgramBuilder::u32> {
  static type::Type* get(const ProgramBuilder::TypesBuilder* t) {
    return t->u32();
  }
};
template <>
struct ProgramBuilder::TypesBuilder::CToAST<ProgramBuilder::f32> {
  static type::Type* get(const ProgramBuilder::TypesBuilder* t) {
    return t->f32();
  }
};
template <>
struct ProgramBuilder::TypesBuilder::CToAST<bool> {
  static type::Type* get(const ProgramBuilder::TypesBuilder* t) {
    return t->bool_();
  }
};
template <>
struct ProgramBuilder::TypesBuilder::CToAST<void> {
  static type::Type* get(const ProgramBuilder::TypesBuilder* t) {
    return t->void_();
  }
};
//! @endcond

}  // namespace tint

#endif  // SRC_PROGRAM_BUILDER_H_
