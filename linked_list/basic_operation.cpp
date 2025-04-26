#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct _Node
{
    int data;
    struct _Node *next;
} Node;

Node* createNode(int data)
{
    Node *newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = NULL;
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
        free(curr);
        return head;
    }

    for (int p = 1; (p < pos) && curr; p++)
    {
        prev = curr;
        curr = curr->next;
    }

    if (curr)
    {
        prev->next = curr->next;
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
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    return prev;
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
    head->next->next = createNode(2);
    head->next->next->next = createNode(1);
    head = insertNode(head, 5, 20);
    printList(head);
    head = deleteNode(head, 5);
    printList(head);
    head = reverseList(head);
    printList(head);
}
