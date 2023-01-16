//
// Created by jlryan on 1/13/23.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define COUNT(list) list->count

//Node represents a node in the linked list, word is the word attached to the node
// word is at the end, as the size of word is unknown so it cannot lie between two consecutive pointers in memory
typedef struct Node{
    struct Node *nextNode,*prevNode;
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

void destroy_list(nodeList *list){
    Node *curnode = list->firstNode;

    while(curnode->nextNode!=NULL){
        curnode = curnode->nextNode;
        free(curnode->prevNode);
    }
    //free(curnode);
    free(list->lastNode);
    //if(list->firstNode != NULL || list->lastNode != NULL){ printf("\nFAILED TO DESTROY LIST\n"); }
    free(list);
}

void add_node(nodeList *list, char *value){
    Node *newNode = calloc(1,sizeof(Node));

//Calloc the memory needed for a string the size of *value
    assert(value != NULL);
    newNode->word = calloc(1,sizeof(char) * strlen(value) + 2 );

//strncpy acts as a secondary check to ensure that only the length of the string is copied
    strncpy(newNode->word,value,sizeof(strlen(value)) + 1);

    if(list -> lastNode == NULL){
        list -> firstNode = newNode;
        list -> lastNode = newNode;
        newNode->prevNode = NULL;
    } else{
        newNode -> prevNode = list -> lastNode;
        list -> lastNode -> nextNode = newNode;     //Takes the previous last node and points it to the next node
        list -> lastNode = newNode;                 //Sets the added node as the last node
    }

    list -> count++;
}

void iterate_list(nodeList *list){
    Node *cur_node = list -> firstNode;
    assert(cur_node != NULL);
    for(int i = 0; i < list -> count; i++){

        printf("%s\n",cur_node->word);
        assert(cur_node->word != NULL);
        //printf("size of %lu\n",strlen(cur_node->word));

        if(cur_node->nextNode == NULL) break;
        cur_node = cur_node -> nextNode;

    }
}

//void stringSlice(char *string, int start, int end){}

void parseFile(char filename[],nodeList *list){
    FILE *file;
    int file_size;
    char* token;
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
        //printf("%s\n",token);
        add_node(list,token);
        token = strtok(NULL,delmiters);
        loop_check++;
        if(loop_check>file_size){ printf("TOKEN LOOP STUCK!!"); exit(1);}
    }

    free(buffer);
    fclose(file);

}

//Returns A - B = - 1       J-A = 7
int compare_nodes(Node *nodeA,Node *nodeB){
    //returns s1 - s2, e.g s1>s2 = + || s1 < s2 = -
    return strcmp(nodeA->word,nodeB->word);
}

void swap_nodes(Node *nodeA, Node *nodeB){
    //Wow I really hope there aren't any memory issues here!
    char *tmp = nodeA->word;
    nodeA->word = nodeB->word;
    nodeB->word = tmp;
}


void rm_node(Node *node,nodeList *list){

    /*
     * Deal with edge cases when removing node and primary case -- no chance this works
     * For start and end nodes, swap the node with its neighbor and then treat it as the normal case.
     * FIXED - The pointer logic looks a bit messy, clean up later maybe - FIXED
     *  FIXED - Just realized this can't handle the cases where there are two nodes only - FIXED
    */
    if(node == NULL){ printf("ERROR; CAN'T REMOVE NULL NODE"); return;}
    if((node->nextNode == NULL) && (node->prevNode == NULL)){
        printf("ATTEMPT REMOVE ONLY NODE\n");
        list->firstNode = NULL;
        list ->lastNode = NULL;
        list->count--;
        free(node);

        assert(list->count==0);
        return;}
    else if(node->prevNode == NULL){
        list->firstNode=node->nextNode;
        node->nextNode->prevNode = NULL;
        list->count--;
        free(node);
        return;
    }
    else if(node->nextNode == NULL){

       list->lastNode = node->prevNode;
       node->prevNode->nextNode = NULL;
       list->count--;
       free(node);
        return;
    }

    else{
        node->prevNode->nextNode = node->nextNode;
        node->nextNode->prevNode = node->prevNode;
        list->count--;
    }
    free(node);

}

void rm_duplicates(nodeList *list){
    Node *curnode = list->firstNode;
    if(curnode==NULL) return;

    while(curnode->nextNode != NULL){
        if(compare_nodes(curnode,curnode->nextNode) == 0){
            //free is included in the rm_node function
            rm_node(curnode->nextNode, list);
        }
        else{
            curnode=curnode->nextNode;
        }
    }
}

nodeList *common_words(nodeList *listA, nodeList *listB) {
    nodeList *common = create_list();
    Node *curnodeA = listA->firstNode;
    Node *curnodeB;
    for (int i = 0; i < listA->count; i++) {
        curnodeB = listB->firstNode;
        for (int j = 0; j < listB->count; j++) {
            //printf("\n%s %s\t",curnodeA->word,curnodeB->word);
            if ((curnodeA == NULL) || (curnodeB == NULL)) { break; }
            if (compare_nodes(curnodeA,curnodeB) == 0) {
                add_node(common, curnodeA->word);
                //printf("\n%s %s\t",curnodeA->word,curnodeB->word);
                //printf("MATCH!\n");

            }
            curnodeB = curnodeB->nextNode;
        }
        curnodeA = curnodeA->nextNode;

    }
    return common;

}

