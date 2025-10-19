# APItool4C

APItool is a tool for generating headers and printable documentation from a special .h.apitool format.

## Installation

To install APItool, you'll need CMake.

```
cmake -S src -B build
cmake --build build
cmake --install build
```

## Format

An APItool (.h.apitool) file is a list of double-newline seperated tokens.

I plan to create documentation for the format; however, the only thing you can do currently is look at `src/APItool.h.apitool` and the source files `parse_token.c`/`parse_specific.c`.

## Use

If you have nano, use it. This project provides syntax highlighting for .h.apitool files in nano.

To use APItool, run it like: `apitool4c {project name} {*.h.apitool} {*.h} {*.html}`.
