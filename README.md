# Arena Allocator

A simple, lightweight bump allocator in C, for learning purposes.

## Description

This project implements a linked-list arena from which memory chunks are
dispensed on demand. When a block is fully allocated, a new block is created and
linked. The eventual `arena_free` call will release all resources in one go.

## Overall Design

- **Linked-list arena** – maintains a linked-list of arenas.
- **Bump allocation** – a single `cursor` advances after each request.
- **Memory alignment** - aligning memory based on `max_align_t`.
- **No free list** – allocations are permanent until the arena is destroyed.
- **Returns `NULL`** when the requested size is larger than a single arena block.

## Dependencies

- **C compiler** (C11 or later)
- **[Criterion](https://github.com/Snaipe/Criterion)** – unit testing framework.
- **[pkg-config](https://www.freedesktop.org/wiki/Software/pkg-config/)** – used
  to locate Criterion’s compile and link flags.
- **[Bear](https://github.com/rizsotto/bear)** - generates `compile_commands.json`
  file for the clangd LSP.

### Installing Criterion & pkg‑config

**macOS (Homebrew):**
```bash
brew install criterion pkg-config bear
