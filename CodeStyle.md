# Code Writing Style for C++

## Introduction

This document defines the code formatting and style rules for C++ to ensure uniformity, readability, and maintainability of the project. The guidelines are based on best practices and adapted to the project's needs.

**Mandatory:** All developers are required to follow the rules outlined here. Exceptions are permitted only with the approval of the project architect.

**Language Standard:** C++23
**Build Policy:** The project is compiled with **`-fno-exceptions`**. Throwing exceptions from project code is **prohibited**. See "Error Handling".

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

CODE
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
CODE

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
| Namespaces                   | **lower_case** (by agreement)   | `namespace my_project`                |
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
- `@note` – if the function reports failures via `spdlog::error` and returns a fallback value.

**Example:**

CODE
/// @brief Reads the contents of a file into a string.
/// @param filename Path to the file.
/// @return String with the file contents, or an empty string on failure.
/// @note On error, logs via spdlog::error and returns an empty string.
std::string ReadFileToString(const std::string& filename) noexcept;
CODE

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

CODE
// my_class.cpp
#include "my_class.h"

#include <algorithm>
#include <string>

#include "third_party/lib.h"
#include "project/utils.h"
CODE

### Namespaces
- Place all project code in a namespace matching the project name (e.g., `namespace my_project`).
- Do not use `using namespace` in header files (to avoid polluting the global namespace).
- In `.cpp` files, `using` for individual symbols is allowed.

---

## Error Handling

The project is built with **`-fno-exceptions`**. Any `throw` reached at runtime terminates the process via `std::terminate`; it does **not** unwind and is **not** catchable. Therefore:

- **Do not use `throw` / `try` / `catch` / `<stdexcept>` in project code.** This includes `std::optional::value()`, `std::variant::get()`, `std::vector::at()`, `std::map::at()`, `std::string::substr()` with invalid pos, and any other API whose failure path throws.
- **Do not use `.at()` / `.value()` / `.get()` on containers and `optional`/`variant`.** Under `-fno-exceptions` these call `std::terminate` (libstdc++) or throw and then terminate (libc++). Always do an explicit bounds check and use `operator[]` / `*optional` after the check.
- **Report failures via `spdlog::error`** (see Logging) and return a safe fallback:
  - numeric types → `0`;
  - vectors / strings → empty or zero-filled, depending on the contract of the call (document it in Doxygen);
  - `std::optional<T>` → `std::nullopt`;
  - objects with a validity flag (e.g., `BinaryFileHandler`) → expose `IsValid() noexcept` and document that every other method returns zero/empty when invalid.
- **Never silently swallow an error.** Every failed operation that produces a fallback value must produce one `spdlog::error` (or `spdlog::warn` when the fallback is part of the documented contract, e.g., zero-padding past EOF).
- **Library calls that may throw** (Boost.Asio callbacks, Diligent, Jolt) must be wrapped at the boundary: validate inputs before the call, and check return codes / error_codes afterwards. If a dependency exposes a throwing API without a non-throwing variant, isolate it behind a project wrapper that converts the failure into the project's non-throwing contract.
- **Mark non-throwing functions `noexcept`.** Anything that does not itself allocate in a way that could throw or call a throwing dependency should be `noexcept`. This is a contract, not a hint: a `noexcept` function must not let any `throw` escape, including from standard-library calls.
- **Streams:** if using `std::fstream` / `std::ifstream`, always call `stream.exceptions(std::ios::goodbit)` after obtaining the stream. Never rely on the default mask if the stream came from user code that may have changed it.

### Example: non-throwing read

CODE
/// @brief Reads exactly count bytes from the stream.
/// @param count Number of bytes requested.
/// @return Vector of exactly count bytes; real data followed by zeros on short read.
/// @note Non-throwing. Logs via spdlog::error on I/O failure or EOF; tail is zero-filled.
std::vector<std::uint8_t> ReadBytes(std::size_t count) noexcept
{
	std::vector<std::uint8_t> buffer(count, 0);
	if (count == 0) {
		return buffer;
	}

	const std::size_t available = m_readPos < m_size ? m_size - m_readPos : 0;
	const std::size_t toRead = std::min(count, available);

	if (toRead > 0) {
		m_file.read(reinterpret_cast<char*>(buffer.data()), static_cast<std::streamsize>(toRead));
		if (!m_file) {
			spdlog::error("ReadBytes: read failed at offset {} (requested {} of {}): {}",
				m_readPos, toRead, count, m_path);
			m_file.clear();
		}
	}

	if (toRead < count) {
		spdlog::error("ReadBytes: requested {} byte(s) at offset {}, only {} available, "
			"zero-padding {} byte(s): {}",
			count, m_readPos, toRead, count - toRead, m_path);
	}

	m_readPos += toRead;
	return buffer;
}
CODE

### Logging

- Use `spdlog` free functions (`spdlog::error`, `spdlog::warn`, `spdlog::info`, `spdlog::debug`, `spdlog::critical`). Do not construct loggers ad-hoc inside business logic.
- **Message format:** `<FunctionOrClass>: <description> (<context>)`.
  - Paths and identifiers that may contain spaces are quoted with `'{}'`.
  - Context (path, line, offset, id) — in parentheses at the end of the message.
- **Level semantics:**
  - `spdlog::error` — an operation failed; a fallback value is returned. Exactly one `error` per failure path.
  - `spdlog::warn` — recoverable deviation (e.g., name mismatch, missing optional field, EOF padding); execution continues.
  - `spdlog::info` — high-level lifecycle events, off by default in hot paths.
  - `spdlog::debug` — trace of successful paths; guarded with `if (spdlog::should_log(spdlog::level::debug))` if the message builds a string.
- **One module — one prefix.** Use the class or function name consistently within a translation unit.
- Never log and return a fallback **and** set an error flag that the caller is required to check on the same call — pick one contract per function and document it.

---

## C++ Specifics

- **Exceptions:** prohibited (see "Error Handling"). Do not use `throw`, `try`, `catch`, `<stdexcept>`, or APIs whose failure path throws.
- **`noexcept`:** mark every function that cannot fail or whose failure is reported via fallback + `spdlog`. Constructors that only set up state and log errors should also be `noexcept` where possible.
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
- Do not use `std::endl` – use `'\n'` for line breaks (endl flushes the buffer, reducing performance).

---

## Automation Tools

For style checking and formatting, use:

- **clang-format** with a configuration file (`.clang-format`). Settings must match this document.
- **clang-tidy** for static analysis and detecting potential issues. Enable at minimum:
  - `bugprone-exception-escape` — flags `noexcept` functions that may throw;
  - `bugprone-unchecked-optional-access`;
  - `cppcoreguidelines-pro-bounds-avoid-unchecked-container-access` is **not** suitable here — the project intentionally uses `operator[]` with manual checks, not `.at()`.
- **Build with warnings**: enable flags `-Wall -Wextra -Wpedantic` (or equivalents in MSVC) and fix all warnings.

Any deviations from this style must be justified and documented in the code with a `// NOLINT` comment (for clang-tidy) and approved by the team.