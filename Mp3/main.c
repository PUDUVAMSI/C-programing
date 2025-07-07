/*
NAME : PUDU VAMSI
DATE : 29/04/2025
DESCRIPTION : MP3 Tag Reader is a small program that lets you read and edit information in MP3 music files, like the song name, artist, album, and year. It helps you view and update the details stored inside your music files.
*/


#include<stdio.h>
#include<stdlib.h>
#include "reader.h"
#include<string.h>
#include "edit.h"
OperationType check_operation_type(char *);     //function declarations
int main(int argc,char* argv[])
{
    if(argc >=2)
    {
        if(check_operation_type(argv[1])==mp_view)          //checking operation type
        {
            reader redInfo;                                 //declaring structure variable
            if(read_and_validate_view_args(argv,&redInfo)==mp_success)
            {
                if(get_data(&redInfo)!=mp_success)          //calling get_data function
                {
                    return mp_failure;
                }
            }
        }
        else if(check_operation_type(argv[1])==mp_edit)     //calling check_operation function
        {
            editor editInfo;
            printf("------------------------------SELECTED EDIT-------------------------------\n");
            if(read_and_validate_edit_args(argv,&editInfo)==mp_success)
            {
                printf("INFO: entered into\n");
                if(modify_change(&editInfo)!=mp_success)
                {
                    printf("changes not modified\n");
                    return 0;
                }
            }
        }
        else{
            printf("ERROR: enter valid arguments\n");
            printf("For more Info type ./a.out --help\n");
        }
    }
    else
    {
        printf("ERROR: arguments you provided is incorrect\n");
    }
}
OperationType check_operation_type(char* symbol)        //definig check_operation_type function
{
    if(strstr(symbol,"-v")!=NULL)
    {
        return mp_view;                                     //selected view option
    }
    else if(strstr(symbol,"-e")!=NULL)
    {
        return mp_edit;                                 //selected edit option
    }
    else if(strstr(symbol,"--help")!=NULL)
    {
        printf("TO VIEW: ./a.out -v [your mp3 file name]\n");
        printf("TO EDIT: ./a.out -e -t/-a/-A/-y/-m/-c [your mp3 file name]\n");           //help function
        exit(0);
    }
    //printf("Not entered")
    else{
        return mp_unsupported;
    }
}
