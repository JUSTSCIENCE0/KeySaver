#!/bin/bash
set -e

ndk_root_option="$1"

externals_dir=$(pwd)/app/src/main/cpp/external
externals_src="$externals_dir"/sources
externals_android_arm64="$externals_dir"/android_arm64-v8a
externals_android_x86_64="$externals_dir"/android_x86_64
externals_linux_x86_64="$externals_dir"/linux_x86_64

find_android_ndk() {
    android_ndk_root="$HOME"/Android/Sdk/ndk
    
    if [ ! -d "$android_ndk_root" ]; then
        echo "ANDROID NDK not found"
        exit -1
    fi
    
    android_ndk_root=$(find "$android_ndk_root" -mindepth 1 -maxdepth 1 -type d | head -n 1)
    if [ -z "$android_ndk_root" ]; then
        echo "ANDROID NDK not found"
        exit -1
    fi
    
    echo "Found ANDROID NDK - $android_ndk_root"
    export ANDROID_NDK_ROOT="$android_ndk_root"
    PATH=$ANDROID_NDK_ROOT/toolchains/llvm/prebuilt/linux-x86_64/bin:$PATH
}

download_sources_archive() {
    sources_url="$1"
    package_name="$2"
    package_archive="$externals_src"/"$package_name".tar.gz
    
    wget -P "$externals_src" "$sources_url"
    tar -xzvf "$package_archive" -C "$externals_src"
    rm "$package_archive"
}

download_sources_git() {
    git_url="$1"
    git_version="$2"
    
    pushd "$externals_src"
        git clone --branch "$git_version" --depth 1 --recurse-submodule "$git_url"
    popd
}

build_openssl() {
    arch="$1"
    path="$2"
    
    ./Configure "$arch" -D__ANDROID_API__=24 no-apps no-docs no-shared no-tests --prefix="$path"/openssl --openssldir="$path"/openssl/ssl
    make
    make install_sw
    make clean
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

if [ -z "$ndk_root_option" ]; then
    find_android_ndk
else 
    export ANDROID_NDK_ROOT="$ndk_root_option"
    PATH=$ANDROID_NDK_ROOT/toolchains/llvm/prebuilt/linux-x86_64/bin:$PATH
fi

if [ -d "$externals_dir" ]; then
    rm -rf "$externals_dir"
fi

mkdir "$externals_dir"
mkdir "$externals_src"
mkdir "$externals_android_arm64"
mkdir "$externals_android_x86_64"

# download openssl
openssl_package=openssl-3.4.0
openssl_url=https://github.com/openssl/openssl/releases/download/"$openssl_package"/"$openssl_package".tar.gz
openssl_dir="$externals_src"/"$openssl_package"
download_sources_archive "$openssl_url" "$openssl_package"

# build openssl
pushd "$openssl_dir"
    build_openssl android-arm64 "$externals_android_arm64"
    build_openssl android-x86_64 "$externals_android_x86_64"
popd

# download protobuf
protobuf_version=v29.3
protobuf_url=https://github.com/protocolbuffers/protobuf.git
download_sources_git "$protobuf_url" "$protobuf_version"

# build protobuf
build_protobuf arm64-v8a "$externals_android_arm64"  android
build_protobuf x86_64    "$externals_android_x86_64" android
build_protobuf x86_64    "$externals_linux_x86_64"   linux

