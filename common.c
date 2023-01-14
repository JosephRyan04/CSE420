//
// Created by jlryan on 1/13/23.
//
#include <stdio.h>
#include <stdlib.h>
#define file1 1

//Node represents a node in the linked list, word is the word attached to the node
// word is at the end, as the size of word is unknown so it cannot lie between two consecutive pointers in memory
typedef struct Node{
    struct Node *nextNode;
    char *word;

}Node;

//nodeList stores the location of the end nodes (or words) from a particular file
typedef struct nodeList{
    int count;
    Node *firstNode;
    Node *lastNode;
}nodeList;

nodeList *create_list(){
    return calloc(1, sizeof(nodeList));
}

void add_node(nodeList *list, char *value){
    Node *newNode = calloc(1,sizeof(Node));
    newNode->word = value;

    if(list -> lastNode == NULL){
        list -> firstNode = newNode;
        list -> lastNode = newNode;
    } else{
        list -> lastNode -> nextNode = newNode;     //Takes the previous last node and points it to the next node
        list -> lastNode = newNode;
    }

    list -> count++;
}

void iterate_list(nodeList *list){
    Node *cur_node = list -> firstNode;
    for(int i = 0; i < list -> count; i++){

        printf("%s\n",cur_node->word);
        cur_node = cur_node ->nextNode;
    }
}
//FILE *readFile(char *filelname[]){}

int main(int argc, char *argv[]) {

    nodeList *list1 = create_list();
    char *testwords[] = {"nodeyeye","node2","michael","pingpongpower"};

    for (int i = 0; i < sizeof(testwords)/sizeof testwords[0]; ++i) {
        add_node(list1, testwords[i]);
    }
    iterate_list(list1);
    printf("%s 123\n", list1->firstNode->word);
//    printf(" %d %s Hello, World!\n", argc,node1.word);
//    printf(" %d %s Hello, World!\n", argc,argv[file1]);

    return 0;
}