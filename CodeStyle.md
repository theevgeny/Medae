# Code Writing Style for C++

## Introduction

This document defines the code formatting and style rules for C++ to ensure uniformity, readability, and maintainability of the project. The guidelines are based on best practices and adapted to the project's needs.

**Mandatory:** All developers are required to follow the rules outlined here. Exceptions are permitted only with the approval of the project architect.
**Language Standard:** C++23

---

## General Principles

- Code is written for people, not for the compiler. The priority is clarity and simplicity.
- Consistency is more important than personal preferences.
- Follow the Principle of Least Astonishment (POLA).
- Minimize complexity: prefer simple solutions over complex ones.
- Use the language features and the standard library instead of reinventing the wheel.

---

## Formatting

- Use **tabs** for indentation. Using spaces for indentation is prohibited.
- Indentation for blocks inside namespaces, classes, functions, loops, and conditionals is required.
- Maximum line length is **120 characters**. Exceptions: long lines in comments or macros, but they must be justified.
- The opening brace for a function, class, enumeration, namespace, etc. definition is placed on a **new line**.
- The opening brace for control constructs (`if`, `for`, `while`, `switch`) is placed on the **same line**, with a space before it.

**Example:**
```cpp
void MyFunction()
{
    // body
}

class MyClass
{
public:
    void Method();
};

if (condition) {
    DoSomething();
} else {
    DoOther();
}
```

- Put a space after keywords (`if`, `for`, `while`, `switch`, `return`, etc.).
- Do **not** put a space before the opening parenthesis in a function call (`foo(arg)`, not `foo (arg)`).
- In binary operations, put spaces around operators: `a + b`, `x == y`.
- Put a space after commas: `int a, b, c;`.
- Do not use spaces for alignment to line up comments or declarations in columns, as this makes maintenance harder. Prefer a single space instead.
- Use English for all identifiers.
- Names should be meaningful and reflect their purpose.
- Avoid abbreviations, except for commonly accepted ones (e.g., `i` for a counter, `ptr` for a pointer).

### Specific Naming Styles

| Entity                       | Style                           | Example                                |
|------------------------------|---------------------------------|---------------------------------------|
| Classes, structs, enums      | **PascalCase**                  | `class FileReader`                    |
| Functions, methods           | **PascalCase**                  | `void ReadFile()`                     |
| Local variables              | **camelCase**                   | `int fileSize`                        |
| Function parameters          | **camelCase**                   | `void SetName(const std::string& name)` |
| Global variables             | **camelCase** with prefix `g_`  | `int g_applicationCounter`            |
| Private class fields         | **camelCase** with prefix `m_`  | `int m_size;`                         |
| Public/protected fields      | **camelCase** (no prefix)       | `int count;` (if allowed)             |
| Constants (constexpr, const) | **kPascalCase**                 | `const int kMaxConnections = 100;`    |
| Macros                       | **UPPER_CASE_WITH_UNDERSCORES** | `#define MY_MACRO 1`                  |
| Namespaces                   | **lower_case** (by agreement)   | `namespace my_project`             |
| Template parameters          | **PascalCase** with prefix `T`  | `template <typename TData>`           |
| Static class members         | **camelCase** with prefix `s_`  | `static int s_instanceCount;`         |

**Explanations:**
- The prefix `g_` for global variables clearly indicates global scope and simplifies searching.
- The prefix `m_` for private class fields makes them easily distinguishable from local variables and parameters.

### Exceptions
- Iterators may have short names: `it`, `i`, `j`.
- Simple index variables in loops: `i`, `j`, `k`.

---

## Comments
- Use `//` for single-line comments. `/* */` is allowed only for multi-line blocks.
- Comments should explain **why** the code is written that way, not **what** it does (if the code is not obvious).
- For documenting public APIs, use **Doxygen** format (`///` or `/** */`).

### Doxygen Comment Requirements
All public classes, methods, enumerations, and important functions must contain comments with:
- `@brief` – a short description.
- `@param` – for each parameter.
- `@return` – for the return value (if not `void`).
- `@throw` – if the function can throw an exception.

**Example:**
```cpp
/// @brief Reads the contents of a file into a string.
/// @param filename Path to the file.
/// @return String with the file contents.
/// @throw std::runtime_error If the file cannot be opened.
std::string ReadFileToString(const std::string& filename);
```

### Code Organization
- Use `// TODO(your_name): description` for unfinished tasks.
- Use `// FIXME: description` for known bugs that need to be fixed.
- Every header file (`.h`, `.hpp`) must contain `#pragma once`.
- Include order in `.cpp` files:
  1. The corresponding header file (first).
  2. System headers.
  3. Library headers.
  4. Internal project headers.

**Example:**
```cpp
// my_class.cpp
#include "my_class.h"

#include <algorithm>
#include <string>

#include "third_party/lib.h"
#include "project/utils.h"
```

### Namespaces
- Place all project code in a namespace matching the project name (e.g., `namespace my_project`).
- Do not use `using namespace` in header files (to avoid polluting the global namespace).
- In `.cpp` files, `using` for individual symbols is allowed.

## C++ Specifics
- Exceptions are allowed and encouraged for error handling.
- Do not use exception specifications (deprecated). Use `noexcept` where appropriate (e.g., move operations).
- Use of `typeid` and `dynamic_cast` is allowed only in extreme cases (e.g., debugging). In main code, prefer polymorphism via virtual functions or the Visitor pattern.
- Prefer `std::unique_ptr` for exclusive ownership, and `std::shared_ptr` for shared ownership.
- Do not use raw pointers for resource ownership (except when working with low-level C code).
- For observers, use raw pointers or `std::weak_ptr` (when working with `shared_ptr`).
- Use STL containers by default (`std::vector`, `std::map`, `std::unordered_map`, etc.).
- For strings, use `std::string` and `std::string_view` for parameters where copying is not required.
- Avoid direct calls to `new` / `delete` – use smart pointers or containers.
- Use `auto` to improve readability when the type is obvious (e.g., iterators, complex types).
- Do not overuse `auto` if it makes the code harder to understand (e.g., function return values).
- Declare variables as `const` if they are not modified.
- Use `constexpr` for compile-time constants.
- Methods that do not change the object state must be `const`.
- Use C++-style casts: `static_cast`, `dynamic_cast`, `const_cast`, `reinterpret_cast`.
- Avoid C-style casts `(type)value` and functional-style casts `type(value)`.
- Always initialize variables when declaring them.
- Avoid undefined behavior (UB): signed integer overflow, out-of-bounds array access, dereferencing a null pointer, etc.
- For fixed-size arrays, use `std::array` instead of C-style arrays.
- Check container bounds – use `.at()` where the index may be invalid.
- Do not use `std::endl` – use `'\n'` for line breaks (endl flushes the buffer, reducing performance).

## Automation Tools

For style checking and formatting, use:

- **clang-format** with a configuration file (`.clang-format`). Settings must match this document.
- **clang-tidy** for static analysis and detecting potential issues.
- **Build with warnings**: enable flags `-Wall -Wextra -Wpedantic` (or equivalents in MSVC) and fix all warnings.

Any deviations from this style must be justified and documented in the code with a `// NOLINT` comment (for clang-tidy) and approved by the team.