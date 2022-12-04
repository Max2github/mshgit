# Additional files

## all.o -> my own libraries precomiped
these are up-to-date:
- linux_arm64
- linux_x86_64
- linux_x86_32
- linux_i386
- mac_arm
- mac_x86_64
- win32_mingw
- win64_mingw

these will soon be up-to-date:
- win32_clang-cl
- win64_clang-cl
- winARM64_clang-cl

### meaning:

#### linux_arm64
Also refered to as aarch64. For 64-bit GNU linux systems, having a CPU / Processor on ARM architecture.

#### linux_x86_64
The most common. For 64-bit GNU linux systems, having a CPU / Processor on x86 architecture.

#### linux_x86_32
Often refered to as linux X32. This is basically a x86_64 system, but using 32-Bit features (e.g. pointers), leading to an increase in performance speed. Also look at [this stackoverflow post](https://stackoverflow.com/questions/7635013/difference-between-x86-x32-and-x64-architectures).
For x86_64 GNU linux systems supporting X32.

#### linux_i386
Also known as x86 (32-bit). For 32-bit GNU linux systems, having a CPU / Processor on x86 architecture.

#### mac_arm
For MacOS with the m1 chip. The m1 chip is on architecture ARM and 64-bit.
I should change the name to `mac_arm64` to make it clear, it is for 64-bit.

#### mac_x86_64
For MacOS with an intel chip. For 64-bit (There are no recent MacOS for 32-bit).

#### win32_mingw
For x86 32-bit / i386 / x86 Windows systems. For 32-bit Windows on x86 architekture (nearly every windows is for architecture x86). Compiled using the mingw compiler and cannot be used with Microsofts CL, or Clang-CL!

#### win64_mingw
For 64-bit Windows on x86 architecture. Compiled with the mingw compiler!

#### win32_clang-cl
For 32-bit Windows on x86 architecture. Compiled with the clang-cl compiler!

#### win64_clang-cl
For 64-bit Windows on x86 architecture. Compiled with the clang-cl compiler!

#### winARM64_clang-cl
For 64-bit Windows on ARM architecture. Windows on ARM is very rare and new, so not a lot of people will need this. Furthermore x86 programs work in Windows on ARM too...<br>
Compiled with the clang-cl compiler!