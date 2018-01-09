#include <cppx/meta>
// #include <cppx/compiler>

// #include <iostream>
// #include <typeinfo>

using namespace cppx;

int x1;
static int x2;

void f1() { }
void f2();
static inline void f3() { }
constexpr int f4() { return 0; }
void f5(int) = delete;

class C1 { };

class C2;

struct S1 {
  int x1;
  mutable int x2;
  static int x3;
  static constexpr int x4 = 0;
  int x5 : 4;

  void f1() { }

  class C { };
};

union U1 { };

enum E1 { X };
enum class E2 { X };
enum class E3 : int;

namespace N { 
  inline namespace M { }
}

int main(int argc, char* argv[]) {
  
  // Variables
  static_assert(std::is_same_v<decltype(reflexpr(x1)), meta::variable_info>);

  // int x1 -- global
  {
    constexpr auto r = reflexpr(x1);
    static_assert(kind(r) == meta::variable_decl);
    static_assert(has_external_linkage(r) == true);
    static_assert(has_static_storage(r) == true);
    static_assert(is_extern(r) == false);
    static_assert(is_static(r) == false);
    static_assert(is_inline(r) == false);
    static_assert(is_constexpr(r) == false);
  }

  // static int x2 -- global
  {
    constexpr auto r = reflexpr(x2);
    static_assert(kind(r) == meta::variable_decl);
    static_assert(has_internal_linkage(r) == true);
    static_assert(has_static_storage(r) == true);
    static_assert(is_extern(r) == false);
    static_assert(is_static(r) == true);
    static_assert(is_inline(r) == false);
    static_assert(is_constexpr(r) == false);
  }

  // Functions
  static_assert(std::is_same_v<decltype(reflexpr(f1)), meta::function_info>);

  // void f1() { }
  {
    constexpr auto r = reflexpr(f1);
    static_assert(kind(r) == meta::function_decl);
    static_assert(has_external_linkage(r) == true);
    static_assert(is_static(r) == false);
    static_assert(is_extern(r) == false);
    static_assert(is_constexpr(r) == false);
    static_assert(is_defined(r) == true);
    static_assert(is_inline(r) == false);
    static_assert(is_deleted(r) == false);
  }

  // void f2();
  {
    constexpr auto r = reflexpr(f2);
    static_assert(kind(r) == meta::function_decl);
    static_assert(has_external_linkage(r) == true);
    static_assert(is_static(r) == false);
    static_assert(is_extern(r) == false);
    static_assert(is_constexpr(r) == false);
    static_assert(is_defined(r) == false);
  }

  // static inline void f3() { }
  {
    constexpr auto r = reflexpr(f3);
    static_assert(kind(r) == meta::function_decl);
    static_assert(has_internal_linkage(r) == true);
    static_assert(is_static(r) == true);
    static_assert(is_extern(r) == false);
    static_assert(is_constexpr(r) == false);
    static_assert(is_defined(r) == true);
    static_assert(is_inline(r) == true);
    static_assert(is_deleted(r) == false);
  }

  // constexpr int f4() { return 0; }
  {
    constexpr auto r = reflexpr(f4);
    static_assert(kind(r) == meta::function_decl);
    static_assert(has_external_linkage(r) == true);
    static_assert(is_static(r) == false);
    static_assert(is_extern(r) == false);
    static_assert(is_constexpr(r) == true);
    static_assert(is_defined(r) == true);
    static_assert(is_inline(r) == true);
    static_assert(is_deleted(r) == false);
  }

  // void f5(int) = delete;
  {
    constexpr auto r = reflexpr(f5);
    static_assert(kind(r) == meta::function_decl);
    static_assert(has_external_linkage(r) == true);
    static_assert(is_static(r) == false);
    static_assert(is_extern(r) == false);
    static_assert(is_constexpr(r) == false);
    static_assert(is_defined(r) == true);
    static_assert(is_inline(r) == true); // FIXME: Apparently so.
    static_assert(is_deleted(r) == true);
  }

  // Namespaces
  
  static_assert(std::is_same_v<decltype(reflexpr(N)), meta::namespace_info>);
  
  // namespace N
  {
    constexpr auto r = reflexpr(N);
    static_assert(kind(r) == meta::namespace_decl);
    static_assert(is_inline(r) == false);
  }

  // inline namespace N::M
  {
    constexpr auto r = reflexpr(N::M);
    static_assert(kind(r) == meta::namespace_decl);
    static_assert(is_inline(r) == true);
  }

  // Classes

  static_assert(std::is_same_v<decltype(reflexpr(C1)), meta::class_info>);

  // class C1 { };
  {
    constexpr auto r = reflexpr(C1);
    static_assert(kind(r) == meta::class_decl);
    static_assert(has_external_linkage(r) == true);
    static_assert(has_access(r) == false);
    static_assert(is_class(r) == true);
    static_assert(is_complete(r) == true);
  }

  // class C2;
  {
    constexpr auto r = reflexpr(C2);
    static_assert(kind(r) == meta::class_decl);
    static_assert(has_external_linkage(r) == true);
    static_assert(has_access(r) == false);
    static_assert(is_class(r) == true);
    static_assert(is_complete(r) == false);
  }

  // struct S1 { ... };
  {
    constexpr auto r = reflexpr(S1);
    static_assert(kind(r) == meta::class_decl);
    static_assert(is_struct(r) == true);
  }

  // class S1::C { ... };
  {
    constexpr auto r = reflexpr(S1::C);
    static_assert(kind(r) == meta::class_decl);
    static_assert(has_access(r) == true);
    static_assert(is_public(r) == true);
  }
  
  // union U1 { ... };
  {
    constexpr auto r = reflexpr(U1);
    static_assert(kind(r) == meta::class_decl);
    static_assert(is_union(r) == true);
  }

  // Data members

  static_assert(std::is_same_v<decltype(reflexpr(S1::x1)), meta::data_member_info>);

  // int S1::x1;
  {
    constexpr auto r = reflexpr(S1::x1);
    static_assert(kind(r) == meta::data_member_decl);
    static_assert(is_public(r) == true);
    static_assert(is_static(r) == false);
    static_assert(is_mutable(r) == false);
    static_assert(is_inline(r) == false);
    static_assert(is_constexpr(r) == false);
    static_assert(is_bitfield(r) == false);
  }

  // mutable int S1::x2;
  {
    constexpr auto r = reflexpr(S1::x2);
    static_assert(kind(r) == meta::data_member_decl);
    static_assert(is_static(r) == false);
    static_assert(is_mutable(r) == true);
  }

  // static int S1::x3;
  {
    constexpr auto r = reflexpr(S1::x3);
    static_assert(kind(r) == meta::data_member_decl);
    static_assert(is_static(r) == true);
    static_assert(is_mutable(r) == false);
  }

  // static constexpr int S1::x4;
  {
    constexpr auto r = reflexpr(S1::x4);
    static_assert(kind(r) == meta::data_member_decl);
    static_assert(is_static(r) == true);
    static_assert(is_inline(r) == true);
    static_assert(is_constexpr(r) == true);
  }

  // int S1::x5 : 4;
  {
    constexpr auto r = reflexpr(S1::x5);
    static_assert(kind(r) == meta::data_member_decl);
    static_assert(is_static(r) == false);
    static_assert(is_bitfield(r) == true);
  }

  // Member functions

  static_assert(std::is_same_v<decltype(reflexpr(S1::f1)), meta::member_function_info>);

  // int S1::f1() { }
  {
    constexpr auto r = reflexpr(S1::f1);
    static_assert(kind(r) == meta::member_function_decl);
    static_assert(is_public(r) == true);
    static_assert(is_normal(r) == true);
    static_assert(is_static(r) == false);
    static_assert(is_constexpr(r) == false);
    static_assert(is_virtual(r) == false);
    static_assert(is_pure_virtual(r) == false);
    static_assert(is_override(r) == false);
    static_assert(is_final(r) == false);
    static_assert(is_defined(r) == true);
    static_assert(is_inline(r) == true); // in-class members are implicitly inline
    static_assert(is_deleted(r) == false);
  }

  // FIXME: Add more tests for member functions.

  // Enums

  static_assert(std::is_same_v<decltype(reflexpr(E1)), meta::enum_info>);

  // enum E1 { ... }
  {
    constexpr auto r = reflexpr(E1);
    static_assert(kind(r) == meta::enum_decl);
    static_assert(has_external_linkage(r) == true);
    static_assert(has_access(r) == false);
    static_assert(is_scoped(r) == false);
  }

  // enum class E2 { ... }
  {
    constexpr auto r = reflexpr(E2);
    static_assert(kind(r) == meta::enum_decl);
    static_assert(has_external_linkage(r) == true);
    static_assert(has_access(r) == false);
    static_assert(is_scoped(r) == true);
  }

  // enum class : int;
  {
    constexpr auto r = reflexpr(E3);
    static_assert(kind(r) == meta::enum_decl);
    static_assert(has_external_linkage(r) == true);
    static_assert(has_access(r) == false);
    static_assert(is_scoped(r) == true);
  }

  // Enumerators

  static_assert(std::is_same_v<decltype(reflexpr(X)), meta::enumerator_info>);

  // enum E1 { X }
  {
    constexpr auto r = reflexpr(X);
    static_assert(kind(r) == meta::enumerator_decl);
    static_assert(has_access(r) == false);
  }

  // enum class E2 { X }
  {
    constexpr auto r = reflexpr(E2::X);
    static_assert(kind(r) == meta::enumerator_decl);
    static_assert(has_access(r) == false);
  }

}
