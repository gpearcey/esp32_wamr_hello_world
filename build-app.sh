#!/bin/bash

CURR_DIR=$PWD
WASM_APPS=${PWD}/wasm-project

cd ${WASM_APPS}

rustup target add wasm32-wasi

RUSTFLAGS="-C link-arg=--strip-all" cargo build --release --target wasm32-wasi

cd target/wasm32-wasi/release

xxd -i wasm_project.wasm > wasm_project.wasm.h

mv wasm_project.wasm.h ../../../../esp-idf-project/main/


