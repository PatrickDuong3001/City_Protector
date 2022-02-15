//=================================================================
// Header file for DLL module.
//
// Copyright 2021 Georgia Tech.  All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must not be made publicly available anywhere.
//=================================================================

#ifndef DOUBLELINKEDLIST_H
#define DOUBLELINKEDLIST_H

// A linked list node structure.
typedef struct llnode_t {
    void* data;
    struct llnode_t* prev;
    struct llnode_t* next;
}LLNode;

/// The structure to store the information of a doubly linked list
typedef struct dlinkedlist_t {
    struct llnode_t* head;
    struct llnode_t* tail;
    struct llnode_t* curr;
    int size;
} DLinkedList;

// Used for testing.
struct LLItem {};
void testDLL(void);

/**
 * create_llnode
 *
 * Creates a node by allocating memory for it on the heap,
 * and initializing its previous and next pointers to NULL and its data pointer to the input
 * data pointer
 *
 * @param data A void pointer to data the user is adding to the doublely linked list.
 * @return A pointer to the linked list node
 */
LLNode* create_llnode(void* data);

/**
 * create_dlinkedlist
 *
 * Creates a doubly linked list by allocating memory for it on the heap. Initialize the size to zero,
 * as well as head and tail pointers to NULL
 *
 * @return A pointer to an empty dlinkedlist
 */
DLinkedList* create_dlinkedlist(void);


/**
 * InsertHead
 *
 * Create a new LLNode with the given data and insert it at the head of the doubly linked list.
 * Be sure to update the list's size, head (and tail if necessary).
 *
 * @param dLinkedList A pointer to the doubly linked list
 * @param data A void pointer to data the user is adding to the doubly linked list.
 * 
 */
void insertHead(DLinkedList* dLinkedList, void* data);

/**
 * deleteNode
 *
 * Delete the node pointed to by Node (splice it out).  Update list head/tail if necessary.
 * Update the list's size. Free the Node's data and Node.
 * Assume Node is not NULL.
 *
 * @param dLinkedList A pointer to the doubly linked list
 * @param Node A pointer to a linked list node.
 */
void deleteNode(DLinkedList* dLinkedList, LLNode* Node);


/**
 * destroyList
 *
 * Destroy the doubly linked list. Everything in the linked list including list structure,
 * nodes and data are all freed from the heap
 *
 * @param dLinkedList A pointer to the doubly linked list
 *
 */
void destroyList(DLinkedList* dLinkedList);

/**
 * getSize
 *
 * Return the size of the doubly linked list
 *
 * @param dLinkedList A pointer to the doubly linked list
 * @return  the size
 */
int getSize(DLinkedList* dLinkedList);
#endif

