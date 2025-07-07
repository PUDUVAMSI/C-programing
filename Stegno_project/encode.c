#include <stdio.h>
#include "encode.h"
#include "types.h"
#include<string.h>
#include"common.h"
/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;//if it is returning NULL
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;//if it is returning NULL
    }

    // Stego Image file
    encInfo->fptr_dest_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_dest_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;//if it is returning NULL
    }

    // No failure return e_success
    printf("INFO : Files are Opened Successfully....\n");
    return e_success;//return e_success
}
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    if(strstr(argv[2],".bmp") != NULL)// check argv[2] is .bmp or not->false return  e_failure
    {
        encInfo->src_image_fname = argv[2];//true store argv[2] into structure member
        printf("INFO : Here We Are Containg .bmp File...\n");
    }
    else
    {
        printf("Error : Here we are not containg .bmp file...\n");
        return e_failure;//if it is not containg the .bmp
    }
    if(argv[3] == NULL)
    {
          printf("Invalid arguments\n");
          return e_failure;
    }
    else if((strstr(argv[3],".txt") != NULL) ||(strstr(argv[3],".c") != NULL )||(strstr(argv[3],".sh") != NULL)||(strstr(argv[3],".csv") != NULL))//check argv[3] is .txt or not->false return  e_failure
    {
        encInfo->secret_fname = argv[3];//true store argv[3] into structure member
        printf("INFO : Here We Are Containg %s File...\n",argv[3]);
    }
    else
    {
        printf("ERROR : %s File is Invalid...\n",argv[3]);
        return e_failure;//if it is not containg the .txt or .c or .sh
    }
    if(argv[4] == NULL)//check argv[4] is null or not ,true then
    {
        encInfo->stego_image_fname = "destination.bmp";//here we are giveing some default name
        printf("INFO : There is no user file by default we are giving one .bmp file...\n");
    }
    else
    {
        if(strstr(argv[4],".bmp") != NULL)//if it is .bmp file we are checking
        {
            printf("INFO : Here We Are Containg .bmp Destination File...\n");
            encInfo->stego_image_fname = argv[4];//if it is true store into structure vari
        }
        else
        {
            printf("ERROR : %s File is Invalid...\n",argv[4]);
            return e_failure;//if it is not containg the .bmp
        }
    }
    return e_success;
}
uint get_file_size(FILE *fptr)
{
    fseek(fptr,0,SEEK_END);//we are shifting the pointer to last
    return ftell(fptr);//here it will give the position
   //return unsigend int value;
}
Status check_capacity(EncodeInfo *encInfo)
{
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);//call the get_image_size(enInfo->fptr_src_image),store into image_capacity
    uint res = strlen(MAGIC_STRING);//magic_string length
    res = res * 8;
    uint header = 54;//for header
    uint size_ex = 32;//for size of ext
    uint encode_ex = 32;//for ext
    uint len = get_file_size(encInfo->fptr_secret);//call the  get_file_size(FILE *fptr),store into size_secret_file;
    encInfo->size_secret_file = len;//we are storing into the structure var
    len = len * 8;
    if(encInfo->image_capacity>(res+header+size_ex+encode_ex+32+len)) //sate 3: image capacity >add;
    {
        printf("INFO : Done Data is Less Then Source File Data...\n");
        return e_success;//true ->print success message, return e_success;
    }
    printf("Error: There is no space to store data..\n");
    return e_failure;//false -> print error message, return e_failure
}
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    rewind(fptr_src_image);//rewind the fptr_src_image
    char imagebuffer[54];//char imagebuffer[54];
    fread(imagebuffer,54,1,fptr_src_image);// read 54 bytes form src image
    fwrite(imagebuffer,54,1,fptr_dest_image);//copy the 54 bytes to destination image
    printf("INFO : Successfully Copied Source file header to destination file..\n");
    return e_success;//return e_success

}
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    int len = strlen(magic_string);//step 1: find the len of magic string;
    char str[]= MAGIC_STRING;
    if((encode_data_to_image(str,len,encInfo->fptr_src_image,encInfo->fptr_dest_image)) == e_success)//call the fucntion encodedata(magicstring,size,encinfo->fptr_src_image,encinfo->fptr_dest_image);
    {
        printf("INFO : Magic String Data Encodeed Successfully ....\n");
        return e_success;//if it returns e_success
    }

}
Status encode_data_to_image(const char *data, int size, FILE *fptr_src_image, FILE *fptr_dest_image)
{
    char imagebuffer[8];//char imagebuffer[8];
    for(int i=0;i<size;i++)//run a loop still size times;
    {
        fread(imagebuffer,8,1,fptr_src_image);//get the 8 bytes from the src file after 54 bytes
        encode_byte_to_lsb(data[i],imagebuffer);//call the Status encode_byte_to_lsb(data[0],image_buffer);
        fwrite(imagebuffer,8,1,fptr_dest_image);//write the imagebuffer to destination_image
    }
    return e_success;//returning the s_success
}
Status encode_byte_to_lsb(char data, char *image_buffer)
{
    for(int i=0;i<8;i++)//writing the logic for encodeing the data
    {
        int res = data & (1<<i);//here we are getting one one byte from the data
        res = res >>i;//we are shifting right
        image_buffer[i] = image_buffer[i] & (~(1));//in image_buffer we are clearing the last lsb bit 
        image_buffer[i] = image_buffer[i] | res;//here we are storing the res in the last lsb bit of the image_buffer
    }
}
Status encode_size_to_lsb(int size, char *image_buffer)
{
    for(int i = 0; i < 32; i++)//runing the loop to insert the size
    {
       int res = size & (1<<i);//here we are getting one one byte from the data
       res = res >> i;//we are shifting right
       image_buffer[i] = image_buffer[i] & (~(1));//in image_buffer we are clearing the last lsb bit
       image_buffer[i] = image_buffer[i] | res;//here we are storing the res in the last lsb bit of the image_buffer
    }
    return e_success;//returning e_success
} 
Status encode_secret_file_extn_size(int size,EncodeInfo *encInfo)
{
    char imagebuffer[32];//char  imagebuffer[32];
    fread(imagebuffer,32,1,encInfo->fptr_src_image);//read the 32bytes from src store into imagebuffer
    encode_size_to_lsb(size,imagebuffer);//call the encode size to lsb(size,imagebuffer);
    fwrite(imagebuffer,32,1,encInfo->fptr_dest_image);//write the 32 bytes to the destination image
    printf("INFO : Secret File Extn Size is Encoded Successfully...\n");
    return e_success;//return e_sucess;
}
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
   int len = strlen(file_extn);//finding the size of ext
   if((encode_data_to_image(file_extn,len,encInfo->fptr_src_image,encInfo->fptr_dest_image)) == e_success)//calling the function to encode_data
    {
        printf("INFO : Secret File Extension Encodeed Successfully ....\n");
        return e_success;//if it returns e_success
    }
}
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    char imagebuffer[32];//char  imagebuffer[32]
    fread(imagebuffer,32,1,encInfo->fptr_src_image);//step 1:read the 32bytes from src store into imagebuffer
    encode_size_to_lsb(file_size,imagebuffer);//step 2 :call the encode size to lsb(size,imagebuffer);
    fwrite(imagebuffer,32,1,encInfo->fptr_dest_image);//step 3: write the 32 bytes to the destination image
    printf("INFO : Secret File Data Size is Encoded Successfully...\n");
    return e_success;//return e_sucess;
}
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    char sec_buffer[encInfo->size_secret_file];;//char  imagebuffer[size of secret file]
    rewind(encInfo->fptr_secret);//we are shifting the file pointer to first
    fread(sec_buffer,encInfo->size_secret_file,1,encInfo->fptr_secret);//read the bytes from the source file to sec_buffer
    char src_buffer[8];//char src_buffer[8]
    for(int i=0;i<(encInfo->size_secret_file);i++)//runing the the loop
    {
        fread(src_buffer,8,1,encInfo->fptr_src_image);//read the bytes from the source file to src_buffer
        encode_byte_to_lsb(sec_buffer[i], src_buffer);//we are calling the function
        fwrite(src_buffer,8,1,encInfo->fptr_dest_image);//write the data to destination file
    }
    printf("INFO : Secret File Data is Encoded Successfully...\n");
    return e_success;//return e_success
}
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char ch[1];//here we are writeing the remaining data to destination file
     while((fread(ch,1,1,fptr_src))>0)//we are reading one one byte
     {
        fwrite(ch,1,1,fptr_dest);//we are writeing one one byte
     }
     return e_success;//return e_success
}

