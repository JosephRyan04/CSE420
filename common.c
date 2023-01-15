//
// Created by jlryan on 1/13/23.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
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

//Calloc the memory needed for a string the size of *value
    assert(value != NULL);
    newNode->word = calloc(1,sizeof(char) * strlen(value) + 2 );

//strncpy acts as a secondary check to ensure that only the length of the string is copied
    strncpy(newNode->word,value,sizeof(strlen(value)) + 1);

    /*
    //printf("new location %p\n",newNode->word);
    *tmp_ptr = *value;
    //printf("\n%s\n",*tmp_ptr);
    //newNode->word = value;
    newNode->word = calloc(1,sizeof(char) * strlen(value) + 1);
    printf("\nold location %p\t",newNode->word);
    //printf("\nlength of %s is %d\n",value, strlen(value));
    newNode->word = value;
    printf("new location %p\n",newNode->word);
    */

    if(list -> lastNode == NULL){
        list -> firstNode = newNode;
        list -> lastNode = newNode;
    } else{
        list -> lastNode -> nextNode = newNode;     //Takes the previous last node and points it to the next node
        list -> lastNode = newNode;                 //Sets the added node as the last node
    }

    list -> count++;
}

void iterate_list(nodeList *list){
    Node *cur_node = list -> firstNode;
    for(int i = 0; i < list -> count; i++){

        printf("%s\n",cur_node->word);
        assert(cur_node->word != NULL);
        //printf("size of %lu\n",strlen(cur_node->word));

        if(cur_node->nextNode == NULL) break;
        cur_node = cur_node -> nextNode;

    }
}

//void stringSlice(char *string, int start, int end){}

int parseFile(char filename[],nodeList *list){
    FILE *file;
    int file_size;
    char* token;
    char *reference = NULL;
    char delmiters[] = "\n \t ";
    int loop_check = 0;

    file = fopen(filename,"r");
    assert(file != NULL);

    fseek(file,0L,SEEK_END);
    file_size = ftell(file);
    char *buffer = calloc(file_size+1,1);

    fseek(file,0L, SEEK_SET);

    fread(buffer,1,file_size,file);

    if(feof(file)){ printf("\nReached EOF\n");}
    //strcpy a terminator char to esnure the file ends
    strcpy(buffer+file_size,"\0");
    //printf("\n%s\n",buffer);

    token = strtok(buffer,delmiters);

    while(token!=NULL){
        printf("%s\n",token);
        add_node(list,token);
        token = strtok(NULL,delmiters);
        loop_check++;
        if(loop_check>file_size){ printf("TOKEN LOOP STUCK!!"); exit(1);}
    }

    free(buffer);
    return file_size;
}

int main(int argc, char *argv[]) {

    nodeList *list1 = create_list();
    nodeList *list2 = create_list();
    char *testwords[] = {"nodeyeye","noas","michael","pingpongpower","pwpw"};

    for (int i = 0; i < sizeof(testwords)/sizeof testwords[0]; ++i) {
        add_node(list1, testwords[i]);
    }
    //iterate_list(list1);

    printf("\n%s sizeof: %lu\n", list1->firstNode->word,sizeof(nodeList));

    printf("\n FILELENGTH %d\n", parseFile("/home/jlryan/CLionProjects/project1/test.txt",list2));
//    printf(" %d %s Hello, World!\n", argc,node1.word);
//    printf(" %d %s Hello, World!\n", argc,argv[file1]);
    iterate_list(list2);

    //free(list1);
    return 0;
}