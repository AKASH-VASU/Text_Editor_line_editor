#include "text_editor.h"

/*
 * OPEN FILE
 */

int open_file(TextEditor *texteditor, char filename[])
{
    FILE *fp;
    char buffer[1024];
    Node *newNode;
    Node *last;
    int length;

    if (texteditor == NULL || filename == NULL)
    {
        return FAILURE;
    }

    fp = fopen(filename, "r");

    if (fp == NULL)
    {
        printf("ERROR: Unable to open file.\n");
        return FAILURE;
    }

    free_editor_document(texteditor);

    clear_stack(&texteditor->undo_stack);
    clear_stack(&texteditor->redo_stack);

    texteditor->current_line = 0;
    texteditor->current_column = 0;
    texteditor->total_characters = 0;

    last = NULL;

    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        remove_newline(buffer);

        length = strlen(buffer);

        newNode = create_node(buffer);

        if (newNode == NULL)
        {
            fclose(fp);
            free_editor_document(texteditor);

            return FAILURE;
        }

        if (texteditor->head == NULL)
        {
            texteditor->head = newNode;
        }
        else
        {
            last->next = newNode;
            newNode->prev = last;
        }

        last = newNode;

        texteditor->total_characters += length;
    }

    fclose(fp);

    texteditor->tail = last;

    if (texteditor->head != NULL)
    {
        texteditor->cursor = texteditor->tail;
        texteditor->current_line = 1;
        Node *temp = texteditor->head;

        while (temp != texteditor->tail)
        {
            texteditor->current_line++;
            temp = temp->next;
        }

        texteditor->current_column = texteditor->cursor->length;
    }
    else
    {
        texteditor->cursor = NULL;
        texteditor->current_line = 0;
        texteditor->current_column = 0;
    }

    strcpy(texteditor->filename, filename);

    texteditor->file_open = 1;

    printf("INFO: File opened successfully.\n");

    return SUCCESS;
}


/*
 * SAVE FILE
 */

int save_file(TextEditor *texteditor)
{
    FILE *fp;
    Node *current;

    if (texteditor == NULL)
    {
        return FAILURE;
    }

    if (texteditor->file_open == 0)
    {
        printf("ERROR: No file is currently open.\n");
        return FAILURE;
    }

    fp = fopen(texteditor->filename, "w");

    if (fp == NULL)
    {
        printf("ERROR: Unable to save file.\n");
        return FAILURE;
    }

    current = texteditor->head;

    while (current != NULL)
    {
        fprintf(fp, "%s", current->line);

        if (current->next != NULL)
        {
            fprintf(fp, "\n");
        }

        current = current->next;
    }

    fclose(fp);

    printf("INFO: File saved successfully.\n");

    return SUCCESS;
}


/*
 *  CLOSE FILE
 */

int close_file(TextEditor *texteditor)
{
    if (texteditor == NULL)
    {
        return FAILURE;
    }

    if (texteditor->file_open == 0)
    {
        printf("ERROR: No file is currently open.\n");
        return FAILURE;
    }

    free_editor_document(texteditor);

    clear_stack(&texteditor->undo_stack);
    clear_stack(&texteditor->redo_stack);

    texteditor->current_line = 0;
    texteditor->current_column = 0;
    texteditor->total_characters = 0;

    texteditor->filename[0] = '\0';

    texteditor->file_open = 0;

    printf("INFO: File closed successfully.\n");

    return SUCCESS;
}