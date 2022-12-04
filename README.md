# Welcome to the msh Interpreter
msh is programmining language I build myself. As I made it before I knew anything about tokens, grammar and stuff, it operates like that:

    replace anything you know - the rest is value / string

(e.g. You should never create the variable "y" -> the command "display" will not work afterwards)

# Installing
msh can be installed via homebrew.
Add the formulas:

    brew tap Max2github/msh

and install:

    brew install msh

Sometimes an error occors, saying that the xcode command line tools (CLI) are too old. Executing the command "softwareupdate ..." suggested by homebrew will make a softwareupdate of your system! Do not do this casually! Instead I recommend trying the command:

    sudo xcode-select --install

Alternatively you can also just download the executable (see releases -> Shell program).

# Building
Currently supports only macos or linux as host.

## Setting up the workspace
1. Make a directory named `msh` and go into it (`mkdir msh && cd msh`)
2. Copy the source code / clone it (`git clone https://github.com/Max2github/mshgit.git`)
3. Copy the source code / clone the repo msh-packages (from me) (`git clone https://github.com/Max2github/msh-packages.git`)
4. go out of the folder `msh` (`cd ..`)
5. clone the lib (from me) repo (`git clone https://github.com/Max2github/lib.git`)

If you choose to have a different layout, checkout the files `dependencies/std.h` and `dependencies/extern.h` and change the inlclude paths accordingly.

## adding packages (or commands)
Also look at the [wiki: Using devel: adding commands and packages](https://github.com/Max2github/mshgit/wiki/Using-devel-:--adding-commands-and-packages#to-add-a-package)

Nearly all commands in msh were moved to [msh-packages](https://github.com/Max2github/msh-packages), in order to separate system and commands.
You need to add them before compiling (there are no dynamic packages until now).

### Recommended packages:
#### basic
- std-essential - the basics
- std-extended - some usefull std functions (nearly essential)
#### should work (tested)
- IPsocket - basics to create a TCP/IP server
#### needs to be tested again
- list - linked lists
#### in BETA and / or does not work on every platform
- async - threading

## all.c vs. all.o
As of now you cannot all get the needed libraries, so I uploaded some precompiled .o files in the folder `other`.

    other/<your system>/all.o

Copy this file and put it in the folder:

    o/lib/

Now you should build with the follwing option:

    make dep=false ...

If it still doesn't work try removing the all.c file from the dependency directory (then make should not find it).

In std.h: If you define the macro `NO_STD_LIB`, you must declare a lot of othe macros
(something like `LIST_H_FREE(p) free(p)`), in which you give replacements for the "normal" standard functions. As I did not try this a lot, I cannot guarantee that it works.

If for some reason you have all libraries needed in all.c, then you just have to check the includes in all.c and you can compile without the `dep=false` argument.

## -) .o File: 
Go to the directory in Terminal:

    cd <Your Path>/mshgit

and build it:

    make objlib

Now you should have a file with the name
"msh.o". 
To include it in a c / c++ file:

    #include "mshgit/include/msh.h"

To build it afterwards:

    gcc -o <your executable> <your code files> mshgit/msh.o

## Cross-compiling
On macos (intel (x86) && m1 (arm64)) you can also cross-compile :)<br>
Type "make help" for more information.
Here a list of what is working until now:

### macos (arm64) (host)

- `windows_x86_32 | ✔` (currently there is a linking problem with the function inet_pton - only if you need socket communication)
- `windows_x86_64 | ✔`
- `macos_i386     | x`
- `macos_x86_64   | x`
- `linux_arm      | ✔` (I mean arm64, will change it later)
- `macos_x86_64   | x` (trick -> run terminal with rosetta)
- `linux_x86_64   | ✔`
- `linux_x86_32   | x`
- `linux_x86_i386 | x`

### linux (arm64) (host)

- `windows_x86_32 | ✔`
- `windows_x86_64 | ✔`
- `macos_i386     | x`
- `macos_x86_64   | x`
- `macos_x86_64   | x`
- `linux_x86_64   | ✔`
- `linux_x86_32   | ✔` (not tested yet)
- `linux_x86_i386 | ✔`

If you have a m1 mac, but want to compile for macos_x86_64, 
there is a little trick:
- in Finder: Applications -> search for Terminal
- duplicate the Terminal.app and rename the duplicate
- right click on the duplicate -> Information
-> set "open with rosetta"
- open the duplicate and type "arch" -> Enter -> now it should respond "i386"
- This duplicate Terminal.app is now on x86_64 and if you build the project there, it will build for macos_x86_64

For the cross-compiling tools check out the file [other/get-cross-compiler.md](other/get-cross-compiler.md).




