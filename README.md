# Arena Allocator

A simple, lightweight bump allocator in C, for learning purposes.

## Description

This project implements a fixed‑size arena from which memory chunks are
dispensed on demand.

To minimize internal fragmentation, **8‑byte alignment is applied only when the
padding waste is strictly less than the requested allocation size**.  
Small objects (1–7 bytes) are packed tightly; larger objects are always 8‑byte
aligned.

## Overall Design

- **Singleton global arena** – initialized on the first allocation.
- **Bump allocation** – a single `cursor` advances after each request.
- **Conditional alignment**:
  - Compute the next 8‑byte boundary: `aligned = (cursor + 7) & ~7`.
  - If `aligned - cursor >= size`, skip alignment and place the block at
    `cursor` (tight packing).
  - Otherwise, use the aligned offset.
- **No free list** – allocations are permanent until the arena is destroyed.
- **Returns `NULL`** when the arena is exhausted.

## Dependencies

- **C compiler** (C99 or later)
- **[pkg-config](https://www.freedesktop.org/wiki/Software/pkg-config/)** – used
  to locate Criterion’s compile and link flags.
- **[Criterion](https://github.com/Snaipe/Criterion)** – unit testing framework.

### Installing Criterion & pkg‑config

**macOS (Homebrew):**
```bash
brew install criterion pkg-config
