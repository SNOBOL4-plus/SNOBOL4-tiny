#!/usr/bin/env node
// test/wasm/run_wasm.js — WASM runner shim for snobol4ever
// Usage: node run_wasm.js <prog.wasm>
// Contract: module exports { main, memory }
//   main() returns byte-length of output written to memory[0..]
//   stdout receives exactly those bytes (no trailing newline added here)

'use strict';

const fs = require('fs');
const path = require('path');

const wasmFile = process.argv[2];
if (!wasmFile) {
  process.stderr.write('usage: node run_wasm.js <prog.wasm>\n');
  process.exit(1);
}

const bytes = fs.readFileSync(wasmFile);

WebAssembly.instantiate(bytes).then(result => {
  const { main, memory } = result.instance.exports;
  if (typeof main !== 'function') {
    process.stderr.write('run_wasm.js: module does not export "main"\n');
    process.exit(1);
  }
  if (!memory) {
    process.stderr.write('run_wasm.js: module does not export "memory"\n');
    process.exit(1);
  }
  const len = main();
  if (len > 0) {
    process.stdout.write(Buffer.from(new Uint8Array(memory.buffer, 0, len)));
  }
}).catch(err => {
  process.stderr.write('run_wasm.js: ' + err.message + '\n');
  process.exit(1);
});
