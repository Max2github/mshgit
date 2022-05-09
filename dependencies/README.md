
These are just .h files of libraries I use (made myself). The all.c file is a shortcut to the loaction of their source code.

std.h is a shortcut for the std-libraries. If you want to use other std libraries you can edit this file. However following functions must be defined:

(for words.h lib)

- printf
- malloc
- free

(for Interpreter)

- sprintf - not anymore, but I should check if the workaround works in src/Commands/sub/normal.c:88
- putchar
- puts
- atoi
- atof
- realloc

The following two could be replaced by another function (like gets - it should read strings from stdin until newline '\n' - including spaces ' ') in src/Commands/sub/normal.c:110 in function msh_command_sub_input

- fgets
- stdin (macro)

Files

- FILE *  (type - perhaps struct?)
- fopen
- fseek
- ftell
- fread
- fclose
- SEEK_END (macro)
- SEEK_SET (macro)

If really want, you can remove the file functions by removing (or comment block) the "int msh_readFile(char filename[])" function in src/execute.c

For the moment the standard library time.h is still needed by mathe.h