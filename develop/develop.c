#include <stdio.h>
#include <stdlib.h>
#include "../dependencies/words.h"

int main(int argc, char * argv[]) {
    if (argc != 3) { return 0; }
    if (word_compare("add", argv[1]) == 0) {
        FILE* file;
        file = fopen(argv[2], "w");
        if(file == NULL) {
            printf("!! ERROR: Can not open file \"%s\"!\n", argv[2]);
            return -1;
        };
        char * string;
        // go to the end of the file
        fseek(file, 0, SEEK_END);
        int len = ftell(file);
        // go to the beginning of the file
        fseek(file, 0, SEEK_SET);
        string = malloc(len+1 * sizeof(char));
        fread(string, len, 1, file);
        fclose(file);
        // printf("%s\n", string);

        free(string);
    }
    return 0;
}