#!/bin/bash -e

# Copyright (C) 2019-21 Intel Corporation and others.  All rights reserved.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

TARGET="esp32"

if [[ -z "${WAMR_PATH}" ]]; then
        export WAMR_PATH=$PWD/../../..
fi

#rm -rf build
idf.py set-target $TARGET
idf.py build
idf.py flash