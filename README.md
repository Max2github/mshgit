# Welcome to the msh Interpreter
msh is programmining language I build myself. As I made it before I knew anything about tokens, grammar and stuff, it operates like that:

    replace anything you know - the rest is value / string

(e.g. You should never create the variable "y" -> the command "display" will not work afterwards)

# Building
Copy the source code / clone it.
Check the files (mshgit/dependencies) "std.h" and "all.c". In "all.c" replace: 

    #include "../../../lib/list.h"
    #include "../../../lib/superstring.h"
    #include "../../../lib/mathe.h"

by their correct paths. If you do not have them get them (they are also from me).

In std.h: You can change the <stdio.h> and the <stdlib.h> libraris with other libraries, as long as you provide the functions needed.

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

