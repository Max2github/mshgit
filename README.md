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

Copy the source code / clone it.
Check the files (mshgit/dependencies) "std.h" and "all.c". In "all.c" replace: 

    #include "../../../lib/list.h"
    #include "../../../lib/superstring.h"
    #include "../../../lib/mathe.h"

by their correct paths. If you do not have them get them (they are also from me).

As of now you cannot get the libraries needed, so I uploaded some precompiled .o files in the folder "other"

    other/<your system>/all.o

Copy this file and put it in the folder:

    o/lib/

Now you should build with the follwing option:

    make dep=false ...

If it still doesn't work try removing the all.c file from the dependency directory (then make should not find it).

In std.h: You can change the <stdio.h> and the <stdlib.h> libraries with other libraries, as long as you provide the functions needed. For the moment this feature will not be possible, because the libraries also need <stdio.h> and <stdlib.h>.

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

- `windows_x86_32??| ???`
- `windows_x86_64??| ???`
- `macos_i386??????????| x`
- `macos_x86_64??????| x`
- `linux_arm ???????? | ???`
- `macos_x86_64 ?? | x` (trick -> run terminal with rosetta)
- `linux_x86_64 ?? | ???`
- `linux_x86_32 ?? | x`

### linux (arm64) (host)

- `windows_x86_32??| ???`
- `windows_x86_64??| ???`
- `macos_i386??????????| x`
- `macos_x86_64??????| x`
- `macos_x86_64 ?? | x` (trick -> run terminal with rosetta, see below)
- `linux_x86_64 ?? | ???` (not tested yet)
- `linux_x86_32 ?? | ???` (not tested yet)

If you have a m1 mac, but want to compile for macos_x86_64, 
there is a little trick:
- in Finder: Applications -> search for Terminal
- duplicate the Terminal.app and rename the duplicate
- right click on the duplicate -> Information
-> set "open with rosetta"
- open the duplicate and type "arch" -> Enter -> now it should respond "i386"
- This duplicate Terminal.app is now on x86_64 and if you build the project there, it will build for macos_x86_64

For the cross-compiling tools check out the file [other/get-cross-compiler.md](other/get-cross-compiler.md).




