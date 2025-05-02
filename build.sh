#!/bin/bash
set -e

BUILD_DIR="build"
CMAKE_PRESET="linux-release"
ARTIFACTS_DIR="artifacts"

if [ -d "$BUILD_DIR" ]; then
    echo "Remove old build directory"
    rm -rf "$BUILD_DIR"
fi

if [ -d "$ARTIFACTS_DIR" ]; then
    echo "Remove old artifacts directory"
    rm -rf "$ARTIFACTS_DIR"
fi

echo "Configure project"
cmake --preset "$CMAKE_PRESET"
echo "Build project"
cmake --build --preset "build-$CMAKE_PRESET" --parallel "$(nproc)"

mkdir -p "$ARTIFACTS_DIR"

echo "Copy artifacts"
cp "$BUILD_DIR/release/desktop/keysaver-desktop" "$ARTIFACTS_DIR/"
cp "$BUILD_DIR/release/desktop/en_US.qm" "$ARTIFACTS_DIR/"
cp "$BUILD_DIR/release/desktop/ru_RU.qm" "$ARTIFACTS_DIR/"
cp "$BUILD_DIR/release/app/src/main/cpp/libkeysaver-core.so" "$ARTIFACTS_DIR/"

echo "SUCCESS!"

