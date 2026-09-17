#include "text_editor.h"


/*
 *  GET NUMBER FROM STRING
 */

int get_number_from_string(char arguments[])
{
    int number = 0;
    int i;


    if (arguments == NULL)
    {
        return -1;
    }


    for (i = 0; arguments[i] != '\0'; i++)
    {
        /*
         * Check whether character is a digit.
         */
        if (arguments[i] < '0' || arguments[i] > '9')
        {
            return -1;
        }


        number = number * 10 + (arguments[i] - '0');
    }


    return number;
}


/*
 *  CREATE SPACE
 */

void create_space(char line[],int old_length, int current_column, int insert_length)
{
    int i;


    for (i = old_length;i >= current_column;i--)
    {
        line[i + insert_length] = line[i];
    }
}



void insert_string(char line[], char argument[], int current_column)
{
    int i;


    for (i = 0;argument[i] != '\0';i++)
    {
        line[current_column + i] = argument[i];
    }
}


/*
 * =========================================================
 *                     CREATE NODE
 * =========================================================
 */

Node *create_node(char line[])
{
    Node *newNode;
    int length;

    if (line == NULL)
    {
        return NULL;
    }

    length = strlen(line);

    newNode = malloc(sizeof(Node));

    if (newNode == NULL)
    {
        return NULL;
    }

    newNode->line = malloc(length + 1);

    if (newNode->line == NULL)
    {
        free(newNode);
        return NULL;
    }

    strcpy(newNode->line, line);

    newNode->length = length;
    newNode->prev = NULL;
    newNode->next = NULL;

    return newNode;
}


/*
 * =========================================================
 *                  REMOVE NEWLINE
 * =========================================================
 */

void remove_newline(char line[])
{
    int length;

    if (line == NULL)
    {
        return;
    }

    length = strlen(line);

    if (length > 0 && line[length - 1] == '\n')
    {
        line[length - 1] = '\0';
    }

    length = strlen(line);

    if (length > 0 && line[length - 1] == '\r')
    {
        line[length - 1] = '\0';
    }
}

/*
 * =========================================================
 *                 COUNT OCCURRENCES
 * =========================================================
 */

int count_occurrences(char line[], char search[])
{
    int count = 0;

    char *position;


    if (line == NULL || search == NULL || search[0] == '\0')
    {
        return 0;
    }


    position = line;


    while ((position = strstr(position, search)) != NULL)
    {
        count++;

        position += strlen(search);
    }


    return count;
}


/*
 * =========================================================
 *                    REPLACE LINE
 * =========================================================
 */

int replace_line(char **line,int *length,char old[],char new[])
{
    int old_length;
    int new_length;

    int count;
    int result_length;

    int i;
    int j;

    char *result;

    char *position;


    if (line == NULL || *line == NULL || length == NULL || old == NULL || new == NULL)
    {
        return 0;
    }


    old_length = strlen(old);
    new_length = strlen(new);


    if (old_length == 0)
    {
        return 0;
    }


    count = count_occurrences(*line, old);


    if (count == 0)
    {
        return 0;
    }


    result_length = *length + count * (new_length - old_length);


    result = malloc(result_length + 1);


    if (result == NULL)
    {
        return 0;
    }


    position = *line;

    i = 0;
    j = 0;


    while (*position != '\0')
    {
        if (strncmp(position,old,old_length) == 0)
        {
            /*
             * Copy replacement string
             * character by character.
             */
            for (int k = 0; k < new_length; k++)
            {
                result[j++] = new[k];
            }


            position += old_length;
        }
        else
        {
            result[j++] = *position;

            position++;
        }
    }


    result[j] = '\0';


    free(*line);


    *line = result;

    *length = result_length;


    return count * (new_length - old_length);
}


/*
 * =========================================================
 *                  GET COPY TEXT
 * =========================================================
 */

int get_copy_text(TextEditor *texteditor, int num_char, char **result)
{
    int start;
    int i;


    if (texteditor == NULL || texteditor->cursor == NULL || result == NULL)
    {
        return FAILURE;
    }


    start =
        texteditor->current_column - num_char;


    if (start < 0)
    {
        return FAILURE;
    }


    *result = malloc(num_char + 1);


    if (*result == NULL)
    {
        printf("ERROR: Memory allocation failed.\n");

        return FAILURE;
    }


    /*
     * Copy characters one by one.
     */
    for (i = 0; i < num_char; i++)
    {
        (*result)[i] = texteditor->cursor->line[start + i];
    }


    (*result)[num_char] = '\0';


    return SUCCESS;
}