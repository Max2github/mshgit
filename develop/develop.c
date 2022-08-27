#include <stdio.h>
#include <stdlib.h>
#include "../dependencies/words.h"

#ifdef WINDOWS
    #include <direct.h>
    #define GetCurrentDir _getcwd
    #define PATH_SEP_CHAR '\\'
    #define PATH_SEP_STRING "\\"
#else
    #include <unistd.h>
    #define GetCurrentDir getcwd
    #define PATH_SEP_CHAR '/'
    #define PATH_SEP_STRING "/"
#endif

#define PATH_PACKAGE_LIST "src/cparts/packages.txt"
#define PATH_COMMAND_LIST "src/cparts/commandList.txt"
#define PATH_COMMAND_DEF "src/cparts/command_def.h"
#define PATH_COMMAND_CUT_MAIN "src/cparts/readZeile_commandCut_main.txt"
#define PATH_COMMAND_CUT_SUB "src/cparts/readZeile_commandCut_sub.txt"
#define PATH_COMMAND_FUNC_MAIN "src/cparts/readZeile_commandFunc_main.txt"
#define PATH_COMMAND_FUNC_SUB "src/cparts/readZeile_commandFunc_sub.txt"

#define bool char
#define true 1
#define false 0

bool File_copy(const char * tar, const char * src) {
    FILE * srcFile = fopen(src, "r");
    FILE * tarAlready = fopen(tar, "r");
    if(tarAlready == NULL && srcFile != NULL) {
        FILE * tarFile = fopen(tar, "w");
        int ch;
        while((ch = fgetc(srcFile)) != EOF) { fputc(ch, tarFile); };
        fclose(tarFile);
    } else  {
        fclose(tarAlready);
    }
    fclose(srcFile);
    return true;
}

void add(const char * list, const char * def, const char * cut, const char * func, const char * name, const char * full) {
    // printf("%s\n", name);
    // get absolute path to current working directory
    char path[200];
    GetCurrentDir(path, 200);
    word_add(path, PATH_SEP_STRING);
    char temp[200];

    // add to commandList
    word_copy(temp, path);
    word_add(temp, list);
    char nameMsh[word_len(name)+1];
    word_copy(nameMsh, name);
    replace(nameMsh, "_", "-"); // for names including '-' : in c '-' is an operator, in msh '_' is a space
    FILE* commandList = fopen(temp, "a+");

    // check if the command already exists
        // go to the end of the file
        fseek(commandList, 0, SEEK_END);
        int len = ftell(commandList);
        // go to the beginning of the file
        fseek(commandList, 0, SEEK_SET);
        char content[len+1];
        fread(content, len, 1, commandList);

        const char * reading = content;
        int found = false;
        while (*reading != '\0') {
            int len = word_len_until(reading, "\n");
            char comm[len+1];
            reading = word_copy_until(comm, reading, "\n");
            if (word_compare(comm, nameMsh) == 0) { found = true; break; }
        }
        if (found) {
            printf("Error: Command %s is already installed!\n", nameMsh);
            return;
        }

    fputs(nameMsh, commandList); fputc('\n', commandList);
    fclose(commandList);

    // add to definition (def)
    word_copy(temp, path);
    word_add(temp, def);
    FILE* def_custom = fopen(temp, "a");
    fputs("void ", def_custom); fputs(full, def_custom); fputs("();", def_custom); fputc('\n', def_custom);
    fclose(def_custom);

    // add to cut
    word_copy(temp, path);
    word_add(temp, cut);
    FILE* cut_main = fopen(temp, "a");
    fputs("replaceS(msh_Wert, \"", cut_main);
    fputs(nameMsh, cut_main);
    fputs("()\", \"\");\n", cut_main);
    fclose(cut_main);

    // add to func
    word_copy(temp, path);
    word_add(temp, func);
    FILE* func_main = fopen(temp, "a");
    fputs("int found_", func_main); fputs(name, func_main); fputs(" = find(newZeile, \"", func_main); fputs(nameMsh, func_main); fputs("()\");\n", func_main);
    fputs("if (found_", func_main); fputs(name, func_main); fputs(" != 0) {\n", func_main);
    fputs("    ", func_main); fputs(full, func_main); fputs("();\n", func_main);
    fputs("};\n", func_main);
    fclose(func_main);
}

