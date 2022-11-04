#include <stdio.h>
#include <stdlib.h>
#include "../dependencies/words.h"
// #include "../../../lib/word_pick_h.h"
#include "../../../lib/src/word_pick.h"

#if defined(WINDOWS) || defined(WIN32) || defined(_WIN32)
    #include <direct.h>
    #include <io.h>
    #define GetCurrentDir _getcwd
    #define PATH_SEP_CHAR '\\'
    #define PATH_SEP_STRING "\\"
    #define file_open(fileStruct, fileName, mode) fopen_s(fileStruct, fileName, mode)
#else
    #include <unistd.h>
    #define GetCurrentDir getcwd
    #define PATH_SEP_CHAR '/'
    #define PATH_SEP_STRING "/"
    #define file_open(fileStruct, fileName, mode) *(fileStruct) = fopen(fileName, mode)
#endif

#define PATH_PACKAGE_LIST "src/cparts/packages.txt"
#define PATH_COMMAND_LIST "src/cparts/commandList.txt"
#define PATH_COMMAND_DEF "src/cparts/command_def.h"
#define PATH_COMMAND_CUT_MAIN "src/cparts/readZeile_commandCut_main.txt"
#define PATH_COMMAND_CUT_SUB "src/cparts/readZeile_commandCut_sub.txt"
#define PATH_COMMAND_FUNC_MAIN "src/cparts/readZeile_commandFunc_main.txt"
#define PATH_COMMAND_FUNC_SUB "src/cparts/readZeile_commandFunc_sub.txt"

#define bool unsigned char
#define true 1
#define false 0

bool OPTION_FLAG_NOCOPY = false;

char File_getCurrentByte(FILE * file) {
    char b = fgetc(file);
    // file->_offset--; // do not advance offset
    fseek(file, -1, SEEK_CUR);
    return b;
}

void File_setCurrentByte(char b, FILE * file) {
    fputc(b, file);
    // file->_offset--; // do not advance offset
    fseek(file, -1, SEEK_CUR);
}

bool File_copy(const char * tar, const char * src) {
    FILE* srcFile = NULL;
    file_open(&srcFile, src, "r");
    FILE* tarAlready = NULL;
    file_open(&tarAlready, tar, "r");
    if(tarAlready == NULL && srcFile != NULL) {
        FILE* tarFile = NULL;
        file_open(&tarFile, tar, "w");
        int ch;
        while((ch = fgetc(srcFile)) != EOF) { fputc(ch, tarFile); };
        fclose(tarFile);
    } else  {
        fclose(tarAlready);
    }
    fclose(srcFile);
    return true;
}

// move all bytes after the bytes to delete forward by one position
bool File_deleteBytes(const  char * path, long startPos, int bytes) {
    char byte = 'n'; // our dummy buffer
    long readPos = startPos + bytes;
    FILE* file = NULL;
    file_open(&file, path, "r+");
    if (file == NULL) { return false; }

    fseek(file, 0, SEEK_END);
    const long fileEndPos = ftell(file);

    // read
    fseek(file, readPos, SEEK_SET);
    while((byte = fgetc(file)) != EOF /*fread(&byte, sizeof(byte), 1, file)*/) {
        readPos++;
        // readPos = ftell(file);

        // write
        fseek(file, startPos, SEEK_SET);
        // fwrite(&byte, sizeof(byte), 1, file);
        fputc(byte, file);
        startPos++;
        // startPos = ftell(file);
        // switch to reading
        fseek(file, readPos, SEEK_SET);
    }
#if defined(WINDOWS) || defined(WIN32) || defined(_WIN32)
    _chsize_s(_fileno(file), fileEndPos - bytes);
#else
    ftruncate(fileno(file), fileEndPos - bytes);
#endif
    fclose(file);
    return true;
}

bool File_insertBytes(const char * path, long startPos, const char * byteStream, int bytes) {
    FILE* file = NULL;
    file_open(&file, path, "r+");
    if (file == NULL) { return false; }

    fseek(file, 0, SEEK_END);
    long endPos = ftell(file);
    if (startPos > endPos) { startPos = endPos; }
    long bytesWritten = endPos - startPos;

    // set dummy at the end of the file if needed
    for (int i = 0; i < bytes; i++) {
        fputc(-1, file);
    }

    fseek(file, 0, SEEK_END);
    long endPosOld = endPos;
    endPos = ftell(file);
    printf("%ld %ld\n", endPosOld, endPos);
    long readPos = endPos - bytes - 1;
    long targetPos = endPos - 1;
    if (readPos < startPos) { readPos = startPos; }

    fseek(file, readPos, SEEK_SET);
    printf("w: %ld, n: %d, s:%c\n", bytesWritten, bytes, File_getCurrentByte(file));

    // move all backwards
    while(readPos >= startPos) {
        // read
        fseek(file, readPos, SEEK_SET);
        char byte = File_getCurrentByte(file);
        readPos--;
        // write
        fseek(file, targetPos, SEEK_SET);
        File_setCurrentByte(byte, file);
        // go back (we are reading backwards)
        targetPos--;
    }
    // write our new content
    fseek(file, startPos, SEEK_SET);
    fwrite(byteStream, bytes, 1, file);
    fclose(file);
    return true;
}

