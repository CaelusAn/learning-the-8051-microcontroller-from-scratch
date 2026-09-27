# 8051 C Coding Style

This repository follows one consistent C style for all 8051 projects.

## Core Rules

- Write code for people first. Prefer clarity over clever expressions.
- Keep one clear responsibility per file and function.
- Keep the same style across every project.

## Naming

- File names use lowercase `snake_case`.
- Variables use `lowerCamelCase`.
- Functions use `PascalCase`.
- Macros and compile-time constants use `UPPER_SNAKE_CASE`.
- Use English names instead of pinyin.

## Formatting

- Use four spaces per indentation level. Do not mix spaces and tabs.
- Use Allman braces: opening braces start on a new line.
- Put spaces around binary operators.
- Put one space after each comma.
- Do not put a space between a function name and its opening parenthesis.

Example:

```c
if (keyValue != MATRIX_KEY_NONE)
{
    ProcessKey(keyValue);
}
```

## Comments

- Every `.c` and `.h` file starts with a file header containing `File`,
  `Brief`, `Author`, and `Date`.
- Public functions use English Doxygen-style comments with `@brief`, `@param`,
  and `@retval`.
- Inline comments must explain intent, not repeat the code.

## Headers

- Every header uses an include guard.
- Headers contain declarations and interface constants only.
- A header includes the dependencies needed by its own declarations.
- Only `public.h` includes `<REGX52.H>`. Other files include `public.h` or a
  module header that includes it.

## Functions

- Use `void` for an empty parameter list and for functions with no return value.
- Keep functions focused. Split a function when it exceeds roughly 50 lines.
- Avoid unnecessary global variables and `extern` declarations.

## 8051 Embedded Rules

- Define `u8`, `u16`, and `u32` once in `public.h`.
- Use `u8`, `u16`, and `u32` consistently instead of mixing the raw types.
- Prefer bit-addressed writes or masks when changing individual port pins.
- Use generic delay functions such as `DelayMs(u16 milliseconds)` and document
  the assumed clock frequency.
- Replace unexplained magic numbers with named constants.

## Git Commits

Use Conventional Commits:

```text
<type>: <subject>
```

Common types are `feat`, `fix`, `docs`, `style`, `refactor`, and `test`.
