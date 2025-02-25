#!/bin/bash
set -e

ndk_root_option="$1"

externals_dir=$(pwd)/external
externals_src="$externals_dir"/sources
externals_linux_x86_64="$externals_dir"/linux_x86_64

download_sources_git() {
    git_url="$1"
    git_version="$2"
    
    pushd "$externals_src"
        git clone --branch "$git_version" --depth 1 --recurse-submodule "$git_url"
    popd
}

build_protobuf() {
    arch="$1"
    path="$2"
    platform="$3"
    
    protobuf_build_dir="$externals_src"/protobuf/build
    mkdir "$protobuf_build_dir"
    pushd "$protobuf_build_dir"
        if [[ "$platform" == "android" ]]; then
            cmake -G "Unix Makefiles" \
                  -DCMAKE_TOOLCHAIN_FILE="$ANDROID_NDK_ROOT"/build/cmake/android.toolchain.cmake \
                  -DCMAKE_MAKE_PROGRAM="$ANDROID_NDK_ROOT"/prebuilt/linux-x86_64/bin/make \
                  -DANDROID_ABI="$arch" \
                  -DANDROID_PLATFORM=android-24 \
                  -Dprotobuf_BUILD_TESTS=OFF \
                  -Dprotobuf_BUILD_PROTOC_BINARIES=OFF \
                  -DCMAKE_BUILD_TYPE=Release \
                  ..
        else
            cmake -DCMAKE_BUILD_TYPE=Release \
                  -Dprotobuf_BUILD_TESTS=OFF \
                  ..
        fi
          
        cmake --build . -- -j$(nproc)
        cmake --install . --prefix "$path"/protobuf	
    popd
    rm -rf "$protobuf_build_dir"
}

if [ -d "$externals_dir" ]; then
    rm -rf "$externals_dir"
fi

mkdir "$externals_dir"
mkdir "$externals_src"

# download protobuf
protobuf_version=v29.3
protobuf_url=https://github.com/protocolbuffers/protobuf.git
download_sources_git "$protobuf_url" "$protobuf_version"

# build protobuf
build_protobuf x86_64 "$externals_linux_x86_64" linux
