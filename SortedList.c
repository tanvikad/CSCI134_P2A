#include "SortedList.h"
#include <pthread.h>
#include <stdio.h>
#include <string.h>

void SortedList_insert(SortedList_t *list, SortedListElement_t *element) {
    if(list->next == NULL) {
        if(opt_yield & INSERT_YIELD) sched_yield();
        list->next = element;
        list->prev = element;
        element->next = list;
        element->prev = list;
        return;
    }


    SortedListElement_t* current = list->next;
    while(1) {
        if(current->key == NULL) {
            //we should add it right before the element
            SortedListElement_t* previous_element = current->prev;
            if(opt_yield & INSERT_YIELD) sched_yield();
            current->prev = element;
            element->next = current;
            element->prev = previous_element;
            previous_element->next = element;
            break;
        }
        //a b c d f 
        if(strcmp(element->key, current->key) < 0) {
            SortedListElement_t* previous_element = current->prev;
            if(opt_yield & INSERT_YIELD) sched_yield();
            current->prev = element;
            element->next = current;
            element->prev = previous_element;
            previous_element->next = element;
            break;
        } else {
            current = current->next;
            continue;
        }
    }

}