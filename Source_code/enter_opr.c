#include "text_editor.h"

int new_line(TextEditor *texteditor)
{
    Node *current;
    Node *newNode;

    int left_length;
    int right_length;

    int i;


    if (texteditor == NULL || texteditor->cursor == NULL)
    {
        printf("ERROR: No text available.\n");
        return FAILURE;
    }


    current = texteditor->cursor;


    left_length = texteditor->current_column;

    right_length = current->length - left_length;


    /*
     * Create new node for the right part.
     */
    newNode = malloc(sizeof(Node));

    if (newNode == NULL)
    {
        printf("ERROR: Memory allocation failed.\n");
        return FAILURE;
    }


    newNode->line = malloc(right_length + 1);

    if (newNode->line == NULL)
    {
        free(newNode);

        printf("ERROR: Memory allocation failed.\n");
        return FAILURE;
    }


    /*
     * Copy the right part of current line
     * into the new line.
     */
    for (i = 0; i < right_length; i++)
    {
        newNode->line[i] = current->line[left_length + i];
    }

    newNode->line[right_length] = '\0';

    newNode->length = right_length;


    /*
     * Current line keeps the left part.
     */
    current->line[left_length] = '\0';

    current->length = left_length;


    /*
     * Connect new node after current node.
     */
    newNode->prev = current;

    newNode->next = current->next;


    if (current->next != NULL)
    {
        current->next->prev = newNode;
    }
    else
    {
        texteditor->tail = newNode;
    }


    current->next = newNode;


    /*
     * Move cursor to new line.
     */
    texteditor->cursor = newNode;

    texteditor->current_line++;

    texteditor->current_column = 0;


    printf("INFO: New line created successfully.\n");

    return SUCCESS;
}