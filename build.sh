#!/usr/bin/env sh
# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
#

set -e
cmake -B build -S . -G Ninja "${@}"
ninja -C build -j $[$(nproc) * 2 + 2]
