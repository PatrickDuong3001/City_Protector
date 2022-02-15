//=================================================================
// Implementation for DLL module.
//
// Copyright 2021 Georgia Tech.  All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must not be made publicly available anywhere.
//=================================================================
#include <stdlib.h>
#include <stdio.h>
#include "doubly_linked_list.h"
#include "globals.h"

// Create a node for a linked list, 
LLNode* create_llnode(void* data) {
    LLNode* newNode;
    newNode = (LLNode*) malloc(sizeof(LLNode));
    newNode->prev = NULL;
    newNode->next = NULL; 
    newNode->data = data; 
    return newNode;
}

DLinkedList* create_dlinkedlist(void) {
    DLinkedList* newList = (DLinkedList*)malloc(sizeof(DLinkedList));
    newList->head = NULL;
    newList->tail = NULL;
    newList->size = 0;
    return newList;
}

/**
  This function helps the program to insert a new node to the head of a linked list. 
  */
void insertHead(DLinkedList* dLinkedList, void* data){
  LLNode* newNode = create_llnode(data);
  if (dLinkedList->head == NULL) //if the linked list is empty
  {
    dLinkedList->head = newNode;   //head and tail point to the Node
    dLinkedList->tail = newNode;   
  }else //if the list is not empty
  {  
    newNode->next = dLinkedList->head;  //Node becomes head
    (dLinkedList->head)->prev = newNode;  
    dLinkedList->head = newNode;   //update new head
  }
  dLinkedList->size = dLinkedList->size + 1; //update size of the list
}

/**
  Function to help the program delete a node from a linked list. 
  */
void deleteNode(DLinkedList* dLinkedList, LLNode* Node){
  // complete this function
  
  if(Node == NULL || dLinkedList->head == NULL) //if the linked list is empty
  {
    return;
  }
  
  if(Node == dLinkedList->head && Node == dLinkedList->tail) //when there's only one node in the list
  {
    dLinkedList->head = NULL;
    dLinkedList->tail = NULL; 
  }else  //when there at least 2 nodes in the list
  {
    if(Node == dLinkedList->head) //if delete first node
    {
        dLinkedList->head = (dLinkedList->head)->next; //first node in list is gone. New head is the second node
        (dLinkedList->head)->prev = NULL;  //second node's prev pointer now is NULL
    }else if(Node == dLinkedList->tail) //if delete last node
    {
        dLinkedList->tail = (dLinkedList->tail)->prev;  //the second to last node becomes the new tail
       (dLinkedList->tail)->next = NULL; //the next pointer of the tail is NULL
    }else //this happens when the deleted node is in the middile of the list
    {
        (Node->prev)->next = Node->next; 
        (Node->next)->prev = Node->prev;
    }  
  }
  
  free(Node->data);
  free(Node);
  dLinkedList->size = dLinkedList->size - 1; //update size of the list
}


void destroyList(DLinkedList* dLinkedList){
  LLNode* Node = dLinkedList->head;
  LLNode* Next;
  while(Node){
    free(Node->data);
    Next = Node->next;
    free(Node);
    Node = Next;
    }
  free(dLinkedList);
}

int getSize(DLinkedList* dLinkedList){
  return dLinkedList->size;
}

// Check for common errors and ASSERT_P to raise the error code.
void testDLL(void){
  int n = 4;
  LLItem* m[n];
  DLinkedList* myList = create_dlinkedlist();
  while (n--)
    m[n] = (LLItem*) malloc(sizeof(LLItem));
  for(n = 0; n<3; n++){
    insertHead(myList, m[n]);
    if ((myList->head->data != m[n]) || (getSize(myList) != n+1))
      uLCD.printf("Complete and debug DLL to get rid of this error\n");
    ASSERT_P(myList->head->data == m[n], ERROR_DLL_INSERT_HEAD);
    ASSERT_P(getSize(myList) == n+1, ERROR_DLL_INSERT_HEAD);
  }
  // [2 1 0]
  LLNode* current = myList->head;
  for (n = 2; n>=0; n--){
    if (current->data != m[n])
      uLCD.printf("Complete and debug DLL to get rid of this error\n");
    ASSERT_P(current->data == m[n], ERROR_DLL_INSERT_HEAD);
    current = current->next;
  }
  pc.printf("OK DLL 3 x insertHead: [2 1 0]\n");
  uLCD.printf("OK DLL 3 inserts:\n    [2 1 0]\n");
  deleteNode(myList, myList->head->next);  // delete middle node
  if (getSize(myList) != 2) 
    uLCD.printf("Complete and debug DLL to get rid of this error\n");
  ASSERT_P(getSize(myList) == 2, ERROR_DLL_DELETE);
  // [2 0]
  current = myList->head;
  for (n = 2; n>=0; n=n-2){
    if (current->data != m[n]) 
      uLCD.printf("Complete and debug DLL to get rid of this error\n");
    ASSERT_P(current->data == m[n], ERROR_DLL_DELETE);
    current = current->next;
  }
  pc.printf("OK DLL deleteNode middle: [2 0]\n");
  uLCD.printf("OK DLL del mid:\n    [2 0]\n");
  insertHead(myList, m[3]); // 3 2 0
  if (getSize(myList) != 3) 
    uLCD.printf("Complete and debug DLL to get rid of this error\n");
  ASSERT_P(getSize(myList) == 3, ERROR_DLL_INSERT_HEAD);
  pc.printf("OK DLL insertHead: [3 2 0]\n");
  uLCD.printf("OK DLL 1 ins:\n    [3 2 0]\n");
  deleteNode(myList, myList->head);  // delete head
  if (getSize(myList) != 2)
    uLCD.printf("Complete and debug DLL to get rid of this error\n");
  ASSERT_P(getSize(myList) == 2, ERROR_DLL_DELETE);
  current = myList->head;
  // [2 0]
  for (n = 2; n>=0; n=n-2){
    if (current->data != m[n]) 
      uLCD.printf("Complete and debug DLL to get rid of this error\n");
    ASSERT_P(current->data == m[n], ERROR_DLL_DELETE);
    current = current->next;
  }
  pc.printf("OK DLL deleteNode head: [2 0]\n");
  uLCD.printf("OK DLL del head:\n    [2 0]\n");
  destroyList(myList);
}

