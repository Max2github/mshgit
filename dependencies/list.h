#pragma once
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum list_types {
    Char, Integer, Float, Double, String, // primitive Variables
    Char_pointer, Integer_pointer, Float_pointer, Double_pointer, // pointer
    Void_pointer, // for e.g. functions
    List, List_pointer, // for e.g. appending a List to another : List -> creates a copy, List_pointer -> only a pointer is given => Memory!
    End // symbolyses the end of the argument list (va_list)
} list_type;

struct list_element {
    void * el; // points an element to any type
    list_type type;
    struct list_element * next;
};
typedef struct list_element list_element;
typedef struct list_element * list;
typedef struct list_element * list_element_pointer;

typedef enum select_keys {
    select_All, select_Distinct, select_Whole_record, select_First
} select_key;

typedef enum sort_keys {
    sort_size_low, sort_size_high, sort_value_low, sort_value_high
} sort_key;

// little functions
int list_node_len(list head);
list list_copy(list value);

// functions to create new list_elements
// primitive
list_element_pointer new_list_element_char(char c);
list_element_pointer new_list_element_int(int zahl);
list_element_pointer new_list_element_float(float zahl);
list_element_pointer new_list_element_double(double zahl);
// array
list_element_pointer new_list_element_string(char * str);
// pointer
list_element_pointer new_list_element_char_pointer(char * zP);
list_element_pointer new_list_element_int_pointer(int * zP);
list_element_pointer new_list_element_float_pointer(float * zP);
list_element_pointer new_list_element_double_pointer(double * zP);
// list & other
list_element_pointer new_list_element_list(list under); // creates a list_element of ->type = List and ->el = list under
list_element_pointer new_list_element_type(list_type type, void * pointer); // creates a copy of a list_element and gives a pointer to it

// add, remove, print & free
// add
list list_addFirst_element(list head, list_element_pointer new_list_el);
void list_addLast_element(list head, list_element_pointer new_list_el_el);
list list_addFirst_list(list head, list newHead);
void list_addIndex_list(list head, list newEl, unsigned int index);
// remove
list list_removeFirst(list head);
void list_removeLast(list head);
void list_removeIndex(list head, unsigned int index);
// print
void list_print_format(unsigned int format);
void list_element_sprint(list_element_pointer head, char * saveto, list separatorList);
void list_toStr(list head, char * saveto, list separatorList);
void list_element_print(list_element_pointer head, unsigned int format);
void list_print(list head, unsigned int format);
// free
void list_element_free(list_element_pointer head);
void list_free(list head);

// whole lists
list new_list_element_as_list(va_list elems);
list list_addFirst(list head, ...);
void list_addLast(list head, ...);
void list_addIndex(list head, unsigned int index, ...);

list_element_pointer list_seek(list head, unsigned int index);
int list_findFirstIndex(list head, int (*cond)(void * , list_type, int));
int list_element_compare(void * value, list_type type, void * element, list_type eltype);
int list_find(list head, void * value, list_type type);

// selects
list list_select_one(list head, int (*cond)(void * , list_type, int), select_key keyword, list greatSelect);
list list_select_va(select_key keyword, va_list args);
list list_select(select_key keyword, ...);
void list_select_remove(select_key keyword, ...);
list list_select_index_one(list head, int (*cond)(void * , list_type, int), select_key keyword, list greatSelect);

// sorting
int list_element_size(list_element_pointer head);
int list_size(list head);
void list_swap_element(list_element_pointer a, list_element_pointer b);
int list_sort_compare(sort_key keyword, int eins, int zwei);
void list_sort_size_to_first(list head, sort_key keyword);
void list_sort(list head, sort_key keyword);

#ifdef __cplusplus
}
#endif