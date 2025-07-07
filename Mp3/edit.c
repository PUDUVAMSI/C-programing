#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "edit.h"
Status read_and_validate_edit_args(char *argv[], editor *editInfo)
{
    if(strstr(argv[4],".mp3")!=NULL)                                //calling read_and_validate_edit_args function
    {
        editInfo->source_name = argv[4];
    }
    else
    {
        printf("ERROR: Source file not opened succesfully\n");
        return mp_failure;
    }
    editInfo->data = argv[3];
    editInfo->data_len = strlen(editInfo->data);                    //finding the length of the data which user will enter in CLA
    editInfo->tag = argv[2];   
    return mp_success;
}
Status open_file(editor *editInfo)                                  //calling open_file function
{
    editInfo->fptr_sample_file = fopen(editInfo->source_name,"r");          //opening our source file name
    if(editInfo->fptr_sample_file == NULL)
    {
        printf("ERROR: File %s is not present",editInfo->source_name);
        return mp_failure;
    }
    editInfo->dupli_name = "duplicate.mp3";
    editInfo->fptr_dupli_file = fopen(editInfo->dupli_name,"wb");           //opening our duplicate file name
    if(editInfo->fptr_dupli_file == NULL)
    {
        printf("ERROR: File %s is not present",editInfo->dupli_name);
        fclose(editInfo->fptr_sample_file);
        return mp_failure;
    }
    return mp_success;

}
Status check_edit_tag(FILE* fptr_sample_file, editor* editInfo)                 //calling check_edit-tag
{
    char buffer[5];
    fread(buffer,4,1,fptr_sample_file);                         //reading first_found tag from the source file
    buffer[4] =  '\0';
    fwrite(buffer,4,1,editInfo->fptr_dupli_file);               //writing first_found tag to the duplicate file
    //printf("res-->%s\n",buffer);
    if(strcmp(buffer,editInfo->tag_name)==0)
    {
        return mp_success;
    }
    return mp_failure;

}
Status insert_edit_size(editor* editInfo,FILE* fptr_dupli_file)         //calling insert data from the source file
{
    //editor->data_len
    char buffer[4];
    uint len = editInfo->data_len +1;
    buffer[0] = (len>> 24) & 0xFF;  
    buffer[1] = (len >> 16) & 0xFF;
    buffer[2] = (len >> 8) & 0xFF;
    buffer[3] = len & 0xFF;
    fwrite(buffer,1,4,fptr_dupli_file);
    return mp_success;          
}
Status insert_edit_data(editor* editInfo,FILE* fptr_dupli_file)
{
    char buffer[editInfo->data_len];
    fwrite(editInfo->data,editInfo->data_len,1,fptr_dupli_file);
    return mp_success;
}
Status get_size(editor* editInfo,FILE* fptr_sample_file)
{
    uint size;
    char buffer[4];
    fread(buffer,4,1,fptr_sample_file);
    fwrite(buffer,4,1,editInfo->fptr_dupli_file);
    for(int i=0;i<2;i++) //loop converting number from big endian to littlt endian from buffer
   {
    char temp=buffer[i];
    buffer[i]=buffer[4-i-1];
    buffer[4-i-1]=temp;
   }
   size=buffer[0]|buffer[1]|buffer[2]|buffer[3];
   //printf("size ----> %u\n",size);
    editInfo->required_size = size;
    return mp_success;
}
Status get__data(FILE* fptr_sample_file,FILE* fptr_dupli_file)
{
    char buffer[1];
    while(fread(buffer,1,1,fptr_sample_file)>0)
    {
        fwrite(buffer,1,1,fptr_dupli_file);
    }
    return mp_success;
}
Status get_org_size(editor* editInfo,FILE* fptr_sample_file)
{
    uint size=0;
    char buffer[4];
    fread(buffer,4,1,fptr_sample_file);
   for(int i=0;i<2;i++) //loop converting number from big endian to littlt endian from buffer
   {
    char temp=buffer[i];
    buffer[i]=buffer[4-i-1];
    buffer[4-i-1]=temp;
   }
   size=buffer[0]|buffer[1]|buffer[2]|buffer[3];
    editInfo->size_data=size; //store the number after converting it
    return mp_success;
}
Status modify_change(editor *editInfo)
{
    if(open_file(editInfo)==mp_failure)
    {
        printf("files not opened succesfully\n");
        return mp_failure;
    }
    //printf("entered\n");
    char* ptr[6] = {"TIT2","TPE1","TALB","TYER","TCON","COMM"};           //storing my tags in array of pointers
    for(int i=0;i<6;i++)
    {
        editInfo->ptr_edit[i] = ptr[i];
    }
    //printf("success->tags\n");
    //-t,-a,-A,-m,-y,-c
    int index=0;
    if(strcmp(editInfo->tag,"-t")==0)
    {
        index= 0;
    }
    else if(strcmp(editInfo->tag,"-a")==0)
    {
        index= 1;
    }
    else if(strcmp(editInfo->tag,"-A")==0)
    {
        index= 2;
    }
    else if(strcmp(editInfo->tag,"-y")==0)
    {
        index= 3;
    }
    else if(strcmp(editInfo->tag,"-m")==0)
    {
        index= 4;
    }
    else if(strcmp(editInfo->tag,"-c")==0)
    {
        index= 5;
    }
    else{
        printf("ERROR: Enter valid tag\n");
        return mp_failure;
    }
    //printf("reading_success\n");
    //printf("%s\n",editInfo->ptr_edit[index]);
    editInfo->tag_name=editInfo->ptr_edit[index];
    //printf("your tag ---->%s\n",editInfo->tag_name);
    char buffer[10];
    fread(buffer,10,1,editInfo->fptr_sample_file);
    fwrite(buffer,10,1,editInfo->fptr_dupli_file);
    //printf("wrtiring 10 success\n");
    for(int i=0;i<6;i++)
    {
        if(check_edit_tag(editInfo->fptr_sample_file,editInfo)==mp_success)
        {
            get_org_size(editInfo,editInfo->fptr_sample_file);
            printf("Tag check succesfull\n");
            if(insert_edit_size(editInfo,editInfo->fptr_dupli_file)==mp_failure)
            {
                printf("ERROR: size not inserted\n");
                return mp_failure;
            }
            char buffer[3];
            fread(buffer,3,1,editInfo->fptr_sample_file);
            fwrite(buffer,3,1,editInfo->fptr_dupli_file);
            //printf("3 bytes check success\n");
            if(insert_edit_data(editInfo,editInfo->fptr_dupli_file)==mp_failure)
            {
                printf("ERROR: data not inserted\n");
                return mp_failure;
            }
            fseek(editInfo->fptr_sample_file,editInfo->size_data-1,SEEK_CUR);
            break;
        }
        if(get_size(editInfo,editInfo->fptr_sample_file)==mp_failure)
        {
                printf("size not copied\n");
                return mp_failure;
        }
        char buffer_1[3];
        fread(buffer_1,3,1,editInfo->fptr_sample_file);
        fwrite(buffer_1,3,1,editInfo->fptr_dupli_file);
        char data_buffer[editInfo->required_size];
        fread(data_buffer,editInfo->required_size-1,1,editInfo->fptr_sample_file);
        fwrite(data_buffer,editInfo->required_size-1,1,editInfo->fptr_dupli_file);
    }
    //fseek(editInfo->fptr_sample_file,editInfo->data_len+4,SEEK_CUR);
    if(get__data(editInfo->fptr_sample_file,editInfo->fptr_dupli_file)==mp_failure)
    {
        printf("remaining data not copied\n");
        return mp_failure;
    }
    rename(editInfo->dupli_name,editInfo->source_name);
    printf("SUCCESS: Data modified Successfully\n");
    return mp_success;
}