long File_read_until(char * target, const char * path, const long startPos, const char search) {
    FILE* file = NULL;
    file_open(&file, path, "r");
    if (file == NULL) { return 0; }

    fseek(file, startPos, SEEK_SET);
    long i = 0;
    while (true) {
        char buffer = fgetc(file);
        if (buffer == search || buffer == EOF) { return i; }
        *target++ = buffer;
        i++;
    }
    fclose(file);
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
    FILE* commandList = NULL;
    file_open(&commandList, temp, "a+");
    if (commandList == NULL) {
        printf("!! ERROR: Can not open file \"%s\"!\n", temp);
        return;
    }

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
            fclose(commandList);
            return;
        }

    fputs(nameMsh, commandList); fputc('\n', commandList);
    fclose(commandList);

    // add to definition (def)
    word_copy(temp, path);
    word_add(temp, def);
    FILE* def_custom = NULL;
    file_open(&def_custom, temp, "a");
    fputs("void ", def_custom); fputs(full, def_custom); fputs("(msh_info *);", def_custom); fputc('\n', def_custom);
    fclose(def_custom);

    // add to cut
    word_copy(temp, path);
    word_add(temp, cut);
    FILE* cut_main = NULL;
    file_open(&cut_main, temp, "a");
    fputs("replaceS(msh_Wert, \"", cut_main);
    fputs(nameMsh, cut_main);
    fputs("()\", \"\");\n", cut_main);
    fclose(cut_main);

    // add to func
    word_copy(temp, path);
    word_add(temp, func);
    FILE* func_main = NULL;
    file_open(&func_main, temp, "a");
    fputs("int found_", func_main); fputs(name, func_main); fputs(" = find(newZeile, \"", func_main); fputs(nameMsh, func_main); fputs("()\");\n", func_main);
    fputs("if (found_", func_main); fputs(name, func_main); fputs(" != 0) {\n", func_main);
    fputs("    ", func_main); fputs(full, func_main); fputs("(msh);\n", func_main);
    fputs("};\n", func_main);
    fclose(func_main);
}

void removeStatement(const char * path, const char * name, const char * toRemove) {
    char buffer[200] = "none";
    bool found = true;
    long i = 0;
    while (true) {
        long plusI = File_read_until(buffer, path, i, '\n');
        if (plusI == 0) { found = false; break; } // file is finished / EOF
        i += plusI;
        buffer[i+1] = '\0';
        if (word_compare(toRemove, buffer) == 0) { break; }
    }
    if (!found) {
        printf("Warning: Command %s is was not properly installed!\n", name);
        /* printf("rm: \"%s\"\n", toRemove);
        printf("buff: \"%s\"\n", buffer);
        printf("i: %ld\n\n", i); */
    } else {
        int len = word_len(buffer);
        // remove the statement + the newline afterwards
        File_deleteBytes(path, i-len, len+1);
    }
}

