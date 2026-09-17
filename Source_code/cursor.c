#include "text_editor.h"


/*
 *                    LEFT MOVEMENT
 */

int move_left(TextEditor *texteditor)
{
    if (texteditor == NULL || texteditor->cursor == NULL)
    {
        printf("ERROR: No text available.\n");

        return FAILURE;
    }


    /*
     * Cursor is inside current line.
     */
    if (texteditor->current_column > 0)
    {
        texteditor->current_column--;

        return SUCCESS;
    }


    /*
     * Cursor is at beginning of current line.
     *
     * Move to previous line.
     */
    if (texteditor->cursor->prev != NULL)
    {
        texteditor->cursor = texteditor->cursor->prev;

        texteditor->current_line--;

        texteditor->current_column = texteditor->cursor->length;

        return SUCCESS;
    }


    /*
     * Already at beginning of document.
     */
    printf("INFO: Already at the beginning of the document.\n");

    return FAILURE;
}


/*
 *                    RIGHT MOVEMENT
 */

int move_right(TextEditor *texteditor)
{
    if (texteditor == NULL || texteditor->cursor == NULL)
    {
        printf("ERROR: No text available.\n");

        return FAILURE;
    }


    /*
     * Cursor is inside current line.
     */
    if (texteditor->current_column < texteditor->cursor->length)
    {
        texteditor->current_column++;

        return SUCCESS;
    }


    /*
     * Cursor is at end of current line.
     *
     * Move to next line.
     */
    if (texteditor->cursor->next != NULL)
    {
        texteditor->cursor = texteditor->cursor->next;

        texteditor->current_line++;

        texteditor->current_column = 0;

        return SUCCESS;
    }


    /*
     * Already at end of document.
     */
    printf("INFO: Already at the end of the document.\n");

    return FAILURE;
}


/*
 *                    UP MOVEMENT
 */

int move_up(TextEditor *texteditor)
{
    int old_column;


    if (texteditor == NULL || texteditor->cursor == NULL)
    {
        printf("ERROR: No text available.\n");

        return FAILURE;
    }


    /*
     * No previous line.
     */
    if (texteditor->cursor->prev == NULL)
    {
        printf("INFO: Already at the first line.\n");

        return FAILURE;
    }


    /*
     * Save current column.
     */
    old_column = texteditor->current_column;


    /*
     * Move to previous line.
     */
    texteditor->cursor = texteditor->cursor->prev;

    texteditor->current_line--;


    /*
     * If previous line is shorter,
     * move cursor to its end.
     */
    if (old_column > texteditor->cursor->length)
    {
        texteditor->current_column = texteditor->cursor->length;
    }
    else
    {
        texteditor->current_column = old_column;
    }


    return SUCCESS;
}


/*
 *                    DOWN MOVEMENT
 */

int move_down(TextEditor *texteditor)
{
    int old_column;


    if (texteditor == NULL || texteditor->cursor == NULL)
    {
        printf("ERROR: No text available.\n");

        return FAILURE;
    }


    /*
     * No next line.
     */
    if (texteditor->cursor->next == NULL)
    {
        printf("INFO: Already at the last line.\n");

        return FAILURE;
    }


    /*
     * Save current column.
     */
    old_column = texteditor->current_column;


    /*
     * Move to next line.
     */
    texteditor->cursor = texteditor->cursor->next;

    texteditor->current_line++;


    /*
     * If next line is shorter,
     * move cursor to its end.
     */
    if (old_column > texteditor->cursor->length)
    {
        texteditor->current_column = texteditor->cursor->length;
    }
    else
    {
        texteditor->current_column = old_column;
    }


    return SUCCESS;
}