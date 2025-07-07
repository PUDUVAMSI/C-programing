#include "decode.h"
#include <stdio.h>
#include "types.h"
#include<string.h>
#include"common.h"

Status read_and_validate_decode_args(char *argv[], decodeInfo *decInfo)//fucntion defintion
{
    if(strstr(argv[2],".bmp") != NULL)// check argv[2] is .bmp or not->false return  e_failure
    {
        decInfo->dest_image_fname = argv[2];//true store argv[2] into structure membe
    }
    else
    {
        printf("ERROR : %s is Invalid File\n",argv[2]);
        return e_failure;//if it is not containg the .bmp
    }
    if(argv[3] == NULL)//check argv[3] is null or not ,true then
    {
        strcpy(decInfo->secret_file_name ,"file");//here we are giveing some default name
    }
    else
    {
        strcpy(decInfo->secret_file_name,strtok(argv[3],"."));//here we are removing the ext
    }
    return e_success;
}
Status open_file(decodeInfo *decInfo)//function defintion
{
    decInfo->fptr_dest_image = fopen(decInfo->dest_image_fname,"r");//here we are opening the destination file in the read mode
    if( decInfo->fptr_dest_image == NULL)//error check
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->dest_image_fname);
        return e_failure;//if it returns null
    }
    return e_success;//return e_success

}
Status decode_magic_string(decodeInfo *decInfo)//function defintion
{
    fseek(decInfo->fptr_dest_image,54,SEEK_SET);//here we are shifting the file pointer the 54 position
    char data[2]={'\0'};//buffer
    if((decode_data_from_image(data,2,decInfo->fptr_dest_image)) == e_success)//calling the function to decode data from the image
    {
        strcpy(decInfo->magic_string,data);//here we are copyong the data to structure var
        return e_success;//return e_success
    }
}
Status decode_secret_file_data(decodeInfo *decInfo)//function defintion
{
     char data[decInfo->secret_fsize];//buffer
     if((decode_data_from_image(data,decInfo->secret_fsize,decInfo->fptr_dest_image)) == e_success)//calling the function decode_data_from_image
     {
        fwrite(data,decInfo->secret_fsize,1,decInfo->fptr_secret_data);//write the data to fptr_secret_data
        return e_success;//return e_success
     }
}
Status decode_data_from_image(char *data, int size, FILE *fptr_dest_image)//function defintion
{
    char image_buffer[8];//buffer
    for(int i=0;i<size;i++)//runing loop for size times 
    {
        fread(image_buffer,8,1,fptr_dest_image);//read 8 bytes from the fptr_dest_image to image_buffer
        decode_byte_from_lsb(&data[i],image_buffer);//calling fun
    }
    return e_success;//return e_success
}
Status decode_byte_from_lsb(char* data, char *image_buffer)//function defintion
{
   int n=0;
   for(int i = 0; i < 8; i++)//runing loop for 8 times
   {
       int res = image_buffer[i] & (1);//we are takeing last lsb of image_buffer 
        res = res << i;//here we are shifting left for i times
        n = n | res;//here we are adding to null
   }
   *data=n;
   return e_success;//return e_success
}
Status decode_secret_file_extn(FILE *fptr_dest_image, decodeInfo *decInfo)//function defintion
{
    char data[decInfo->ext_size];//buffer
    if((decode_data_from_image(data,decInfo->ext_size,fptr_dest_image)) == e_success)//calling the function
    {
        strcpy(decInfo->ext_data,data);//copying
    }
    return e_success;//return e_success
}
Status decode_secret_file_extn_size(FILE *fptr_dest_image,decodeInfo *decInfo)//function defintion
{
     char image_buffer[32];//buffer
     fread(image_buffer,32,1,fptr_dest_image);//read 32 bytes form fptr_dest_image to image_buffer to get the size of extn
     int b=0,res;
     for(int i = 0; i < 32; i++)//runing the loop for 32 times
     {
        res = image_buffer[i] & (1);//geting one one byte from the lab of image_buffer
        res = res << i;//we are shifting left
        b = b | res;//adding res to b
     }
    decInfo->ext_size=b;//ext_size we are updating with the b 
    return e_success;//return e_success
}
Status decode_secret_file_size(FILE *fptr_dest_image, decodeInfo *decInfo)//function defintion
{
   uint x=0,res;
   char image_buffer[32];//buffer
   fread(image_buffer,32,1,fptr_dest_image);//read 32 bytes from the fptr_dest_image to buffer
   for(int i = 0; i < 32; i++)//runing the loop for 32 times
   {
        res = image_buffer[i] & (1);//geting one one byte from the lab of image_buffer
        res = res << i;//we are shifting left
        x = x | res;//adding res to x
   }
   decInfo->secret_fsize = x;//secret_fsize we are updating with the x
   return e_success;//return e_success
}
Status do_decoding(decodeInfo *decInfo)//function defintion
{
    if(open_file(decInfo) == e_success)//function calling 
    {
      printf("INFO : Opened Files Successfully..\n");  
      if((decode_magic_string(decInfo)) == e_success)//function calling
      {
           printf("INFO : Magic String Decoded Successfully....\n");
           char *str;
           printf("Enter the Magic string: ");//here user will enter some key that key we have to compare correct or not
           scanf("%s",str);
           if(strcmp(decInfo->magic_string,str) == 0)//here we are comapareing
           {   
                //if it is matching
               if((decode_secret_file_extn_size(decInfo->fptr_dest_image,decInfo)) == e_success)//function calling
               {
                    printf("INFO : Secret File Extn Size is Decoded Successfully...\n");
                    if((decode_secret_file_extn(decInfo->fptr_dest_image,decInfo)) == e_success)//function calling
                    {
                       printf("INFO : Secret File Extension Decoded Successfully..\n");
                       decInfo->dup = strcat(decInfo->secret_file_name,decInfo->ext_data);//here we are useing strcat to add secrect_file_name and ext_data
                       printf("hellop\n");
                       decInfo->fptr_secret_data = fopen(decInfo->dup,"w");//after adding both we are opening the file with "w" mode
                       if((decode_secret_file_size(decInfo->fptr_dest_image,decInfo)) == e_success)//function calling
                       { 
                           printf("INFO : Secret File Size Decoded Successfully..\n");
                           if((decode_secret_file_data(decInfo)) == e_success)//function calling
                           {
                               printf("INFO : Secret File Data Decoded Successfully..\n");
                               return e_success;//return e_success
                           }
                           else
                           {
                              return e_failure;//return e_failure
                           }
                       }
                       else
                       {
                           return e_failure;//return e_failure
                       }
                    }
                    else
                    {
                        return e_failure;//return e_failure
                    }
                }
                else
                {
                    return e_failure;//return e_failure
                }
           }
           else
          {
             printf("ERROR : Invalid Magic String...\n");
             return e_failure;
          }
      }
      else
      {
        return e_failure;//return e_failure
      }
   }
   else
   {
     return e_failure;//return e_failure
   }
}