void package(const char * path, const char * list) {
    char dir[200];
    GetCurrentDir(dir, 200);
    char pathfull[200];
    word_copy(pathfull, dir); word_add(pathfull, PATH_SEP_STRING);
    word_add(pathfull, path);
    char pkgDir[200];
    word_copy(pkgDir, pathfull);
    word_add(pathfull, PATH_SEP_STRING); word_add(pathfull, "config.txt");

    FILE * config = fopen(pathfull, "r");
    if(config == NULL) {
        printf("!! ERROR: Can not open file \"%s\"!\n", pathfull);
    };

    char * content;
    // go to the end of the file
    fseek(config, 0, SEEK_END);
    int len = ftell(config);
    // go to the beginning of the file
    fseek(config, 0, SEEK_SET);
    content = malloc(len+1 * sizeof(char));

    fread(content, len, 1, config);
    fclose(config);

    const char * reading = content;
    while (*reading != '\0') {
        len = word_len_until(reading, ":");
        char type[len+1];
        reading = word_copy_until(type, reading, ":");
        len = word_len_until(reading, ":");
        char name[len+1];
        reading = word_copy_until(name, reading, ":");
        len = word_len_until(reading, ":");
        char src[len+1];
        reading = word_copy_until(src, reading, "\n");

        replaceS(type, " ", "");
        replaceS(name, " ", "");
        replaceS(src, " ", "");

        replace(name, "-", "_"); // for names including '-' : in c '-' is an operator, in msh '_' is a space
        char full[200] = "msh_command_";
        word_add(full, type);
        word_add(full, "_");
        word_add(full, name);

        if (word_compare("main", type) == 0) {
            add(PATH_COMMAND_LIST, PATH_COMMAND_DEF, PATH_COMMAND_CUT_MAIN, PATH_COMMAND_FUNC_MAIN, name, full);
            // if not left blank
            if (word_compare(src, "") != 0 && *src != '-') {
                // copy to src/Commands/main
                const char * r = src;
                while(find(r, PATH_SEP_STRING)) { r += word_len_until(r, PATH_SEP_STRING)+1; }
                char targetPath[200];
                word_copy(targetPath, dir); word_add(targetPath, PATH_SEP_STRING);
                word_add(targetPath, "src"); word_add(targetPath, PATH_SEP_STRING);
                word_add(targetPath, "Commands"); word_add(targetPath, PATH_SEP_STRING);
                word_add(targetPath, "main"); word_add(targetPath, PATH_SEP_STRING);
                word_add(targetPath, r);
                char srcFull[200];
                word_copy(srcFull, pkgDir); word_add(srcFull, PATH_SEP_STRING);
                word_add(srcFull, src);
                printf("copy: %s -> %s\n\n", srcFull, targetPath);
                File_copy(targetPath, srcFull);
            }
        } else if (word_compare("sub", type) == 0) {
            add(PATH_COMMAND_LIST, PATH_COMMAND_DEF, PATH_COMMAND_CUT_SUB, PATH_COMMAND_FUNC_SUB, name, full);
            // if not left blank
            if (word_compare(src, "") != 0 && *src != '-') {
                // copy to src/Commands/sub
                const char * r = src;
                while(find(r, PATH_SEP_STRING)) { r += word_len_until(r, PATH_SEP_STRING)+1; }
                char targetPath[200];
                word_copy(targetPath, dir); word_add(targetPath, PATH_SEP_STRING);
                word_add(targetPath, "src"); word_add(targetPath, PATH_SEP_STRING);
                word_add(targetPath, "Commands"); word_add(targetPath, PATH_SEP_STRING);
                word_add(targetPath, "sub"); word_add(targetPath, PATH_SEP_STRING);
                word_add(targetPath, r);
                char srcFull[200];
                word_copy(srcFull, pkgDir); word_add(srcFull, PATH_SEP_STRING);
                word_add(srcFull, src);
                printf("copy: %s -> %s\n\n", srcFull, targetPath);
                File_copy(targetPath, srcFull);
            }
        } else if (word_compare("list", type) == 0) {
            
        }
    }

    free(content);
}

/**
 * @brief 
 * format:
 * <prog_name> <action> <data>
 * @param prog_name name of this executable (e.g devel)
 * @param action e.g. "add", "package", ...
 * @param ---------------
 * @param data action == "add" : msh_command_<type>_<name>
 *      @param data::type e.g. "main", "sub", "package", ...
 *      @param data::name the name of your content
 * @param ---------------
 * @param data action == "package" : path to the package folder
 * @param ---------------
 * @return errorcode (int) - default 0
 */
int main(int argc, char * argv[]) {
    if (argc != 3) { return 0; }

    if (word_compare("add", argv[1]) == 0) {
        // msh_command_<type>_<name>
        //  0      1      2      3
        char ** argArr;
        int argArrTeile = split(argv[2], "_", &argArr);
        if (argArrTeile != 3) {
            puts("Argument: Incorrect format! Abording!");
            freeWordArr(argArr, argArrTeile);
            return 0;
        }
        // printWordArr((const char **) argArr, argArrTeile);
        freeWordArr(argArr, argArrTeile);
        if (word_compare(argArr[2], "main") == 0) {
            add(PATH_COMMAND_LIST, PATH_COMMAND_DEF, PATH_COMMAND_CUT_MAIN, PATH_COMMAND_FUNC_MAIN, argArr[3], argv[2]);
        } else if (word_compare(argArr[2], "sub") == 0) {
            add(PATH_COMMAND_LIST, PATH_COMMAND_DEF, PATH_COMMAND_CUT_SUB, PATH_COMMAND_FUNC_SUB, argArr[3], argv[2]);
        }
    } else if (word_compare("package", argv[1]) == 0) {
        package(argv[2], PATH_PACKAGE_LIST);
    } else if (word_compare("remove", argv[1]) == 0) {
        // msh_command_<type>_<name>
        //  0      1      2      3
        char ** argArr;
        int argArrTeile = split(argv[2], "_", &argArr);
        if (argArrTeile != 3) {
            puts("Argument: Incorrect format! Abording!");
            freeWordArr(argArr, argArrTeile);
            return 0;
        }
        // printWordArr((const char **) argArr, argArrTeile);
        freeWordArr(argArr, argArrTeile);
        if (word_compare(argArr[2], "main") == 0) {
            // add(PATH_COMMAND_LIST, PATH_COMMAND_DEF, PATH_COMMAND_CUT_MAIN, PATH_COMMAND_FUNC_MAIN, argArr[3], argv[2]);
        } else if (word_compare(argArr[2], "sub") == 0) {
            // add(PATH_COMMAND_LIST, PATH_COMMAND_DEF, PATH_COMMAND_CUT_SUB, PATH_COMMAND_FUNC_SUB, argArr[3], argv[2]);
        }
    }

    return 0;
}