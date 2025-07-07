#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types
#include<stdio.h>
/* 
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

/*#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4*/

typedef struct _DecodeInfo
{
    /* Secret File Info */
    char secret_file_name[20];			//to store secret file name
    FILE* fptr_secret_data;	
    char *dup;		//to store secret file address
 	char magic_string[100];
    uint ext_size;
    uint secret_fsize;
    char ext_data[4];			//to store the size of secret data

    /* Stego Image Info */
    char *dest_image_fname;		//to store dest file name
    FILE *fptr_dest_image;		//to store dest file adress

} decodeInfo;


/* Encoding function prototype */

/* Check operation type */

/* Read and validate Encode args from argv */
Status read_and_validate_decode_args(char *argv[], decodeInfo *decInfo);

/* Perform the encoding */
Status do_decoding(decodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status open_file(decodeInfo *decInfo);

/* Store Magic String */
Status decode_magic_string(decodeInfo *decInfo);

/* Encode secret file extenstion */
Status decode_secret_file_extn(FILE *fptr_dest_image, decodeInfo *decInfo);
Status decode_secret_file_extn_size(FILE *fptr_dest_image,decodeInfo *decInfo);
/* Encode secret file size */
Status decode_secret_file_size(FILE *fptr_dest_image, decodeInfo *decInfo);

/* Encode secret file data*/
Status decode_secret_file_data(decodeInfo *decInfo);

/* Encode function, which does the real encoding */
Status decode_data_from_image(char *data, int size, FILE *fptr_dest_image);
/* Encode a byte into LSB of image data array */
Status decode_byte_from_lsb(char* data, char *image_buffer);

#endif