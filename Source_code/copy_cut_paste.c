#include "text_editor.h"

/*
 *  COPY TEXT
 */

int copy_text(TextEditor *texteditor, char argument[])
{
    int num_char;
    char *result;


    if (texteditor == NULL || argument == NULL)
    {
        return FAILURE;
    }


    num_char = get_number_from_string(argument);


    if (num_char <= 0)
    {
        printf("ERROR: Number of characters must be greater than 0.\n");

        return FAILURE;
    }


    if (texteditor->cursor == NULL)
    {
        printf("ERROR: No text available.\n");

        return FAILURE;
    }


    if (num_char > texteditor->current_column)
    {
        printf("ERROR: Cannot copy %d characters. Only %d characters available before cursor.\n",num_char,texteditor->current_column);

        return FAILURE;
    }


    if (get_copy_text(texteditor,num_char, &result) == FAILURE)
    {
        return FAILURE;
    }


    free(texteditor->clipboard);


    texteditor->clipboard = result;

    texteditor->clipboard_length = num_char;


    printf("INFO: Copy Successful.\n");

    return SUCCESS;
}


/*
 * CUT TEXT
 */

int cut_text(TextEditor *texteditor, char argument[])
{
    int num_char;


    if (texteditor == NULL || argument == NULL)
    {
        return FAILURE;
    }


    num_char = get_number_from_string(argument);


    if (num_char <= 0)
    {
        printf("ERROR: Number of characters must be greater than 0.\n");

        return FAILURE;
    }


    if (texteditor->cursor == NULL)
    {
        printf("ERROR: No text available.\n");

        return FAILURE;
    }


    if (num_char > texteditor->current_column)
    {
        printf("ERROR: Cannot cut %d characters.Only %d characters available before cursor.\n",num_char,texteditor->current_column);

        return FAILURE;
    }


    /*
     * Copy first.
     */
    if (copy_text(texteditor, argument) == FAILURE)
    {
        return FAILURE;
    }


    /*
     * Delete copied text.
     */
    if (delete(texteditor, argument) == FAILURE)
    {
        return FAILURE;
    }


    printf("INFO: Cut Successful.\n");

    return SUCCESS;
}


/*
 * PASTE TEXT
 */

int paste_text(TextEditor *texteditor)
{
    if (texteditor == NULL)
    {
        return FAILURE;
    }


    if (texteditor->clipboard == NULL || texteditor->clipboard_length == 0)
    {
        printf("ERROR: Clipboard is empty.\n");

        return FAILURE;
    }


    if (insert(texteditor, texteditor->clipboard) == FAILURE)
    {
        return FAILURE;
    }


    printf("INFO: Paste Successful.\n");

    return SUCCESS;
}