# src/driver/wasm/ — WASM interpreter stub

**Status:** Placeholder — no WASM tree-walk interpreter exists here.

## What WASM execution looks like

SNOBOL4 → WASM runs via the **emit pipeline**, not an interpreter:

```
scrip --jit-emit --wasm <file.sno>   →  <file.wat>  (WAT text)
wat2wasm <file.wat>                  →  <file.wasm>
node test/wasm/run_wasm.js <file.wasm>
```

The runtime (`src/runtime/wasm/snobol4_runtime.wat`) provides the `sno`
namespace imports. It is pre-compiled to `src/runtime/wasm/sno_runtime.wasm`
by `make scrip`.

## Known limitations

- `EVAL()` / `CODE()` are not supported (dynamic compilation requires a
  runtime compiler, which WASM does not have in this pipeline).
- Keyword assignments (`&TRIM`, `&STLIMIT`, etc.) generate `global.set`
  but the globals are not declared — programs using them will fail `wat2wasm`.
  Workaround: remove keyword assignments before emitting.

## Future work

A WASM-hosted interpreter (`sno-interp.wasm`) would eliminate the two
limitations above. Tracked as M-JITEM-WASM in MILESTONE-SCRIP-X86-COMPLETION.md.
