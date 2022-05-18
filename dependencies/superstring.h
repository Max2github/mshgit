#pragma once
/**
 * This library defines 4 new datatypes:
 * 1.) superstring
 *      -) an alternative to a "normal" string
 *      -) saved as a linked list
 * 2.) s_element
 *      -) one node of the superstring
 * 3.) s_arr
 *      -) a list consisting of superstrings / like an Array of superstrings, but saved as a linked list
 *      -) an alternative to a "wordArr" (see words.h) / a 2D-char-Array
 *      -) saved as a linked list
 * 4.) s_arr_element
 *      -) one node of an s_arr
 * */

#ifdef __cplusplus
extern "C" {
#endif

// one s_element saves a char and how often it occurs at this position
// -> "lll" will be saved as c = 'l', anzahl = 3
// like an Array in C, a superstring is only a pointer to the first element
struct superstring_element {
    char c;
    int anzahl;
    struct superstring_element * next;
};
typedef struct superstring_element s_element;
typedef s_element * superstring;

struct s_arr_element {
    superstring element;
    struct s_arr_element * next;
};
typedef struct s_arr_element s_arr_element;
typedef s_arr_element * s_arr;

 // these typedefs below do nothing! They are only for a better understanding!
typedef superstring new_superstring; // "new" will only be used in order to indicate, that a new superstring was created in the process
typedef s_arr new_s_arr; // "new" will only be used in order to indicate, that a new s_arr was created in the process
typedef superstring mod_superstring; // "mod" will only be used in order to indicate, that the superstring was modified in the process
typedef s_arr mod_s_arr; // "mod" will only be used in order to indicate, that the s_arr was modified in the process
typedef superstring new_s_element_pointer; // "new_s_arr_element_pointer" will be used to indicate, that a new s_element will be created and the pointer to it will be returned (cannot be used like a normal superstring!!!)
typedef s_arr new_s_arr_element_pointer; // "new_s_arr_element_pointer" will be used to indicate, that a new s_arr_element will be created and the pointer to it will be returned (cannot be used like a normal s_arr!!!)

// superstring - no memory-leaks!!!
int s_len(const superstring value); // returns the lenght of the superstring
int s_node_len(const superstring value); // returns the number of node of the superstring
new_s_element_pointer s_createC(char value, int anzahl); // creates a new s_element
new_superstring s_init(const char * string); // creates a new superstring from a string
superstring s_seek(const superstring dest, int index); // gives a pointer to a node of the superstring
mod_superstring s_addFirst(superstring head, char value, int anzahl); // creates a new s_element and moves it to the first position of the superstring
mod_superstring s_addLast(superstring head, char value, int anzahl); // creates a new s_element and moves it to the last position of the superstring
void s_addChar(superstring dest, char c, int index, int anzahl); // creates a new s_element and moves it to any position (exept the first!) of the superstring
mod_superstring s_removeFirst(superstring head); // deletes (and frees the allocated memory) of the first s_element in the superstring
void s_removeChar(superstring dest, int index); // deletes (and frees the allocated memory) of a s_element at any position (exept the first!) in the superstring
void s_print(const superstring value); // prints a superstring to stdout
void s_free(superstring value); // frees a whole superstring (all s_element)
void s_stringify(const superstring value, char * saveto); // converts a superstring to a string and saves the string at "saveto"
new_superstring s_copy(const superstring value); // creates a new superstring, which is a copy of another superstring
new_superstring s_alt_copy(const superstring value); // an alternative to s_copy
mod_superstring s_add(superstring dest, const superstring value); // adds a superstring (value) to the end of a superstring (dest)
mod_superstring s_addStr(superstring dest, const char * value); // adds a string (value) to the end of a superstring (dest)
int s_compare(const superstring value1, const superstring value2);

// s_arr
int s_arr_len(const s_arr value); // returns the lenght of the s_arr
s_arr s_arr_createEl(const superstring value); // creates a new s_arr_element
s_arr s_arr_init(int anzahl, ...); // creates a new s_arr from several strings. "anzahl" = number of arguments (exept anzahl)
s_arr s_arr_addFirst(s_arr head, const superstring value); // creates a new s_arr_element and moves it to the first position of the s_arr
s_arr s_arr_addLast(s_arr head, const superstring value); // creates a new s_arr_element and moves it to the last position of the s_arr
void s_arr_addEl(s_arr dest, const superstring el, int index); // creates a new s_arr and moves it to any position (exept the first!) of the s_arr
s_arr s_arr_removeFirst(s_arr head); // deletes (and frees the allocated memory) of the first s_arr_element in the s_arr (including the superstrings)
void s_arr_removeEl(s_arr dest, int index); // deletes (and frees the allocated memory) of an s_arr_element at any position (exept the first!) in the s_arr (including the superstrings)
superstring s_arr_getEl(s_arr dest, int index); // returns the superstring inside the s_arr_element at any position
void s_arr_print(s_arr value); // prints an s_arr (with style)
void s_arr_free(s_arr value); // frees a whole s_arr (all s_arr, including the superstrings)
int s_arr_stringify(const s_arr value, char *** saveto); // converts a s_arr to a wordArr (see words.h) / a 2D-char_Array and saves it at "saveto" (including malloc). Only give empty pointer  to "saveto" in the form of "&pointer"!!!
int s_arr_findFirstIndex(const s_arr value, const superstring search);

// combi / multiple AND search
void s_free_mult(int index, ...); // can free several pointers of different types (int * , char * , double * , float * , void * , superstring, s_arr)
void s_zwischenStr(const superstring text, int start, int end, char * saveto); // saves the content between index "start" and "ende" of a superstring (text) as a string to saveto
new_superstring s_zwischen(const superstring text, int start, int end); // creates a new superstring with the content between index "start" and "ende" of a superstring (text)
void s_printf(const char * text, ...); // like "printff", but can only print strings and superstrings BETA!!!
int s_findIndex(const superstring text, const superstring search, int * saveto); // returns how many times "search" was found in "text" and saves an Array of 0 and 1 to saveto
new_s_arr s_split(const superstring text, const superstring search);
new_superstring s_join(const s_arr wordArr, const superstring separator);

// CHANGE!!!
s_arr s_splitStr(const superstring text, const char * search);
s_arr s_splitFirstStr(const superstring text, const char * search);
superstring s_joinStr(s_arr text, const char * value); // !!! CHANGE !!! -> Memory leak !!!
superstring s_replace(superstring text, const superstring search, const superstring repl); // !!! CHANGE !!! -> ugly
superstring s_replaceStr(superstring text, char * search, char * repl);
superstring s_replaceFirst(superstring text, const superstring search, const superstring repl); // !!! CHANGE !!! -> ugly
superstring s_replaceFirstStr(superstring text, char * search, char * repl);

#ifdef __cplusplus
}
#endif