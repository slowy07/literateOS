# create toolchain

_unpack_binutils() {
  wget "ftpmirror.gnu.org/binutils/binutils-2.32.tar.gz"
  tar -xzvf binutils-2.32.tar.gz
    export BINUTILS_BUILD_DIR=binutils-2.32
}

_unpack_gcc() {
  wget "http://ftpmirror.gnu.org/gcc/gcc-8.2.0/gcc-8.2.0.tar.gz"
  tar -xzvf gcc-8.2.0.tar.gz
    export GCC_BUILD_DIR=gcc-8.2.0
    rm gcc-8.2.0.tar.gz
}

_build_binutils() {
  cd $BINUTILS_BUILD_DIR
  patch -p1 < ../literate-binutils-2.32.patch
  ./configure --target=x74_64-literate --prefix=$TOOLCHAIN_PREFIX --with-sysroot=$LITERATE_SYSROOT --disable-werror --enable-shared 
  make -j$JOBCOUNT
  make install
}

_build_gcc() {
  cd $GCC_BUILD_DIR
  patch -p1 < ../literate-gcc-8.2.0.patch
  ./configure --target=x86_64-literate --prefix=$TOOLCHAIN_PREFIX --with-sysroot=$LITERATE_SYSROOT --enable-language=c,c++ --enable-shared
  make all-gcc all-target-libgcc -j$JOBCOUNT
  make install-gcc install-target-libgcc
}

_build_libstdcpp() {
  cd $GCC_BUILD_DIR
  make all-target-libstdc++-v3 -j$JOBCOUNT
  make install-target-libstdc++-v3
}

_binutils() {
  _unpack_binutils
  _build_binutils
}

_gcc() {
  _unpack_gcc
  _build_gcc
}

_libstdcpp() {
  _build_libstdcpp
}

if [ -z "$LITERATE_SYSROOT" -o -z "$TOOLCHAIN_PREFIX" ]; then
  export TOOLCHAIN_PREFIX=$HOME/.local/share/literate
  export LITERATE_SYSROOT=$HOME/.local/share/literate/sysroot
  echo "LITERATE_SYSROOT or TOOLCHAIN_PREFIX not seet, continuing will use default:\nTOOLCHAIN_PREFIX: $TOOLCHAIN_PREFIX\nLITERATE_SYSROOT: $LITERATE_SYSROOT"
  read
fi

if [-z "$1" ]; then
  echo "Usage: $0 (binutils/gcc/libstdcpp)"
else
  _$1
fi
