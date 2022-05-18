#pragma once

#ifdef __cplusplus
extern "C" {
#endif

int find(const char text[], const char search[]);
int findIndex(const char text[], const char search[], int * saveto);
int findWords(const int found[], int size, int searchNumber, int wordsFound, int words[][2]);
int replace(char text[], const char search[], const char repl[]);
int split(const char text[], const char search[], char *** saveto);
int join(const char ** wordArr, int words, const char search[], char saveto[]);
int replaceS(char text[], const char search[], const char repl[]);
int splitFirst(const char text[], const char search[], char *** saveto);
int replaceFirst(char text[], const char search[], const char repl[]);

void printIntArr(const int arr[], const int size);
void printDoubleArr(const double arr[], int size, const char after[2]);
void printWordArr(const char ** arr, int words);

void freeWordArr(char ** arr, int words);

void word_copy(char dest[], const char word[]);
int word_len(const char word[]);
int word_compare(const char word1[], const char word2[]);
void word_add(char dest[], const char word[]);
int zwischen(const char text[], const char word1[], const char word2[], char saveto[]);

int intLen(int zahl);
void intToString(int zahl, char * saveto);
void doubleToString(float zahl, int afterpoint, char * saveto);

unsigned int word_len_until(const char * word, const char * search);
const char * word_copy_until(char * dest, const char * word, const char * search);

#ifdef __cplusplus
}
#endif