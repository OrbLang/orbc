#!/usr/bin/env bash

meson compile -C builddir

cp ./builddir/compile_commands.json .
