#!/bin/bash

set -e

# === Config ===
TARGET=i686-elf
PREFIX="$HOME/opt/cross"
PATH="$PREFIX/bin:$PATH"
BINUTILS_VER=2.42
GCC_VER=13.2.0
JOBS=$(nproc)

# === Directories ===
mkdir -p ~/src
cd ~/src

echo "Downloading binutils and GCC..."
# === Download Sources ===
wget -nc https://ftp.gnu.org/gnu/binutils/binutils-$BINUTILS_VER.tar.xz
wget -nc https://ftp.gnu.org/gnu/gcc/gcc-$GCC_VER/gcc-$GCC_VER.tar.xz

tar -xf binutils-$BINUTILS_VER.tar.xz
tar -xf gcc-$GCC_VER.tar.xz

# === Build Binutils ===
mkdir -p build-binutils
cd build-binutils
../binutils-$BINUTILS_VER/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make -j$JOBS
make install
cd ..

# === Build GCC (C only) ===
cd gcc-$GCC_VER
./contrib/download_prerequisites
cd ..
mkdir -p build-gcc
cd build-gcc
../gcc-$GCC_VER/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c --without-headers
make all-gcc -j$JOBS
make all-target-libgcc -j$JOBS
make install-gcc
make install-target-libgcc
cd ~

# === Add to PATH (permanently) ===
SHELL_RC="$HOME/.bashrc"
if [[ "$SHELL" == *zsh ]]; then
  SHELL_RC="$HOME/.zshrc"
fi

if ! grep -q 'opt/cross/bin' "$SHELL_RC"; then
  echo "export PATH=\"\$HOME/opt/cross/bin:\$PATH\"" >> "$SHELL_RC"
  echo "Added cross-compiler to $SHELL_RC"
fi

echo ""
echo "✅ Done! You can now use i686-elf-gcc"
echo "👉 Run 'source $SHELL_RC' or restart your terminal."
echo "👉 Test with: i686-elf-gcc --version"

