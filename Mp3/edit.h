#define EDIT_H
#include "tags.h"
#include<stdio.h>
//#define MAX_CHARACTERS 10
typedef struct Editor
{
    char* source_name;
    FILE* fptr_sample_file;
    FILE* fptr_dupli_file;
    char* dupli_name;
    char* data;
    char* tag;
    uint data_len;
    char* tag_name;
    uint dupli_size;
    uint required_size;
    char* title;
    uint size_data;
    char* ptr_edit[6];
}editor;

Status read_and_validate_edit_args(char *argv[], editor *editInfo);

Status modify_change(editor *editInfo);

Status open_file(editor *editInfo);

Status check_edit_tag(FILE* fptr_sample_file, editor* editInfo);

Status insert_edit_size(editor* editInfo,FILE* fptr_dupli_file);

Status insert_edit_data(editor* editInfo,FILE* fptr_dupli_file); 

Status get_size(editor* editInfo,FILE* fptr_sample_file);

Status get__data(FILE* fptr_sample_file,FILE* fptr_dupli_file);

Status get_org_size(editor* editInfo,FILE* fptr_sample_file);