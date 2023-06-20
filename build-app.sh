#!/bin/bash

cd wasm-project
rm -r target

CURR_DIR=$PWD
WASM_APPS=${PWD}/wasm-project

cd ${WASM_APPS}

rustup target add wasm32-wasi

RUSTFLAGS="-C link-arg=--strip-all -C link-arg=-zstack-size=8192 -C link-arg=--export=__heap_base -C link-arg=--export=__data_end" cargo build --release --target wasm32-wasi

cd target/wasm32-wasi/release

xxd -i wasm_project.wasm > test_wasm_rs.h

mv test_wasm_rs.h ../../../../esp-idf-project/main/


