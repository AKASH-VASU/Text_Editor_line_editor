#include "text_editor.h"


/*
 * DELETE FUNCTION
 */

int delete(TextEditor *texteditor, char arguments[])
{
    int num_char;
    int remaining;
    int available;
    int delete_count;

    int old_length;
    int new_length;
    int start;
    int i;

    char *temp;

    Node *current;
    Node *prev;


   
    // validate input
    if (texteditor == NULL ||
        arguments == NULL)
    {
        return FAILURE;
    }


    // char to nnum
    num_char = get_number_from_string(arguments);


    // validating number
    if (num_char <= 0)
    {
        printf("ERROR: Number of characters must be greater than 0.\n");

        return FAILURE;
    }


    /*
     * Check whether text exists.
     */
    if (texteditor->cursor == NULL)
    {
        printf("ERROR: No text available.\n");

        return FAILURE;
    }

    current = texteditor->cursor;

    available = texteditor->current_column;

    prev = current->prev;


    while (prev != NULL)
    {
        available += prev->length;

        prev = prev->prev;
    }


    /*
     * Requested delete amount cannot exceed available
     * characters.
     */
    if (num_char > available)
    {
        printf("ERROR: Cannot delete %d characters. "
               "Only %d characters available before cursor.\n",
               num_char,
               available);

        return FAILURE;
    }


    /*
     * Save state for UNDO.
     */

    if (save_state(texteditor,
                   &texteditor->undo_stack) == FAILURE)
    {
        return FAILURE;
    }


    /*
     * New operation clears REDO.
     */
    clear_stack(&texteditor->redo_stack);


    remaining = num_char;


    /*
     * DELETE BACKWARD
     */

    while (remaining > 0)
    {
        current = texteditor->cursor;


        /*
         * Characters available before cursor in current line.
         */
        available = texteditor->current_column;


        /*
         * CASE 1:
         *
         * Enough characters are available in current line.
         */

        if (remaining <= available)
        {
            old_length = current->length;


            /*
             * Starting index of deleted region.
             */
            start =
                texteditor->current_column -
                remaining;


            new_length =
                old_length -
                remaining;


            /*
             * Shift remaining characters left.
             *
             * +1 also moves '\0'.
             */
            for (i = start;
                 i <= old_length;
                 i++)
            {
                current->line[i] =
                    current->line[i + remaining];
            }


            /*
             * Reduce memory.
             */
            temp = realloc(current->line,
                           new_length + 1);


            if (temp == NULL)
            {
                printf("ERROR: Memory reallocation failed.\n");

                return FAILURE;
            }


            current->line = temp;

            current->length = new_length;


            /*
             * Move cursor backward.
             */
            texteditor->current_column = start;


            /*
             * Update total characters.
             */
            texteditor->total_characters -= remaining;


            remaining = 0;
        }


        /*
         * CASE 2:
         *
         * Not enough characters in current line.
         *
         * Move to previous line.
         */

        else
        {
            delete_count = available;


            remaining -= delete_count;

            texteditor->total_characters -=
                delete_count;


            /*
             * Remove characters before cursor.
             */
            if (delete_count > 0)
            {
                current->line[0] = '\0';


                temp = realloc(current->line, 1);


                if (temp == NULL)
                {
                    printf("ERROR: Memory reallocation failed.\n");

                    return FAILURE;
                }


                current->line = temp;

                current->length = 0;
            }


            /*
             * Move to previous line.
             */
            prev = current->prev;


            /*
             * Previous line exists.
             */
            if (prev != NULL)
            {
                prev->next = current->next;


                if (current->next != NULL)
                {
                    current->next->prev = prev;
                }


                if (texteditor->tail == current)
                {
                    texteditor->tail = prev;
                }


                free(current->line);

                free(current);


                /*
                 * Cursor moves to previous line.
                 */
                texteditor->cursor = prev;

                texteditor->current_line--;

                texteditor->current_column =
                    prev->length;
            }


            /*
             * No previous line.
             */
            else
            {
                texteditor->head = current->next;


                if (current->next != NULL)
                {
                    current->next->prev = NULL;
                }
                else
                {
                    texteditor->tail = NULL;
                }


                free(current->line);

                free(current);


                texteditor->cursor =
                    texteditor->head;


                if (texteditor->head == NULL)
                {
                    texteditor->current_line = 0;

                    texteditor->current_column = 0;
                }
                else
                {
                    texteditor->current_line = 1;

                    texteditor->current_column = 0;
                }
            }
        }
    }


    printf("INFO: Delete Successful.\n");

    return SUCCESS;
}