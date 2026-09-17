#include "text_editor.h"


/*
 * =========================================================
 *              CREATE HISTORY STATE
 * =========================================================
 */

History *create_history_state(TextEditor *texteditor)
{
    History *history;

    Node *current;
    Node *newNode;
    Node *last = NULL;

    int line_number = 1;


    if (texteditor == NULL)
    {
        return NULL;
    }


    /*
     * Allocate history structure.
     */
    history = malloc(sizeof(History));


    if (history == NULL)
    {
        return NULL;
    }


    /*
     * Initialize.
     */
    history->head = NULL;
    history->tail = NULL;
    history->cursor = NULL;

    history->current_line = texteditor->current_line;

    history->current_column = texteditor->current_column;

    history->total_characters = texteditor->total_characters;

    history->next = NULL;


    /*
     * Copy complete document.
     */
    current = texteditor->head;


    while (current != NULL)
    {
        newNode = malloc(sizeof(Node));


        if (newNode == NULL)
        {
            free_history_state(history);

            return NULL;
        }


        newNode->line = malloc(current->length + 1);


        if (newNode->line == NULL)
        {
            free(newNode);

            free_history_state(history);

            return NULL;
        }


        strcpy(newNode->line, current->line);


        newNode->length = current->length;


        newNode->prev = last;

        newNode->next = NULL;


        /*
         * First node.
         */
        if (history->head == NULL)
        {
            history->head = newNode;
        }
        else
        {
            last->next = newNode;
        }


        last = newNode;


        /*
         * If this node corresponds to
         * the editor cursor, remember it.
         */
        if (line_number == texteditor->current_line)
        {
            history->cursor = newNode;
        }


        line_number++;

        current = current->next;
    }


    history->tail = last;


    /*
     * Empty document.
     */
    if (texteditor->head == NULL)
    {
        history->cursor = NULL;
    }


    return history;
}


/*
 * =========================================================
 *                     SAVE STATE
 * =========================================================
 */

int save_state(TextEditor *texteditor, History **stack)
{
    History *newHistory;


    if (texteditor == NULL ||
        stack == NULL)
    {
        return FAILURE;
    }


    newHistory = create_history_state(texteditor);


    if (newHistory == NULL)
    {
        printf("ERROR: Unable to save editor state.\n");

        return FAILURE;
    }


    /*
     * Push onto stack.
     */
    newHistory->next = *stack;

    *stack = newHistory;


    return SUCCESS;
}


/*
 * =========================================================
 *                FREE HISTORY STATE
 * =========================================================
 */

void free_history_state(History *history)
{
    Node *current;
    Node *next;


    if (history == NULL)
    {
        return;
    }


    current = history->head;


    while (current != NULL)
    {
        next = current->next;


        free(current->line);

        free(current);


        current = next;
    }


    free(history);
}


/*
 * =========================================================
 *                     CLEAR STACK
 * =========================================================
 */

void clear_stack(History **stack)
{
    History *current;
    History *next;


    if (stack == NULL)
    {
        return;
    }


    current = *stack;


    while (current != NULL)
    {
        next = current->next;


        free_history_state(current);


        current = next;
    }


    *stack = NULL;
}


/*
 * =========================================================
 *                FREE EDITOR DOCUMENT
 * =========================================================
 */

void free_editor_document(TextEditor *texteditor)
{
    Node *current;
    Node *next;


    if (texteditor == NULL)
    {
        return;
    }


    current = texteditor->head;


    while (current != NULL)
    {
        next = current->next;


        free(current->line);

        free(current);


        current = next;
    }


    texteditor->head = NULL;
    texteditor->tail = NULL;
    texteditor->cursor = NULL;
}


/*
 * =========================================================
 *                  RESTORE STATE
 * =========================================================
 */

void restore_state(TextEditor *texteditor,
                   History *history)
{
    Node *current;
    Node *newNode;
    Node *last = NULL;

    int line_number = 1;


    if (texteditor == NULL ||
        history == NULL)
    {
        return;
    }


    /*
     * Remove current document.
     */
    free_editor_document(texteditor);


    /*
     * Re-create document from history.
     */
    current = history->head;


    while (current != NULL)
    {
        newNode = malloc(sizeof(Node));


        if (newNode == NULL)
        {
            printf("ERROR: Memory allocation failed.\n");

            return;
        }


        newNode->line = malloc(current->length + 1);


        if (newNode->line == NULL)
        {
            free(newNode);

            printf("ERROR: Memory allocation failed.\n");

            return;
        }


        strcpy(newNode->line, current->line);


        newNode->length = current->length;


        newNode->prev = last;

        newNode->next = NULL;


        if (texteditor->head == NULL)
        {
            texteditor->head = newNode;
        }
        else
        {
            last->next = newNode;
        }


        /*
         * Restore cursor by line number.
         */
        if (line_number == history->current_line)
        {
            texteditor->cursor = newNode;
        }


        last = newNode;

        line_number++;

        current = current->next;
    }


    /*
     * Restore tail.
     */
    texteditor->tail = last;


    /*
     * Restore editor information.
     */
    texteditor->current_line =  history->current_line;

    texteditor->current_column = history->current_column;

    texteditor->total_characters = history->total_characters;


    /*
     * Empty document.
     */
    if (texteditor->head == NULL)
    {
        texteditor->cursor = NULL;

        texteditor->current_line = 0;

        texteditor->current_column = 0;
    }
}


/*
 * =========================================================
 *                        UNDO
 * =========================================================
 */

int undo(TextEditor *texteditor)
{
    History *state;


    if (texteditor == NULL)
    {
        return FAILURE;
    }


    /*
     * Nothing to undo.
     */
    if (texteditor->undo_stack == NULL)
    {
        printf("ERROR: Nothing to undo.\n");

        return FAILURE;
    }


    /*
     * Save current state into REDO.
     */
    if (save_state(texteditor,  &texteditor->redo_stack) == FAILURE)
    {
        return FAILURE;
    }


    /*
     * Get top UNDO state.
     */
    state = texteditor->undo_stack;


    /*
     * Pop state.
     */
    texteditor->undo_stack = state->next;


    /*
     * Restore old state.
     */
    restore_state(texteditor, state);


    /*
     * Free history state.
     */
    free_history_state(state);


    printf("INFO: Undo Successful.\n");

    return SUCCESS;
}


/*
 * =========================================================
 *                        REDO
 * =========================================================
 */

int redo(TextEditor *texteditor)
{
    History *state;


    if (texteditor == NULL)
    {
        return FAILURE;
    }


    /*
     * Nothing to redo.
     */
    if (texteditor->redo_stack == NULL)
    {
        printf("ERROR: Nothing to redo.\n");

        return FAILURE;
    }


    /*
     * Save current state into UNDO.
     */
    if (save_state(texteditor,
                   &texteditor->undo_stack) == FAILURE)
    {
        return FAILURE;
    }


    /*
     * Get top REDO state.
     */
    state =
        texteditor->redo_stack;


    /*
     * Pop state.
     */
    texteditor->redo_stack =
        state->next;


    /*
     * Restore state.
     */
    restore_state(texteditor,
                  state);


    /*
     * Free history state.
     */
    free_history_state(state);


    printf("INFO: Redo Successful.\n");

    return SUCCESS;
}