/*
NAME : PUDU VAMSI
DATE : 11/04/204
DESCRIPTION : This project implements a steganographic system using the Least Significant Bit (LSB) technique to hide secret information within digital images. The main objective is to enable secure, imperceptible communication by embedding a message (text) into the pixel values of an image without significantly altering its appearance.
*/


#include <stdio.h>
#include "encode.h"
#include "types.h"
#include<string.h>
#include "decode.h"
OperationType check_operation_type(char*);//here we are writeing the function prototype
int main(int argc,char* argv[])//in main function we are collecting arguments
{
    if(argc >= 3)//check whether the user is passed mimum 4 arguments or not
    {
        if((check_operation_type(argv[1]))==e_encode)//call the check operation type function(argv[1])
        {
            printf("INFO : You Have Selected Encodeing Option...\n\n");//check returning the e_encode or not ->true go to next step,
            EncodeInfo encInfo;//decalre a variable with EncodeInfo print the msg like selected encodeong
            if((read_and_validate_encode_args(argv,&encInfo)) == e_success)//call the function read_and_validate
            {
                  //check e_success or e_failure
              if(do_encoding(&encInfo)==e_success)//here we are calling the function do_encoding
              {
                printf("\nINFO : Total Data Encoded Successfully...\n");//atlast we are printing the success message
                fclose(encInfo.fptr_src_image);//here we are colseing all files
                fclose(encInfo.fptr_secret);
                fclose(encInfo.fptr_dest_image);
              }
              else
              {
                printf("Error\n");//if it is a e_failure
              }
            }
            else
            {
                printf("INFO : There is a error in read and validate function...\n");
            }
        }
        else if((check_operation_type(argv[1]))==e_decode)//we are function calling the check either user is doing encoding or decoding
        {
            printf("INFO : You Have Selected Decodeing Option...\n\n");//check returning the e_encode or not ->true go to next step,
            decodeInfo decInfo;
            if((read_and_validate_decode_args(argv,&decInfo)) == e_success)//call the function read_and_validate
            {
               if((do_decoding(&decInfo)) == e_success)
               {
                  printf("\nINFO : Total Data Decoded Successfully...\n");//atlast we are printing the success message
                  fclose(decInfo.fptr_secret_data);//here we are colseing all files
                  fclose(decInfo.fptr_dest_image);
               }
               else
               {
                  printf("Error\n");
               }
            }
        }
    }
    else
    {
        printf("Arguments Not Existing!!\n");//if user is passing invalid number of arguments
    }
      return 0;
}
OperationType check_operation_type(char *argv)
{
    if(strcmp(argv,"-e") == 0)//check whether the symbol is containing -e or not
    {
        return e_encode;//true return e_encode,flase go to next step
    }
    else if(strcmp(argv,"-d") == 0)//check whether the symbol is containing -d or not
    {
        return e_decode;//true return e_decode,flase return en_unsupported
    }
    else
    {
        return e_unsupported;//if it is unsupported argument
    }
}
