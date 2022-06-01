#include "include/msh.h"
#include <stdio.h>
#include "dependencies/words.h"

#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

#define DEFAULT_MAIN "main.msh"

int main(int argvc, char * argv[]) {
    if (argvc > 2) { puts("!! Error : too many arguments"); return 1; }
    // if(word_compare(argv[1], "help") == 0) {  }
    char path[200];
    GetCurrentDir(path, 200);
    word_add(path, "/");
    if (argvc == 2) { word_add(path, argv[1]); }
    else { word_add(path, DEFAULT_MAIN); }
    msh_readFile(path);
    
    // char c = getchar(); // don't close - just let the user see it for a bit...
    return 0;
}