//bubble sort and remove all duplicate words
//Currently causing issues that I don't want to deal with so Im gonna try mergesort...
void bubble_sort(nodeList *list) {
    int compare;
    if (list->count <= 1) {
        printf("List is length <=1 & sorted.\n");
        return;
    }
    Node *curnode = list->firstNode;
    for (int j = 0; j < list->count; j++) {

        for (int i = 0; i < list->count - 1; ++i) {
            compare = compare_nodes(curnode, curnode->nextNode);
            if (compare > 0) {
                //printf("%s > %s\n",curnode->word,curnode->nextNode->word);
                swap_nodes(curnode, curnode->nextNode);
                curnode = curnode->nextNode;
            }
            else if(compare == 0){rm_node(curnode->nextNode,list);}
            else{curnode = curnode->nextNode;}
        }
        curnode = list->firstNode;
    }
}

nodeList *list_merge(nodeList *leftList,nodeList *rightList){
    nodeList *merged = create_list();

    //If either list has elements, add an element to the merged list
    while((leftList->count > 0) || (rightList->count >0)){
        //If both lists have elements, compare them and add the smaller element
        if((leftList->count > 0) && (rightList->count >0)){
            if(compare_nodes(leftList->firstNode,leftList->firstNode) >=0){
                add_node(merged,leftList->firstNode->word);
                rm_node(leftList->firstNode,leftList);
            }
            else{
                add_node(merged,rightList->firstNode->word);
                rm_node(rightList->firstNode,rightList);
            }
        }
        else if(leftList->count > 0){
            add_node(merged,leftList->firstNode->word);
            rm_node(leftList->firstNode,leftList);
        }
        else if(rightList->count > 0){
            add_node(merged,rightList->firstNode->word);
            rm_node(rightList->firstNode,rightList);
        }
    }

    return merged;

}
nodeList *merge_sort(nodeList *list){

    int len = COUNT(list);
    if(len <= 1){return list;}

    nodeList *leftList = create_list();
    nodeList *rightList = create_list();
    Node *curnode = list->firstNode;
    int split = len / 2;

    //Adds to leftlist until middle is reached then adds to right list
    for(int i = 0; i < len; i++){
        if(split > i){
            add_node(leftList,curnode->word);
            curnode = curnode->nextNode;
        }
        else{
            add_node(rightList,curnode->word);
            curnode = curnode->nextNode;
        }
    }

    nodeList *left_sorted = merge_sort(leftList);
    nodeList *right_sorted = merge_sort(rightList);

    if(left_sorted != leftList){ destroy_list(leftList);}
    if(right_sorted != rightList){ destroy_list(rightList);}

    return list_merge(left_sorted,right_sorted);

}

//Writes values of all nodes of a list into a file, Delimited with \n
void writeFile(char *output,nodeList *list){
    FILE *file;
    Node *curnode = list->firstNode;
    file = fopen(output,"w");

    do {
        fprintf(file, "%s\n",curnode->word);
        //fputs("\n",file);
        curnode=curnode->nextNode;
    }while(curnode->nextNode != NULL);
    fputs(curnode->word,file);
    fclose(file);
}

void master(int nArgs, char *args[],int fileIndex){
    nodeList *stat = create_list();

    int nFiles = strtol(args[1],NULL,10);
    nodeList *files[nFiles];
    if(nFiles <= 1){
        parseFile(args[2],stat);
        bubble_sort(stat);
        rm_duplicates(stat);
        writeFile(args[3],stat);
        return;
    }
    else {
        for (int i = fileIndex; i < nArgs - 1; i++) {
            printf("\n%s\n",args[i]);
            files[i-fileIndex] = create_list();
            parseFile(args[i], files[i-fileIndex]);

        }
        bubble_sort(files[0]);
        rm_duplicates(files[0]);

        bubble_sort(files[1]);
        rm_duplicates(files[1]);

        nodeList *Common = common_words(files[0],files[1]);
        int lists = 2;

        if(nFiles>=3) {
            do {
                //If no matching words, return a blank file. This saves unnecessary work because
                //if two files have no shared words, there are no words in common with all files.
                if(Common->count == 0){
                    printf("EASY OUT\n");
                    writeFile(args[nArgs-1],Common);
                    return;
                }
                bubble_sort(files[lists]);
                rm_duplicates(files[lists]);
                Common = common_words(Common, files[lists]);
                //bubble_sort(Common);
                //rm_duplicates(Common);
                lists++;
            } while (lists < nFiles - 1);
        }


        bubble_sort(Common);
        rm_duplicates(Common);
        writeFile(args[nArgs-1],Common);
        return;
    }
}

int main(int argc, char *argv[]) {

    /*
    nodeList *list1 = create_list();
    nodeList *list2 = create_list();
    nodeList *list3 = create_list();

    //iterate_list(list1);

    //printf("\n%s sizeof: %lu\n", list1->firstNode->word,sizeof(nodeList));

    printf("\n FILELENGTH %d ARGC %d\n", parseFile("files/in1_3.txt",list1),argc);
    printf("\n FILELENGTH %d ARGC %d\n", parseFile("files/in2_3.txt",list2),argc);
    printf("\n FILELENGTH %d ARGC %d\n", parseFile("files/in3_3.txt",list3),argc);
    printf("\n%s\n",NEXT(list2->firstNode));
//    printf(" %d %s Hello, World!\n", argc,node1.word);
//    printf(" %d %s Hello, World!\n", argc,argv[file1]);
    //swap_nodes(list2->lastNode,list2->firstNode);
    //rm_node(list2->lastNode,list2);
    nodeList *Common = create_list();


    Common = common_words(list1,list2);
    Common = common_words(Common, list3);

    bubble_sort(Common);
    iterate_list(Common);

    writeFile("files/output.txt",Common);

/*
    /*
    char cwd[256];
    getcwd(cwd,256);
    printf("%s",cwd);
    */
    //free(list1);

    master(argc,argv,2);
    printf("\nARGC %d Argv[1] %s\n",argc,argv[1]);
    return 0;
}