void removeC(const char * list, const char * def, const char * cut, const char * func, const char * name, const char * full) {
    char path[200];
    GetCurrentDir(path, 200);
    word_add(path, PATH_SEP_STRING);
    char temp[200];

    // get index and remove it from commandList
    word_copy(temp, path);
    word_add(temp, list);
    char nameMsh[word_len(name)+1];
    word_copy(nameMsh, name);
    replace(nameMsh, "_", "-"); // for names including '-' : in c '-' is an operator, in msh '_' is a space
    FILE* commandList = NULL;
    file_open(&commandList, temp, "r");
    if (commandList == NULL) {
        puts("Error: No commands installed!");
        fclose(commandList);
        return;
    }
    puts(name);

    // check where the command is + remove it
        // go to the end of the file
        fseek(commandList, 0, SEEK_END);
        int len = ftell(commandList);
        // go to the beginning of the file
        fseek(commandList, 0, SEEK_SET);
        char content[len+1];
        fread(content, len, 1, commandList);

        const char * reading = content;
        int found = false;
        int indexStart = 0;
        while (*reading != '\0') {
            int len = word_len_until(reading, "\n");
            char comm[len+1];
            reading = word_copy_until(comm, reading, "\n");
            if (word_compare(comm, nameMsh) == 0) {
                found = true;
                break;
            }
            indexStart += len;
        }
        if (!found) {
            printf("Error: Command %s is not installed!\n", nameMsh);
            fclose(commandList);
            return;
        }

    fclose(commandList);
    // delete from commandList + the newline afterwards
    File_deleteBytes(temp, indexStart, word_len(nameMsh)+1);

    // helper vars / buffers
    char toRemove[200];

    // delete from def
    temp[word_len(path)] = '\0';
    word_add(temp, def);
    word_copy(toRemove, "void "); word_add(toRemove, full); word_add(toRemove, "(msh_info *);");
    removeStatement(temp, nameMsh, toRemove);

    // delete from cut
    temp[word_len(path)] = '\0';
    word_add(temp, cut);
    word_copy(toRemove, "replaceS(msh_Wert, \""); word_add(toRemove, nameMsh); word_add(toRemove, "()\", \"\");");
    removeStatement(temp, nameMsh, toRemove);

    // delete from func
    temp[word_len(path)] = '\0';
    word_add(temp, func);
    word_copy(toRemove, "int found_"); word_add(toRemove, name); word_add(toRemove, " = find(newZeile, \""); word_add(toRemove, nameMsh); word_add(toRemove, "()\");");
    removeStatement(temp, nameMsh, toRemove);
    word_copy(toRemove, "if (found_"); word_add(toRemove, name); word_add(toRemove, " != 0) {");
    removeStatement(temp, nameMsh, toRemove);
    word_copy(toRemove, "    "); word_add(toRemove, full); word_add(toRemove, "(msh);");
    removeStatement(temp, nameMsh, toRemove);
    word_copy(toRemove, "};");

    // removeStatement(temp, nameMsh, toRemove);
}

