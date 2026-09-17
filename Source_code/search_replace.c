#include "text_editor.h"

/*
 * SEARCH TEXT
 */

int search_text(TextEditor *texteditor, char argument[])
{
    Node *current;
    char *position;

    int line_number = 1;
    int column;


    if (texteditor == NULL || argument == NULL || argument[0] == '\0')
    {
        return FAILURE;
    }


    if (texteditor->head == NULL)
    {
        printf("ERROR: No text available.\n");

        return FAILURE;
    }


    current = texteditor->head;


    while (current != NULL)
    {
        position = current->line;


        while ((position = strstr(position, argument)) != NULL)
        {
            column = position - current->line;


            printf("FOUND: \"%s\" at Line %d, Column %d\n",argument,line_number, column);


            position++;
        }


        current = current->next;

        line_number++;
    }


    return SUCCESS;
}


/*
 * REPLACE TEXT
 */

int replace_text(TextEditor *texteditor, char argument[])
{
    char old_text[256];
    char new_text[256];

    char *space;

    Node *current;

    int found = 0;
    int result;


    if (texteditor == NULL ||argument == NULL ||argument[0] == '\0')
    {
        return FAILURE;
    }


    space = strchr(argument, ' ');


    if (space == NULL)
    {
        printf("ERROR: Replace requires old and new text.\n");

        return FAILURE;
    }


    *space = '\0';

    strcpy(old_text, argument);

    strcpy(new_text, space + 1);


    if (old_text[0] == '\0' || new_text[0] == '\0')
    {
        printf("ERROR: Invalid replace arguments.\n");

        return FAILURE;
    }


    if (texteditor->head == NULL)
    {
        printf("ERROR: No text available.\n");

        return FAILURE;
    }


    /*
     * Check whether anything exists to replace.
     */
    current = texteditor->head;


    while (current != NULL)
    {
        if (strstr(current->line, old_text) != NULL)
        {
            found = 1;
            break;
        }

        current = current->next;
    }


    if (found == 0)
    {
        printf("INFO: Text not found.\n");

        return FAILURE;
    }


    /*
     * Save state once for the complete replace operation.
     */
    if (save_state(texteditor, &texteditor->undo_stack) == FAILURE)
    {
        return FAILURE;
    }


    clear_stack(&texteditor->redo_stack);


    current = texteditor->head;


    while (current != NULL)
    {
        result = replace_line(&current->line,&current->length,old_text,new_text);


        if (result > 0)
        {
            texteditor->total_characters += result;
        }


        current = current->next;
    }


    printf("INFO: Replace Successful.\n");

    return SUCCESS;
}