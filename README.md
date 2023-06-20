# esp32_wamr_hello_world
Hello World example for esp32 with ESP-IDF and wasm-micro-runtime.

The ESP-IDF project is written in C, which reads a binary Wasm file. A rust program compiled to a .wasm file calls a C function to print "Hello World #11". 

## Requirements
### ESP-IDF
Follow [this](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/) guide to setup ESP-IDF to build and flash your esp32 board. This poject was completed with esp version 5.0.2. Other versions are likely compatible, but not tested. 

### Rust
The hosted code in this example was written in Rust. You can get started with Rust [here](https://www.rust-lang.org/learn/get-started). 

You will also need the following to compile rust to .wasm files:
* wasm-gc 
  * `$ cargo install wasm-gc`
* wasi-sdk
  * Available from the  [release page](https://github.com/WebAssembly/wasi-sdk/releases)


### WAMR - WebAssembly Micro Runtime
[WebAssembly Micro Runtime](https://github.com/bytecodealliance/wasm-micro-runtime/tree/main) is a lightweight WASM runtime with a small footprint, high performance, and can be used on embedded systems, such as an esp32. 

You can find out how to set up WAMR from their [documentation](https://wamr.gitbook.io/document/basics/getting-started).

## Instructions

Add this line with your path to wamr to your .bashrc to set your WAMR_PATH variable

`export WAMR_PATH=<path-to-wasm-micro-runtime>`

Update your bashrc:

`$ source .bashrc`

### Compile lib.rs
Navigate to top level directory and run

`$ ./build-app.sh`

This compiles lib.rs to a WebAssembly binary file, then into a hex format that is loaded into main.c.

### Flash the esp32
Navigate to esp-idf-project:

`$ cd esp-idf-project/`

Build the program and flash the board:

`$ ./build_idf.sh`
  
Additional instructions for building ESP-IDF projects can be found [here](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/#build-your-first-project).

## File Structure

```
esp32_wamr_hello_world
├── esp-idf-project
│   ├── main
│   │   ├── CMakeLists.txt
|   |   └── main.cpp
│   ├── build_idf.sh
│   └── CMakeLists.txt
├── wasm-project
│   ├── src
|   |   └── lib.rs
│   └── cargo.toml
├── .gitignore
├── build-app.sh
└── README.md
```