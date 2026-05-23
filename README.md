# Arena Allocator

A simple, lightweight bump allocator in C, for learning purposes.

## Description

This project implements a linked-list arena from which memory chunks are
dispensed on demand. When a block is fully allocated, a new block is created and
linked. The eventual `arena_free` call will release all resources in one go.

To minimize internal fragmentation, **8‑byte alignment is applied only when the
padding waste is strictly less than the requested allocation size**.  
Small objects (1–7 bytes) are packed tightly; larger objects are always 8‑byte
aligned.

## Overall Design

- **Linked-list arena** – maintains a linked-list of arenas.
- **Bump allocation** – a single `cursor` advances after each request.
- **Conditional alignment**:
  - Compute the next 8‑byte boundary: `aligned = (cursor + 7) & ~7`.
  - If `aligned - cursor >= size`, skip alignment and place the block at
    `cursor` (tight packing).
  - Otherwise, use the aligned offset.
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