Status do_encoding(EncodeInfo *encInfo)
{
    if(open_files(encInfo) == e_success)//step 1:call the open file functin(encinfo)
    {
    
        if((check_capacity(encInfo)) == e_success)//step 2:check success or failure it it is success go to step 4;(check_capa);
        {
            if((copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_dest_image)) == e_success)//step 4: call the copy_bmp_headre(encinfo->fptr_src_image,encinfo->fptr_dest_image);
            {
                //true -> print sucess message  go to step 5;
                if((encode_magic_string(MAGIC_STRING,encInfo)) == e_success)//step 5-> call the encode_magic string(MAGIC_STRING,encinfo);
                {
                    //true ->printf sucess message,go to step 6
                    char *str= strchr(encInfo->secret_fname,'.');//step 6-> Extract the .txt from the secret_file_name;
                    strcpy(encInfo->extn_secret_file,str);//and store into the stuct member (extn_secret_file)
                    int len = strlen(encInfo->extn_secret_file);
                    if(encode_secret_file_extn_size(len,encInfo) == e_success)//call the status encode_secre_file_extn_size(int size,EncodeInfo *encinfo);
                    {
        
                        if((encode_secret_file_extn(encInfo->extn_secret_file,encInfo)) == e_success)//here we are calling the function to encode_secret_file_extn
                        {
                            if((encode_secret_file_size(encInfo->size_secret_file,encInfo)) == e_success)//here we are calling the function to encode_secret_file_size
                            {
    
                                if((encode_secret_file_data(encInfo)) == e_success)//here we are calling the function to encode_secret_file_data
                               {
                                  if((copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_dest_image)) == e_success)//here we are calling the function to copy_remaining_image_data
                                  {
                                     printf("INFO : Remaining Data Encoded Successfully...\n");
                                     return e_success;//return e_success
                                  }
                                  else
                                  {
                                     printf("ERROR : Not Encoded Remaining Data...\n");
                                     return e_failure;//false return failure
                                  }
                                }
                                else
                                {
                                    printf("ERROR : Not Encoded Secret File Data...\n");
                                    return e_failure;//false return failure
                                }
                            }
                            else
                            {
                                printf("ERROR : Not Encoded Secret File Size...\n");
                                return e_failure;//false return failure
                            }
                        }
                        else
                        {
                            printf("ERROR : Not Encoded Secret Ext...\n");
                            return e_failure;//false return failure
                        }       
                    }
                    else
                    {
                        printf("ERROR : Not Encoded Secret File Ext Size...\n");
                        return e_failure;//false return failure
                    }            
                }
                else
                {
                    printf("ERROR : Not Encoded Magic String Data...\n");
                    return e_failure;//false return failure
                }
            }
            else
            {
                printf("ERROR : In Copying The bmp header...\n");
                return e_failure;//false return failure
            }
        }
        else
        {
            printf("ERROR : In Check Capacity Data...\n");
            return e_failure;//false return failure
        }
    }
            
    else
    {
        printf("ERROR : Open File...\n");
        return e_failure;//false return failure
    }
}
