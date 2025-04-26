#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#if 1
#define DOUBLY_LINKED_LIST
#endif

typedef struct _Node
{
    int data;
    struct _Node *next;
#if defined(DOUBLY_LINKED_LIST)
    struct _Node *prev;
#endif
} Node;

Node* createNode(int data)
{
    Node *newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = NULL;
#if defined(DOUBLY_LINKED_LIST)
    newNode->prev = NULL;
#endif
    return newNode;
}

Node* insertNode(Node *head, int pos, int data)
{
    if (pos < 1) return head;
    Node *newNode = createNode(data);
    Node *curr = head;

    if (pos == 1)
    {
        // insert at beginning. Head will be updated
        newNode->next = head;
        return newNode;
    }

    for (int p = 1; (p < pos - 1) && curr; p++)
    {
        curr = curr->next;
    }

    if (curr)
    {
        newNode->next = curr->next;
        curr->next = newNode;
#if defined(DOUBLY_LINKED_LIST)        
        newNode->prev = curr;
        if(newNode->next)
        {
            newNode->next->prev = newNode;
        }
#endif
    }
    return head;
}

Node* deleteNode(Node *head, int pos)
{
    if (pos < 1) return head;
    Node *curr = head;
    Node *prev = NULL;

    if (pos == 1)
    {
        head = head->next;
#if defined(DOUBLY_LINKED_LIST)
        head->prev = NULL;
#endif
        free(curr);
        return head;
    }

    for (int p = 1; (p < pos) && curr; p++)
    {
#if !defined(DOUBLY_LINKED_LIST)
        prev = curr;
#endif
        curr = curr->next;
    }

    if (curr)
    {
#if !defined(DOUBLY_LINKED_LIST)
        prev->next = curr->next;
#else
        curr->prev->next = curr->next;
        if(curr->next)
        {
            curr->next->prev = curr->prev;
        }
#endif
        free(curr);
    }

    return head;
}

Node* reverseList(Node *head)
{
    if (head == NULL) return head;
    Node *curr = head;
    Node *prev = NULL;
    Node *next;

    while(curr)
    {
#if !defined(DOUBLY_LINKED_LIST)
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
#else
        // swap prev and next pointer
        prev = curr->prev;
        curr->prev = curr->next;
        curr->next = prev;

        // move to the next node (Next node becomes prev because of swapping)
        curr = curr->prev;
#endif
    }
    // update head to the last node
#if defined(DOUBLY_LINKED_LIST)
    if (prev) 
    {
        head = prev->prev;
        return head;
    }
#else
    head = prev;
#endif
    return head;
}

void printList(Node *head)
{
    while (head)
    {
        printf("%d ", head->data);
        head = head->next;
    }
    printf("\n");
}

int main()
{
    Node *head = createNode(3);
    head->next = createNode(5);
#if defined(DOUBLY_LINKED_LIST)
    head->next->prev = head;
#endif

    head->next->next = createNode(2);
#if defined(DOUBLY_LINKED_LIST)
    head->next->next->prev = head->next;
#endif

    head->next->next->next = createNode(1);
#if defined(DOUBLY_LINKED_LIST)
    head->next->next->next->prev = head->next->next;
#endif

    head = insertNode(head, 3, 20);
    printList(head);
    head = deleteNode(head, 2);
    printList(head);
    head = reverseList(head);
    printList(head);
}
