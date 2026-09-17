#ifndef TEXT_EDITOR_H
#define TEXT_EDITOR_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SUCCESS 0
#define FAILURE -1

typedef struct node
{
    struct node *prev;
    char *line;
    int length;
    struct node *next;
} Node;


typedef struct history
{
    Node *head;
    Node *tail;
    Node *cursor;

    int current_line;
    int current_column;
    int total_characters;

    struct history *next;

} History;


typedef struct texteditor
{
    Node *head;
    Node *tail;
    Node *cursor;

    int current_line;
    int current_column;
    int total_characters;

    History *undo_stack;
    History *redo_stack;

    char filename[256];
    int file_open;

    char *clipboard;
    int clipboard_length;

} TextEditor;


/* INSERT */
int insert(TextEditor *texteditor, char argument[]);

void create_space(char line[], int old_length,int current_column,int insert_length);

void insert_string(char line[],char argument[],int current_column);


/* DELETE */
int delete(TextEditor *texteditor, char argument[]);


/* Misc */
int get_number_from_string(char arguments[]);
int print(TextEditor texteditor);


/* UNDO - REDO */
int undo(TextEditor *texteditor);
int redo(TextEditor *texteditor);

int save_state(TextEditor *texteditor, History **stack);
History *create_history_state(TextEditor *texteditor);

void free_history_state(History *history);
void clear_stack(History **stack);

void free_editor_document(TextEditor *texteditor);
void restore_state(TextEditor *texteditor, History *history);


/* Cursor */
int move_left(TextEditor *texteditor);
int move_right(TextEditor *texteditor);
int move_up(TextEditor *texteditor);
int move_down(TextEditor *texteditor);

/*File operations*/
int open_file(TextEditor *texteditor, char filename[]);
int save_file(TextEditor *texteditor);
int close_file(TextEditor *texteditor);

Node *create_node(char line[]);
void remove_newline(char line[]);


int search_text(TextEditor *texteditor, char argument[]);
int replace_text(TextEditor *texteditor, char argument[]);


int copy_text(TextEditor *texteditor, char argument[]);
int cut_text(TextEditor *texteditor, char argument[]);
int paste_text(TextEditor *texteditor);

int count_occurrences(char line[], char search[]);
int replace_line(char **line, int *length, char old[], char new[]);
int get_copy_text(TextEditor *texteditor, int num_char, char **result);

int new_line(TextEditor *texteditor);

#endif