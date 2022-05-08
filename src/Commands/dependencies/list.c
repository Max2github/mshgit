#include "../../../include/alg.h"
#include "../../../dependencies/words.h"

list msh_getListByName(const char * name) {
    list temp = LIST_SPEICHER;
    while (temp != NULL) {
        if ( word_compare( (char *) ((list) temp->el)->next->el /* get the second element of the current node and cast it to a string */ , name) == 0 ) {
            return (list) ((list) temp->el)->el /* get the first element of the current node and cast it to a list */;
        }
        temp = temp->next;
    }
    return NULL;
}
list msh_getListBossByName(const char * name) {
    list temp = LIST_SPEICHER;
    while (temp != NULL) {
        if ( word_compare( (char *) ((list) temp->el)->next->el /* get the second element of the current node and cast it to a string */ , name) == 0 ) {
            return ((list) temp->el) /* get the first element of the current node and cast it to a list */;
        }
        temp = temp->next;
    }
    return NULL;
}
list msh_getNodeByName(const char * name) {
    list temp = LIST_SPEICHER;
    while (temp != NULL) {
        if ( word_compare( (char *) ((list) temp->el)->next->el /* get the second element of the current node and cast it to a string */ , name) == 0 ) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}
list msh_getBeforeNodeByName(const char * name) {
    list temp = LIST_SPEICHER;
    list before = NULL;
    while (temp != NULL) {
        if ( word_compare( (char *) ((list) temp->el)->next->el /* get the second element of the current node and cast it to a string */ , name) == 0 ) {
            return before;
        }
        before = temp;
        temp = temp->next;
    }
    return NULL;
}