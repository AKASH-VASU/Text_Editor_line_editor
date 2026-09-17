#include "text_editor.h"


int main()
{
    char input_line_buffer[500];

    char *command;
    char *argument;

    TextEditor texteditor;



    texteditor.head = NULL;
    texteditor.tail = NULL;
    texteditor.cursor = NULL;

    texteditor.current_column = 0;
    texteditor.current_line = 0;

    texteditor.total_characters = 0;

    texteditor.undo_stack = NULL;
    texteditor.redo_stack = NULL;

    texteditor.filename[0] = '\0';
    texteditor.file_open = 0;



    printf("=====================================\n");
    printf("          TEXT EDITOR v1.0\n");
    printf("=====================================\n\n");


    printf("Text Editor Commands:\n");

    printf("insert <text>       - Insert text at cursor\n");
    printf("delete <number>     - Delete characters before cursor\n");
    printf("left                - Move cursor left\n");
    printf("right               - Move cursor right\n");
    printf("up                  - Move cursor up\n");
    printf("down                - Move cursor down\n");
    printf("undo                - Undo last edit\n");
    printf("redo                - Redo last undone edit\n");
    printf("print               - Display text\n");
    printf("open <filename>     - Open a file\n");
    printf("save                - Save current file\n");
    printf("close               - Close current file\n");
    printf("exit                - Exit editor\n");


    while (1)
    {
        printf("\nEnter the Command: ");


        if (fgets(input_line_buffer,sizeof(input_line_buffer),stdin) == NULL)
        {
            break;
        }

        input_line_buffer[strcspn(input_line_buffer, "\n")] = '\0';

        // checking for if there is nothing in buffer

        if (input_line_buffer[0] == '\0')
        {
            continue;
        }

        command = strtok(input_line_buffer, " ");

        argument = NULL;


        if (command != NULL)
        {
            argument = strtok(NULL, "");
        }



        // operation starts here

        if (strcmp(command, "insert") == 0)
        {
            if (argument == NULL)
            {
                printf("ERROR: Insufficient argument for insert.\n");

                continue;
            }


            insert(&texteditor, argument);
        }
        else if (strcmp(command, "delete") == 0)
        {
            if (argument == NULL)
            {
                printf("ERROR: Insufficient argument for delete.\n");

                continue;
            }
            delete(&texteditor, argument);
        }
        else if (strcmp(command, "left") == 0)
        {
            move_left(&texteditor);
        }
        else if (strcmp(command, "right") == 0)
        {
            move_right(&texteditor);
        }
        else if (strcmp(command, "up") == 0)
        {
            move_up(&texteditor);
        }
        else if (strcmp(command, "down") == 0)
        {
            move_down(&texteditor);
        }
        else if (strcmp(command, "undo") == 0)
        {
            undo(&texteditor);
        }
        else if (strcmp(command, "redo") == 0)
        {
            redo(&texteditor);
        }
        else if (strcmp(command, "print") == 0)
        {
            print(texteditor);
        }
        else if (strcmp(command, "exit") == 0)
        {
            break;
        }
        else if (strcmp(command, "open") == 0)
        {
            if (argument == NULL)
            {
                printf("ERROR: Insufficient argument for open.\n");
                continue;
            }

            open_file(&texteditor, argument);
        }

        else if (strcmp(command, "save") == 0) 
        {
            save_file(&texteditor);
        }

        else if (strcmp(command, "close") == 0)
        {
            close_file(&texteditor);
        }
        else if(strcmp(command,"enter") == 0)
        {
            new_line(&texteditor);
        }
        else
        {
            printf("ERROR: Invalid Command.\n");
        }
    }


    /*
     * =====================================================
     * Clean memory before exiting
     * =====================================================
     */

    free_editor_document(&texteditor);

    clear_stack(&texteditor.undo_stack);

    clear_stack(&texteditor.redo_stack);


    printf("\nText editor closed.\n");


    return 0;
}