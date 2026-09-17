#include "text_editor.h"


/*
 *                     INSERT FUNCTION
 */

int insert(TextEditor *texteditor, char argument[])
{
    int insert_length;
    int old_length;
    int new_length;

    char *temp;

    /*
     * Validate editor
     */
    if (texteditor == NULL)
    {
        printf("ERROR: Invalid text editor.\n");
        return FAILURE;
    }

    /*
     * Validate argument
     */
    if (argument == NULL || argument[0] == '\0')
    {
        printf("ERROR: Text cannot be empty.\n");
        return FAILURE;
    }

    insert_length = strlen(argument);


    /*
     * CASE 1:
     * No line exists.
     * Create first line.
     */

    if (texteditor->head == NULL)
    {
        Node *newNode;

        /*
         * Save state for UNDO
         *
         * We save the empty document before insertion.
         */
        if (save_state(texteditor, &texteditor->undo_stack) == FAILURE)
        {
            return FAILURE;
        }

        /*
         * New operation clears REDO.
         */
        clear_stack(&texteditor->redo_stack);


        newNode = malloc(sizeof(Node));

        if (newNode == NULL)
        {
            printf("ERROR: Memory allocation failed.\n");
            return FAILURE;
        }


        newNode->line = malloc(insert_length + 1);

        if (newNode->line == NULL)
        {
            free(newNode);

            printf("ERROR: Memory allocation failed.\n");
            return FAILURE;
        }


        strcpy(newNode->line, argument);

        newNode->length = insert_length;

        newNode->prev = NULL;
        newNode->next = NULL;


        texteditor->head = newNode;
        texteditor->tail = newNode;
        texteditor->cursor = newNode;

        texteditor->current_line = 1;
        texteditor->current_column = insert_length;

        texteditor->total_characters = insert_length;


        printf("INFO: Insert Successful.\n");

        return SUCCESS;
    }


    /*
     * CASE 2:
     * Insert into existing current line.
     */


    /*
     * Save current state before modification.
     */
    if (save_state(texteditor, &texteditor->undo_stack) == FAILURE)
    {
        return FAILURE;
    }

    /*
     * New operation clears REDO.
     */
    clear_stack(&texteditor->redo_stack);


    old_length = texteditor->cursor->length;

    new_length = old_length + insert_length;


    /*
     * Increase memory for line.
     */
    temp = realloc(texteditor->cursor->line,
                   new_length + 1);

    if (temp == NULL)
    {
        printf("ERROR: Memory reallocation failed.\n");

        return FAILURE;
    }


    texteditor->cursor->line = temp;


    /*
     * Create space for new string.
     */
    create_space(texteditor->cursor->line,
                 old_length,
                 texteditor->current_column,
                 insert_length);


    /*
     * Insert new string.
     */
    insert_string(texteditor->cursor->line,
                  argument,
                  texteditor->current_column);


    /*
     * Update node information.
     */
    texteditor->cursor->length = new_length;


    /*
     * Update editor information.
     */
    texteditor->total_characters += insert_length;

    texteditor->current_column += insert_length;


    printf("INFO: Insert Successful.\n");

    return SUCCESS;
}








/*
 * DISPLAY
 */

int print(TextEditor texteditor)
{
    Node *temp;
    int i = 1;


    if (texteditor.head == NULL)
    {
        printf("No Lines to print\n");

        printf("Cursor position : Line 0, Column 0\n");

        return SUCCESS;
    }


    temp = texteditor.head;


    while (temp != NULL)
    {
        printf("LINE %-3d %s\n",
               i,
               temp->line);

        temp = temp->next;

        i++;
    }


    printf("Cursor position : Line %-3d, Column %d\n",
           texteditor.current_line,
           texteditor.current_column);


    return SUCCESS;
}