void package_readConfig_C(const char * line) {
    word_picker_array spl = word_pick_split(line, " ");
    if (spl.written == 0) { return; }
    if (word_pick_compare(WORD_PICKER_ARRAY_GET(spl, 0), "")) {
        
    }
    SIMPLE_ARRAY_FREE(spl);
}
void package_readConfig_M(const char * line) {
    word_picker_array spl = word_pick_split(line, " ");
    if (spl.written == 0) { return; }
    word_picker keyword = WORD_PICKER_ARRAY_GET(spl, 0);

    if (spl.written >= 5 && word_pick_compare(keyword, "lib") == 0) {
        const char libpath[] = "../../lib/";
        word_picker key1 = WORD_PICKER_ARRAY_GET(spl, 1);
        word_picker val1 = WORD_PICKER_ARRAY_GET(spl, 2);
        word_picker key2 = WORD_PICKER_ARRAY_GET(spl, 3);
        word_picker val2 = WORD_PICKER_ARRAY_GET(spl, 4);

        if (word_pick_compare(key1, "-h")) {
            // header
        }
        // else if ...

        if (word_pick_compare(key2, "-c")) {
            // source
        }
        // else if ...
    } else if (spl.written >= 3 && word_pick_compare(keyword, "set") == 0) {
        word_picker key = WORD_PICKER_ARRAY_GET(spl, 1);
        word_picker value = WORD_PICKER_ARRAY_GET(spl, 2);

        // add to dependencies/std.h
        // #undef <key>
        // #define <key> <value>
    }
    SIMPLE_ARRAY_FREE(spl);
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

    FILE* config = NULL;
    file_open(&config, pathfull, "r");
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
        // check for special config
        if (*reading == '#') {
            len = word_len_until(reading, "\n");
            char line[len+1];
            word_copy_until(line, reading, "\n");
            package_readConfig_C(line);
            continue;
        }
        if (*reading == '-') {
            len = word_len_until(reading, "\n");
            char line[len+1];
            word_copy_until(line, reading, "\n");
            package_readConfig_M(line);
            continue;
        }
        // read regular line
        len = word_len_until(reading, ":");
        char type[len+1];
        reading = word_copy_until(type, reading, ":");
        len = word_len_until(reading, ":");
        char name[len+1];
        reading = word_copy_until(name, reading, ":");
        len = word_len_until(reading, ":");
        char src[len+1];
        reading = word_copy_until(src, reading, "\n");

        // remove blanks
        replaceS(type, " ", "");
        replaceS(name, " ", "");
        replaceS(src, " ", "");

        // execute regular line
        replace(name, "-", "_"); // for names including '-' : in c '-' is an operator, in msh '_' is a space
        char full[200] = "msh_command_";
        word_add(full, type);
        word_add(full, "_");
        word_add(full, name);

        if (word_compare("main", type) == 0) {
            add(PATH_COMMAND_LIST, PATH_COMMAND_DEF, PATH_COMMAND_CUT_MAIN, PATH_COMMAND_FUNC_MAIN, name, full);
            // if not left blank
            if (!OPTION_FLAG_NOCOPY && word_compare(src, "") != 0 && *src != '-') {
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
            if (!OPTION_FLAG_NOCOPY && word_compare(src, "") != 0 && *src != '-') {
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
        }
    }
    free(content);
}

void test() {
    File_insertBytes("develop/test.txt", 0, "Hello:", 6);
    File_insertBytes("develop/test.txt", 11, ", Goodbye!", 10);
    File_deleteBytes("develop/test.txt", 0, 6);
    File_insertBytes("develop/test.txt", 3, "-----c-----", 11);
}

bool handleOptionFlag(const char * option) {
    if (word_compare(option, "nocopy") == 0) { OPTION_FLAG_NOCOPY = true; return true; }
    return false;
}

bool handleOptionKeyValue(const char * option, const char * value) {
    return false;
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
    // if (argc != 3) { return 0; }
    if (argc == 0) { return 0; }
    int curArg = 1;
    for(;;) {
        if (*(argv[curArg]) == '-') {
            if (argv[curArg][1] == '-') {
                // if this is a valid option flag then skip the flag
                if (handleOptionFlag(argv[curArg]+2)) { curArg++; continue; };
            }
            // if this is a valid option flag then skip key and value
            if (handleOptionKeyValue(argv[curArg]+1, argv[curArg+1])) { curArg += 2; continue; };
        }
        break;
    }

    if (curArg > argc) { return 0; }

    if (word_compare("add", argv[curArg]) == 0) {
        // msh_command_<type>_<name>
        //  0      1      2      3
        char ** argArr;
        int argArrTeile = split(argv[curArg+1], "_", &argArr);
        if (argArrTeile != 3) {
            puts("Argument: Incorrect format! Abording!");
            freeWordArr(argArr, argArrTeile);
            return 0;
        }
        // printWordArr((const char **) argArr, argArrTeile);
        freeWordArr(argArr, argArrTeile);
        if (word_compare(argArr[2], "main") == 0) {
            add(PATH_COMMAND_LIST, PATH_COMMAND_DEF, PATH_COMMAND_CUT_MAIN, PATH_COMMAND_FUNC_MAIN, argArr[3], argv[curArg+1]);
        } else if (word_compare(argArr[2], "sub") == 0) {
            add(PATH_COMMAND_LIST, PATH_COMMAND_DEF, PATH_COMMAND_CUT_SUB, PATH_COMMAND_FUNC_SUB, argArr[3], argv[curArg+1]);
        }
    } else if (word_compare("package", argv[curArg]) == 0) {
        package(argv[curArg+1], PATH_PACKAGE_LIST);
    } else if (word_compare("remove", argv[curArg]) == 0) {
        // msh_command_<type>_<name>
        //  0      1      2      3
        char ** argArr;
        int argArrTeile = split(argv[curArg+1], "_", &argArr);
        if (argArrTeile != 3) {
            puts("Argument: Incorrect format! Abording!");
            freeWordArr(argArr, argArrTeile);
            return 0;
        }
        // printWordArr((const char **) argArr, argArrTeile);
        freeWordArr(argArr, argArrTeile);
        if (word_compare(argArr[2], "main") == 0) {
            removeC(PATH_COMMAND_LIST, PATH_COMMAND_DEF, PATH_COMMAND_CUT_MAIN, PATH_COMMAND_FUNC_MAIN, argArr[3], argv[curArg+1]);
        } else if (word_compare(argArr[2], "sub") == 0) {
            removeC(PATH_COMMAND_LIST, PATH_COMMAND_DEF, PATH_COMMAND_CUT_SUB, PATH_COMMAND_FUNC_SUB, argArr[3], argv[curArg+1]);
        }
    } else if (word_compare("removeP", argv[curArg]) == 0) {

    } else if (word_compare("test", argv[curArg]) == 0) {
        test();
    }

    return 0;
}