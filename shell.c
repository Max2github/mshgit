#include "dependencies/std.h"
#include "errno.h"

#include "include/msh.h"

#include "dependencies/extern.h"

#if PF_WINDOWS
    #include <direct.h>
    #define GetCurrentDir _getcwd
#elif PF_UNIX
    #include <unistd.h>
    #define GetCurrentDir getcwd
    #include <signal.h>
#endif

#define DEFAULT_MAIN "main.msh"

// bitflags !!
enum msh_shell_options_e {
    // set1
    msh_shell_option_none = 0,
    msh_shell_option_version,
    msh_shell_option_help,
    msh_shell_option_verboose = 4,
};

struct msh_shell_options {
    unsigned char set1;
    unsigned char number;
};
typedef struct msh_shell_options msh_shell_options;

#define MSH_SHELL_OPTIONS_DEFAULT { msh_shell_option_none, 0 }

bool msh_shell_handleOptionFlag(const char * option, msh_shell_options * optout) {
    if (word_compare(option, "version") == 0) { optout->set1 |= msh_shell_option_version; return true; }
    if (word_compare(option, "help") == 0) { optout->set1 |= msh_shell_option_help; return true; }
    if (word_compare(option, "verboose") == 0) { optout->set1 |= msh_shell_option_verboose; return true; }
    return false;
}

bool msh_shell_handleOptionKeyValue(const char * option, const char * value, msh_shell_options * optout) {
    return false;
}

msh_shell_options msh_shell_parse_options(int argc, char * argv[]) {
    msh_shell_options opt = MSH_SHELL_OPTIONS_DEFAULT;
    int curArg = 1;
    for(; curArg < argc;) {
        if (*(argv[curArg]) == '-') {
            if (argv[curArg][1] == '-') {
                // if this is a valid option flag then skip the flag
                if (msh_shell_handleOptionFlag(argv[curArg]+2, &opt)) { curArg++; continue; };
            }
            // if this is a valid option flag then skip key and value
            if (msh_shell_handleOptionKeyValue(argv[curArg]+1, argv[curArg+1], &opt)) { curArg += 2; continue; };
        }
        break;
    }
    opt.number = curArg-1;
    return opt;
}

void msh_shell_sigint(int signal) {
    putchar('\n');
    // 80 times - you can count if you want to :)
    puts("################################################################################");
    puts("                            TERMINATING BY INTERUPT                             ");
    puts("################################################################################");

    // send exit event to all running instances / msh_info's
    MSH_VERBOOSE_LOG(NULL, "send exit event to all running instances");
    msh_exec_event_exitAll();

    // close all open sockets
    MSH_VERBOOSE_LOG(NULL, "close all open sockets");
    msh_socket_closeAll();
}

int main(int argc, char * argv[]) {
    #if PF_UNIX
        signal(SIGINT, msh_shell_sigint);
    #elif PF_WINDOWS

    #endif

    msh_shell_options opt = msh_shell_parse_options(argc, argv);

    if (opt.set1 & msh_shell_option_version) {
        puts("3.0.0");
        return 0;
    }
    if (opt.set1 & msh_shell_option_verboose) {
        MSH_FLAGS_BITWISE1 |= msh_flagbit1_verboose;
    }

    int restArg = argc - opt.number;
    opt.number++;
    // printf("restArg: %d\noptNum: %d\nargc: %d\n", restArg, opt.number, argc);

    if (restArg > 2) { puts("!! Error : too many arguments"); return 1; }
    // if(word_compare(argv[1], "help") == 0) {  }
    char path[200];
    GetCurrentDir(path, 200);
    word_add(path, "/");
    if (restArg == 2) { word_add(path, argv[opt.number]); }
    else { word_add(path, DEFAULT_MAIN); }
    msh_readFile(path);
    
    // char c = getchar(); // don't close - just let the user see it for a bit...
    return 0;
}