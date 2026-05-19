# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Context

This is a C++ academic assignment for **CCP6124 OOPDS (Object-Oriented Programming & Data Structures)**, Term 2610. The assignment implements a **Virtual Machine** in C++. See `CCP6124 OOPDS Term 2610 Assignment - Virtual Machine.pdf` for the full specification.

## Build & Run

Compile any `.cpp` file with:
```bash
g++ -o output_name file.cpp && ./output_name
```

For C++17 features:
```bash
g++ -std=c++17 -o output_name file.cpp && ./output_name
```

There is no build system (no Makefile); compile individual files directly.

## Code Style

- Use `class` with `private`/`public` access modifiers for encapsulation
- `practice.cpp` demonstrates the expected OOP style: private member variables with public getter/setter methods
- No external libraries — standard library (`<iostream>`, `<string>`, `<vector>`, etc.) only
