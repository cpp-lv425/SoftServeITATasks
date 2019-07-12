#include <stdio.h>
#include <string.h>

#define DATA 0
#define NEXT 1
#define POINTERS_IN_NODE 2

void StringPrint(char** list) {
    printf("<begin>\n");
    // Print if node exist and has data inside
    while(list && list[DATA]) {
        printf("%s\n", list[DATA]);
        list = (char**) list[NEXT];
    }
    printf("<end>\n");
}

void StringListInit(char*** list) {
    // Allocation 8 bytes for node [pointer to str][pointer to next node]
    *list = (char**) malloc(sizeof(char*) * POINTERS_IN_NODE);
    (*list)[DATA] = (*list)[NEXT] = NULL;
}

void StringListDestroy(char*** list) {
    // Delete node and go to the next
    do {
        char** tmp = (char**) (*list)[NEXT];
        free((*list)[DATA]);
        free(*list);
        *list = tmp;
    }
    while (*list);
    *list = NULL;
}

int StringListSize(char** list) {
    int size = 0;
    // Increment and go to the next node
    while (list && list[DATA]) ++size, list = (char**) list[NEXT];
    return size;
}

void StringListAdd(char** list, char* str) {
    // Allocate memory for string and write data to this memory
    char* text = (char*) malloc(sizeof(char) * strlen(str) + 1);
    strcpy(text, str);

    // If node don't contain string (root node) - write data
    if(list[DATA] == NULL) {
        list[DATA] = text;
    }
    else {
        // Go to the last node
        while(list[NEXT]) list = (char**) list[NEXT];
        // Create new node and write data
        char** node = (char**) malloc(sizeof(char*) * POINTERS_IN_NODE);
        node[DATA] = text;
        node[NEXT] = NULL;
        list[NEXT] = (char*) node;
    }
}

void StringListRemove(char** list, char* str) {
    char** prev = NULL;
    while(list) {
        if(list[DATA] && !strcmp(list[DATA], str)) {
            // If before current node exist another node
            if(prev) {
                prev[NEXT] = list[NEXT];
                free(list[DATA]);
                free(list);
                list = (char**) prev[NEXT];
            }
            // If we must remove first (root) node and next node exist
            else if(list[NEXT]) {
                char *tmp = list[DATA];
                memcpy(list, list[NEXT], sizeof(char *) * POINTERS_IN_NODE);
                free(tmp);
            }
            // If list has one node
            else if(list[DATA]) {
                free(list[DATA]);
                list[DATA] = NULL;
            }
        // Check next node
        } else {
            prev = list;
            list = (char**) list[NEXT];
        }
    }
}

int StringListIndexOf(char** list, char* str) {
    int i = 0;
    // Find first occurrence of string and return index (start from 0)
    while(list) {
        if(!strcmp(list[DATA], str)) {
            return i;
        } else {
            list = (char**) list[NEXT];
            ++i;
        }
    }
    return -1;
}

void StringListRemoveDuplicates(char** list) {
    while(list) {
        char** curr = (char**) list[NEXT];
        // Save first occurrence of node
        char** prev = list;
        while(curr) {
            // If node is duplicate - remove
            if(!strcmp(list[DATA], curr[DATA])) {
                prev[NEXT] = curr[NEXT];
                free(curr[DATA]);
                free(curr);
                curr = (char**) prev[NEXT];
            }
            // Go to the next node
            else {
                prev = curr;
                curr = (char**) curr[NEXT];
            }
        }
        // Save next uniq node
        list = (char**) list[NEXT];
    }
}

void StringListReplaceInStrings(char** list, char* before, char* after) {
    while (list) {
        if (!strcmp(list[DATA], before)) {
            // Remove string from node
            free(list[DATA]);
            // Write new string
            list[DATA] = (char*) malloc(sizeof(char) * strlen(after) + 1);
            strcpy(list[DATA], after);
        }
        list = (char**) list[NEXT];
    }
}

void StringListSort(char** list) {
    if (!(StringListSize(list) > 1)) return;

    int size = StringListSize(list);
    // Bubble sort
    for(int i = 0; i < size; ++i) {
        char** curr = (char**) list[NEXT];
        char** prev = list;
        for(int j = 0; j < size - i - 1; ++j) {
            int i = 0;
            // If letters in string is same - go to the next letters
            while(prev[DATA][i] == curr[DATA][i]) ++i;
            // Swap pointers to data in nodes
            if(prev[DATA][i] > curr[DATA][i]) {
                char *tmp = prev[DATA];
                prev[DATA] = curr[DATA];
                curr[DATA] = tmp;
            }
            prev = curr;
            curr = (char**) curr[NEXT];
        }
    }
}

int main() {
    char **list;
    StringListInit(&list);
    StringListAdd(list, "Hello");
    StringListAdd(list, "Hi");
    StringListAdd(list, "Aloha");
    StringListDestroy(&list);
}