# Welcome to the msh developping tools

comming soon

## Create your own msh commands
1. Make a new c file with the name: 

    msh_command_<i>`<type>`</i>_<i>`<name of your command>`</i>.c

The "type" can be either "main" or "sub".

2. The c file should look like this:

    #include "<i>`<your path>`</i>/include/msh.h"
    #include "<i>`<your path>`</i>/include/alg.h"
    #include "<i>`<your path>`</i>/dependencies/words.h"
    #include "<i>`<your path>`</i>/dependencies/std.h"

    void msh_command_<i>`<type>`</i>`_`<i>`<name of your command>`</i>() {
        ...
    }

3. Write your Code in the function. Please look at the this [comming soon]() for more information about using msh-intern (of the interpreter) functions and variables.

4. To add your code run:

    make action=add file=<i>`<path to your file>`</i> command

5. Now just build your code

    make host=<i>`<your host sytem>`</i> target=<i>`<your target system>`</i> ...

- The target option is only required for cross-compiling.
- The host option is only required for cross-compiling on non-MacOS.