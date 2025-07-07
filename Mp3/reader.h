#define READER_H
#include "tags.h"
#include<stdio.h>
#define MAX_CHARACTERS 10
typedef struct READER
{
    char* src_file_name;
    FILE* fptr_file;
    uint title_size;
    char* title;
    char* ptr[6];
}reader;

Status read_and_validate_view_args(char *argv[], reader *redInfo);

Status get_data(reader* redInfo);

Status open_files(reader *redInfo);

Status check_tag(FILE* fptr_file, reader* redInfo);

Status check_title_size(reader* redInfo,FILE* fptr_file);

Status get_title_data(reader* redInfo,FILE* fptr_file); 

Status get_album_data_size(reader* redInfo,FILE* fptr_file);

Status get_album_data(reader* redInfo,FILE* fptr_file);