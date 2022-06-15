# Cross-compiling
This is only for MacOS and Linux (here: Ubuntu)

# Errors while cross-compiling
When an executable (e.g aarch64-unknown-linux-gnu-gcc) is invoked for the first time, you may get the message that this App cannot be opened. If this is the case, search for the executable in the directory of toolchain. -> Right click -> "Open". Now your mac should ask you if you really want to open this -> Hit "Open". You can close the terminal / kill the program after that. Now it should work.

# MacOS

## windows x86_64 & x86_32:

    sudo port install mingw-w64

## aarch64-unknown-linux-gnu
Download the toolchain from here: (Direct Link)

    https://github.com/thinkski/osx-arm-linux-toolchains/releases/download/8.3.0/aarch64-unknown-linux-gnu.tar.xz

The homepage is:

    https://thinkski.github.io/osx-arm-linux-toolchains/

Important: Before cross-compiling you must add the compiler to your PATH:

    export PATH=$PATH:/<your path>/aarch64-unknown-linux-gnu/bin/

## x86_64-unknown-linux-gnu
Download the toolchain from here: (Direct Link)

    https://github.com/fafk/mac-to-linux-cross-toolchain/releases/download/v1.0/x86_64-unknown-linux-gnu.tar.gz

The homepage is:

    https://github.com/fafk/mac-to-linux-cross-toolchain

Important: Before cross-compiling you must add the compiler to your PATH:

    export PATH=$PATH:/<your path>/x86_64-unknown-linux-gnu/bin/

(For me it is:
    export PATH=$PATH:/Users/max/Downloads/x86_64-unknown-linux-gnu/bin/
)

# Linux (Ubuntu)

## windows x86_64 & x86_32:

    sudo apt install mingw-w64

## aarch64-linux-gnu

    sudo apt install binutils-aarch64-linux-gnu

## x86_64-linux-gnu && x86_64-linux-gnux32 (64-Bit && 32-Bit)

    sudo apt install x86_64-linux-gnux32-gcc


    x86_64-linux-gnu-gcc

