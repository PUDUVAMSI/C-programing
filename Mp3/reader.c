#include <stdio.h>
#include "reader.h"
#include "tags.h"
#include<string.h>
#include<stdlib.h>
Status open_files(reader* redInfo)      //defining open_files function
{
    redInfo->fptr_file= fopen(redInfo->src_file_name,"r");          //opening mp3 file
    if(redInfo->fptr_file==NULL)
    {
     //perror();
     printf("ERROR: FILE IS NOT FOUND\n");
     return mp_failure;
    }
   return mp_success;
}
Status read_and_validate_view_args(char *argv[], reader *redInfo)       //defining read_and_validate_view_args function
{
    if(strstr(argv[2],".mp3")!=NULL)
    {
        redInfo->src_file_name = argv[2];                               //saving mp3 file name in src_file_name
        return mp_success;
    }
    else
    {
        printf("The files you provided is not sufficient\n");
        printf("If you want to guide press ./a.out --help\n");
        return mp_failure;    
    }
}
Status check_tag(FILE* fptr_file,reader* redInfo)       //defining check_tag function
{
    char buffer[5];
    fread(buffer,4,1,fptr_file);
    buffer[4] =  '\0';
   //printf("reims --> %s\n",buffer);
    for(int i=0;i<6;i++)
    {
        if(strcmp(buffer,redInfo->ptr[i])==0)           //comparing the rag with my tags
        {
            return mp_success;                          //if it is found then return
        }
    }
    return mp_failure;
}
Status check_title_size(reader* redInfo,FILE* fptr_file)        //defining check_title_size function
{
    uint size;
    char buffer[4];
    fread(buffer,4,1,fptr_file);
    for(int i=0;i<2;i++) //loop converting number from big endian to littlt endian from buffer
   {
    char temp=buffer[i];
    buffer[i]=buffer[4-i-1];
    buffer[4-i-1]=temp;
   }
   size=buffer[0]|buffer[1]|buffer[2]|buffer[3];       //just converting to little endian
    redInfo-> title_size = size;
    //printf("res-->%u\n",redInfo->title_size);
    //printf("length--->%u\n",redInfo-> title_size);

}
Status get_data(reader *redInfo)                            //calling get_data function
{
    if(open_files(redInfo)==mp_failure)
    {
        printf("file not opened\n");                    //callingopen files function
        return mp_failure;
    }
    //printf("file opened succesfullu\n");
 
    char* p[6] = {"TIT2","TPE1","TALB","TYER","TCON","COMM"};           //storing my tags in array of pointers
    for(int i=0;i<6;i++)
    {
        redInfo->ptr[i] = p[i];
    }
    fseek(redInfo->fptr_file,10,SEEK_SET);                          //shifting my file pointer to 10 bytes
    printf("--------------------------------------------------------------------\n");
    printf("MP3 Tag Reader And Editor for ID3 2.3 version\n");
    printf("--------------------------------------------------------------------\n");
    for(int i=0;i<6;i++)
    {
        if(check_tag(redInfo->fptr_file, redInfo)!=mp_success)          //calling check_tag function
        {
            printf("tag not fetched successfully\n");
            return mp_failure;
        }
        if(check_title_size(redInfo,redInfo->fptr_file)==mp_failure)
        {
            printf("title not fetch succesfuuly\n");                            //calling check_title_size
            return mp_failure;
        }
        fseek(redInfo->fptr_file,3,SEEK_CUR);
        char buffer[redInfo->title_size];
        //printf("size----->%u\n",redInfo->title_size);
        fread(buffer,redInfo->title_size-1,1,redInfo->fptr_file);               //storing data in buffer
        buffer[redInfo->title_size-1]= '\0';
        //printf("get-- -> %s\n",buffer);
        switch(i)
        {
            case 0:
                    printf("TITLE        :    %s\n",buffer);
                    break;                                                  //printing the details the file
            case 1:
                    printf("ARTIST       :    %s\n",buffer);
                    break;
            case 2:
                    printf("ALBUM        :    %s\n",buffer);
                    break;
            case 3:
                    printf("YEAR         :    %s\n",buffer);
                    break;
            case 4:
                    printf("GENRE        :    %s\n",buffer);
                    break;
            case 5:
                    printf("Language     :    %s\n",buffer);
                    break;

        }
    }
    printf("--------------------------------------------------------------------\n");
}