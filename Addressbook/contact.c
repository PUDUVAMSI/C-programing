#include <stdio.h>
#include<unistd.h>
#include <stdlib.h>
#include <string.h>
#include "contact.h"
#include <ctype.h>
#include "file.h"
#include "populate.h"

void load()//This is the function definition
{
  for(int j=1;j<=100;j++)//load
 { 
    printf("Loading your Contacts[");
    for(int i=0;i<=100;i++)//here we are runing the loop
    {
        if(i<j)
        {
          printf("_");
        }
        else
        {
          printf(" ");
        }
    }
    printf("] %d%%\r",j);
    fflush(stdout);
    usleep(20000);
  }
  printf("\nLoading Complited.\n");
}
void listContacts(AddressBook *addressBook) //function defintion
 { //here we are printing the all contacts
    int flag=0;
    if((addressBook->contactCount)>0)
    {
    load();
    printf("------------------------------------------- ADDRESSBOOK CONTACTS ----------------------------------------------------------------\n");
    printf("+------+-------------------+-----------------+------------------------+\n");
    printf("|S.No  |Name               |Mobile Number    |Email Id                |\n");//these are the head lines
    printf("+------+-------------------+-----------------+------------------------+\n");
    for(int i=0;i<(addressBook->contactCount);i++)//here we are using the one loop to print all contacts
    {
        flag++;
        printf("|%-6d|%-19s|%-17s|%-24s|\n",i+1,addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
        printf("+------+-------------------+-----------------+------------------------+\n");
    }
    }
    else
    {
    if(flag==0)//if there is no contacts
    {
      printf("------------------------------------------------\n");
      printf("No Contacts Found!\n");
      printf("------------------------------------------------\n");
    }
    }
    //printf("%s\n", addressBook->contacts[addressBook->contactCount].name);
}

void initialize(AddressBook *addressBook) {
    addressBook->contactCount = 0;
   // populateAddressBook(addressBook);

    // Load contacts from file during initialization (After files)
    loadContactsFromFile(addressBook);
}

void saveAndExit(AddressBook *addressBook) {
    saveContactsToFile(addressBook); // Save contacts to file
    exit(EXIT_SUCCESS); // Exit the program
}
int is_name(char str[],AddressBook *addressBook)//here we are validating the name
{
    int i=0;
    int flag=0;
    while(str[i])//name should contain only alphabit charcters
    {
       int res= isalpha(str[i]);//here we are pre defined function to know the char is alphabit or not
       if(res==0 && str[i]!=' '&& str[i]!='.')//if there is any other char we are returning the 0
       {
          flag++;
          return 0;
       }
       i++;
    }
    if(flag==0)//else we are returning the 1
    return 1;
}
int is_number(char str[],AddressBook *addressBook)//here we are checking the Mobile number
{
    int count=0,flag=0;
    int len=strlen(str);//here we are finding the length.that length must be 10
    if(len!=10)
    {
        printf("Invalid Phone Number!\n");
        return 0;
    }
    else//if len is 10 it will enter inside the loop
    {
        int i=0;
        while(str[i])//after we are takeing character by character
        {
            int res=isdigit(str[i]);//here we are checking the each char is digit or not
            if(res==0)
            {
                printf("Invalid Phone Number!\n");
                return 0;//if it is not a digit it will return 0
            }
            else
            {
              count++;
              i++;
            }
        }
        if(count==len)
        {
            for(int u=0;u<(addressBook->contactCount);u++)//here we are checking the mobile is unique or not 
            {
               int cmp = strcmp(addressBook->contacts[u].phone,str);//for compring we are using one per defind function
                if(cmp == 0)
                {
                    flag++;
                    printf("Number Already Existing!\n");
                    return 0;
                }
            }
            if(flag==0)
            {
                return 1;
            }

        }
    }
}
int is_mail(char str[],AddressBook *addressBook)//here we are validating the Email id
{
    int flag=0,i=0,count=0,j=0,temp=0;
    char* ptr;
    while(str[j] !='\0')
    {
      if(str[j]=='@')//here we are checking the @ is there or not
      {
           temp++;
      }
      j++;
    }
    if(temp==1)//if it is persent
    {
        
        char* fptr=strstr(str,".com");//here we are checking the .com is there or not
        if(fptr==0)
        {
            printf("Invalid Email Id!\n");
            return 0;
        }
        else//if it there
        {
           if(str[i])
           {
             int res=isalpha(str[i]);
             if(res)
             {
               count++;
             }
           }
           for( i=0;i<(addressBook->contactCount);i++)//here we are checking the Email id is unique or not
           {
              if(strcmp(addressBook->contacts[i].email,str)==0)
              {
                flag++;
                printf("Email Id Already Existing!\n");
                return 0;
              }
            }
            if(flag==0 && count !=0)//if email id is unique it will return 1
            {
              return 1;
            }
        }
    }
    else
    {
      return 0;
    }
}
void createContact(AddressBook *addressBook)//This is the function defintion
{//here we are creating the contact

     int i=1,j=1,k=1,l=1;
      while(j)
      {
       char na[50];
       printf("Enter the Name: ");//here the user is giveing some input name
       getchar();
       scanf("%[^\n]",na);
       int res=is_name(na,addressBook);//here we are calling the function to validate the name
       if(res)//if it is correct name then we going to copy into our structer 
       {
         strcpy(addressBook->contacts[addressBook->contactCount].name,na);//here we are using the per defind function to copy the name
         j=0;//after we are terminating the loop
       }
       else//if it is not a valid name
       {
          if(k<=3)//we are asking the user to reenter the name 
         {
            printf("Invalid Name!\n");
            printf("Re");
            k++;
         }
         else if(k>3)//here we are giveing the limited amount of attempts only
         {
          printf("You have reached your Limit try after some time!\n");
          j=0;
          i=0;
          l=0;
          break;
         }
       }
      }
     int r=1;
     while(i)
     {
       getchar();
       char number[10];
       printf("Enter the Moblie Number: ");//here we are taking the input from user
       scanf("%[^\n]",number);
       int res=is_number(number,addressBook);//here we are calling the function to validate the number
       if(res)//if it is valid number
       {
         strcpy(addressBook->contacts[addressBook->contactCount].phone,number);//here we are copying the number into struct
         i=0;//after we are terminating the loop
       }
       else
       {
         if(r<=3)//here we are giveing the limited amount of attempts only
         {
            printf("Re");
            r++;
         }
         else
         {
            printf("You have reached your attemptes!\n");
            i=0,l=0;
            break;
         }
       }
    }
    int t=1;
    while(l)
    {
        char mail[20];
        getchar();
        printf("Enter the Email Id: ");//here are we are tacking the input from the user
        scanf("%[^\n]",mail);
        int res=is_mail(mail,addressBook);//here we are calling the function to validate the email
        if(res)//if it is valid email
        {
            strcpy(addressBook->contacts[addressBook->contactCount].email,mail);//we are copying the mail into struct
            printf("Contact Created Successfully.\n");
            addressBook->contactCount++;
            l=0;//after we are terminating the loop
        }
        else
        {
          if(t<=3)
          {
            printf("Re");//here we are giveing the limited amount of attempts only
            t++;
          }
          else
          {
            printf("You have reached attempts try after some time!\n");
            l=0;
            break;
          }
        }
    }
 
}

void searchContact(AddressBook *addressBook) //This is the function  defintion
{//here we are using this function to search the contact
    int k=1;
    char name[20];
    char mob[10];
    char mail[20];
    while(k)
    {
        printf("\t\t\t--------Choose One Option-----------\n");//here we are giving some options to user to select one opt
        printf("1.By Using Name\n2.By Using Mobile Number\n3.Using Email Id\n4.Exit\n");
        int opt,flag=0;
        printf("Enter the one option: ");//user is going to select one option
        scanf("%d",&opt);
        getchar();
        switch(opt)//based on option we are entering the into that case
        {
                 case 1: printf("Enter the Person Name: ");//by using the person name we are going to search the contact
                         scanf("%[^\n]",name);
                         for(int i=0;i<(addressBook->contactCount);i++)
                         {
                            if(strcmp(addressBook->contacts[i].name,name)==0)//here we are compering the name with struct array names
                            {//if it is there it will print that person info
                                flag++;
                                printf("\t\t\t\t\t-------------------***Contact Info***---------------------->\n");
                                printf("-------------------------------------------\n");
                                printf("Name:%s\n",addressBook->contacts[i].name);
                                printf("-------------------------------------------\n");
                                printf("Moblie Number:%s\n",addressBook->contacts[i].phone);
                                printf("-------------------------------------------\n");
                                printf("Email:%s\n",addressBook->contacts[i].email);
                                printf("-------------------------------------------\n");

                            }
                         }
                         if(flag==0)//if it is not there
                         {
                            printf("Contact is Not Found!\n");
                         }
                         break;
                 case 2: printf("Enter the Person Mobile number: ");//by using the person mobile number we are going to search the contact  
                         scanf("%[^\n]",mob);
                         for(int i=0;i<(addressBook->contactCount);i++)
                         {
                             if(strcmp(addressBook->contacts[i].phone,mob)==0)//here we are compering the contacts
                            {//if it is there it will print that person info
                              flag++;
                              printf("\t\t\t\t\t-------------------***Contact Info***---------------------->\n");
                              printf("-------------------------------------------\n");
                              printf("Name:%s\n",addressBook->contacts[i].name);
                              printf("-------------------------------------------\n");
                              printf("Moblie Number:%s\n",addressBook->contacts[i].phone);
                              printf("-------------------------------------------\n");
                              printf("Email:%s\n",addressBook->contacts[i].email);
                              printf("-------------------------------------------\n");
                            }
                         }
                         if(flag==0)//if it is not there
                         {
                            printf("Contact is Not Found!\n");
                         }
                         break;  
                 case 3: printf("Enter the Person Gmail Id: ");
                         scanf("%[^\n]",mail);
                         for(int i=0;i<(addressBook->contactCount);i++)//by using the person email id we are going to search the contact
                         {
                            if(strcmp(addressBook->contacts[i].email,mail)==0)//here we are compering the contacts
                            {//if it is there it will print that person info
                               flag++;
                               printf("\t\t\t\t\t-------------------***Contact Info***---------------------->\n");
                               printf("-------------------------------------------\n");
                               printf("Name:%s\n",addressBook->contacts[i].name);
                               printf("-------------------------------------------\n");
                               printf("Moblie Number:%s\n",addressBook->contacts[i].phone);
                               printf("-------------------------------------------\n");
                               printf("Email:%s\n",addressBook->contacts[i].email);
                               printf("-------------------------------------------\n");
                            }
                         }
                         if(flag==0)//if it is not there
                         {
                            printf("Contact is Not Found!\n");
                         }
                         break; 
                 case 4: k=0;//this for exit
                         break;  
               default : printf("Invalid Input!\n");
                         break;                                  
        }
    }


}
int search_name(AddressBook* addressBook,char* name,int* count2,int* duplicate)//function definition
{
	
	int dup[10];
	int count=0;
	int flag=0;
	int real=-1;
	for(int i=0;i<(addressBook->contactCount);i++)//here we are checking the name by runing loop
	{
		 int res = strcmp(addressBook->contacts[i].name,name);//here we are using the strcmp to compare the  name
		 if(res == 0)
		 {
			 flag =1;
			 dup[count] = i;
			 duplicate[count] = i;
			 count++;
		}
		
	}
	*count2 = count;
	 if(flag==1)
	{
	      for(int i=0;i<count;i++)
	      {
			    real = dup[i];
			    printf("\t\t:::USER DETAILS:::\n");//here we are printing the detailes
		      printf("\n------------------------------------------------\n\n");
			    printf("Name: %s\n",addressBook->contacts[real].name);
			    printf("Phone: %s\n",addressBook->contacts[real].phone);
		      printf("Email: %s\n",addressBook->contacts[real].email);
			    printf("\n------------------------------------------------\n");
	      }
	      return real;
	}
	else
  {
	    printf("Student Not Found\n");//if the std is not found
	    return 0;
  }

}
int search_phone(AddressBook* addressBook,char* phone)//function def
{
	int flag=0;
	int real;
	for(int i=0;i<(addressBook->contactCount);i++)//runing the loop
	{
		int res = strcmp(addressBook->contacts[i].phone,phone);//here we are comparing the phone numbers
		 if(res == 0)
		 {
			 flag =1;
			 real=i;
			 break;
		 }
	  }
	 if(flag==1)
	 {
		  printf("\t\t:::USER DETAILS:::\n");//here we are printing the detailes
		  printf("\n----------------------------------------------\n\n");
		  printf("name: %s\n",addressBook->contacts[real].name);
		  printf("phone: %s\n",addressBook->contacts[real].phone);
		  printf("email: %s\n",addressBook->contacts[real].email);
		  printf("\n----------------------------------------------\n");
		  return real;
	}
	 else
	 {
		 printf("Student not found: \n");
		 return 0;
	}
}
int search_email(AddressBook* addressBook,char* email)//function def
{
	int flag=0;
	int real;
			  for(int i=0;i<(addressBook->contactCount);i++)//here we are runing the loop
			  {
				 int res = strcmp(addressBook->contacts[i].email,email);
				 if(res == 0)
				 {
					 flag =1;
					 real=i;
					 break;
				 }
		          }
			  if(flag==1)
			  {
			     printf("\t\t:::USER DETAILS:::\n");//here we are printing the detailes
			     printf("\n--------------------------------------------------\n\n");
			     printf("name: %s\n",addressBook->contacts[real].name);
			     printf("phone: %s\n",addressBook->contacts[real].phone);
		       printf("email: %s\n",addressBook->contacts[real].email);
			     printf("\n--------------------------------------------------\n");
			     return real;
			  }
			  else
			  {
				  printf("Student not found: ");
				  return 0;
			  }
}
void edit_fun(AddressBook* addressBook,int real)
{//this function is used to to edit the contact
	int E=1;
	int opt;
	char name1[20];
	char phone1[20];
	char email1[40];

				while(E)
				{
					printf("1. Edit name\n2. Edit phone\n3. Edit email\n4. Exit\n");
					printf("Enter option: ");
					//char name1[20];
					//char phone1[20];
					//char email1[20];
					int res;
					scanf("%d",&opt);
					switch(opt)
					{
					case 1: printf("Enter tha name: ");
						      scanf(" %[^\n]",name1);
						      res = is_name(name1,addressBook);
						      if(res == 0)
						      {
							       printf("Invalid Name!\n");
						      }
						      else
						      {
							     strcpy(addressBook->contacts[real].name,name1);
                   printf("Contact Updated Successsfully.\n");
						      }
                  break;
					case 2: printf("Enter tha phone: ");
						      scanf(" %[^\n]",phone1);
						      res = is_number(phone1,addressBook);
						      if(res == 0)
						      {
							       printf("Invalid Number!\n");
						      }
						      else
						      {
							       strcpy(addressBook->contacts[real].phone,phone1);
                     printf("Contact Updated Successsfully.\n");
						      }
                  break;
					case 3: printf("Enter tha email: ");
						      scanf(" %[^\n]",email1);
						      res = is_mail(email1,addressBook);
						      if(res == 0)
						      {
							        printf("Invalid Email!\n");
						      }
						      else
					     	  {
							       strcpy(addressBook->contacts[real].email,email1);
                     printf("Contact Updated Successsfully.\n");
						      }
                  break;
					case 4: E=0;
                  break;

					}
				}
}
void editContact(AddressBook *addressBook)//function definition for edit contact 
{
    int E=1;
    while(E)
    {
       printf("---Choose One Option--->\n");
	   printf("1. Search By name\n2. Search by Phone Number\n3. Search By Email Id\n4. Exit\n");
	   printf("Enter your choice: ");
	   int opt,real,k=1,res;
	   int edit_duplicate[10];
	   int count1=0;
	   char name[20];
	   char phone[20];
	   char email[20];
	   int name_dup[20];
	   char dup_num[20];
	   char dup_email[30];
	   int flag=0,flag2=0;
	   int dup_opt;
	   int dup_real;
	   scanf("%d",&opt);
	   switch(opt)
	   {
		   case 1: printf("Enter name: ");//here we are editing by the name of person
			         scanf(" %[^\n]",name);
			         real = search_name(addressBook,name,&count1,name_dup); 
			         if(real != -1)
			         {
				         if(count1>1)
				        {
					         printf("\n*There are some Duplicate Contacts\n*please re-type with phone or email\n");
					         while(k)  
					        {
						              printf("1. search by mobile\n2. search ny email\n");  
                          printf("Enter the Option: ");
						              scanf("%d",&dup_opt);
						              if(dup_opt==1)
						              { 
                             printf("Enter the Number: ");                 
						                 scanf(" %[^\n]",dup_num);
						                 for(int i=0;i<count1;i++)
						                 {
							                	dup_real = name_dup[i];
								                if(strcmp(addressBook->contacts[dup_real].phone,dup_num)==0)
								                {
									                 flag2 =1;
									                 break;
								                } 
						                 }
						                 if(flag2==1)
						                {
							                  edit_fun(addressBook,dup_real);
							                  k=0;
						                }
						                else
						               {
						               		printf("\nInvalid Number!\n");
						               }
				                  }
				          }
				        }
				        else
				        {
					        edit_fun(addressBook,real);	
				        }

			        }
			        else
			        {
				          printf("Contact not found!\n");
			        }
			         break;
		   case 2: printf("Enter phone: ");//here we are editing by the number of person
			         scanf(" %[^\n]",phone);
			         real = search_phone(addressBook,phone); 
			         if(real != -1)
			         {
		       		    edit_fun(addressBook,real);		  
			         }
			         else
			         {
                    printf("Contact not found!\n");
			         }
			         break;
			   
		   case 3: printf("Enter mail id: ");//here we are editing by the email of person
			         scanf(" %[^\n]",email);
			         real = search_email(addressBook,email); 
			         if(real != 1)
			         {
				           edit_fun(addressBook,real);
               }
			         else
			         {
				          printf("person not found: \n");
			         }
			         break;
		   case 4: E=0;
			         break;
	   }
    }
}

void deleteContact(AddressBook *addressBook)//This is the function definition
{//here we are specific deleting the contact
  int u=1;
  while(u)
  {
        printf("\t\t\t--------***Choose One Option***-----------\n");//here we are giving some options to user to select one opt
        printf("1.By Using Name\n2.By Using Mobile Number\n3.Using Email Id\n4.Exit\n");
        int opt,real,i,flag=0,count=0;
        char name[30];
        char number[10];
        char mail[30];
        int arr[10];
        printf("Enter the one option: ");
        scanf("%d",&opt);
        getchar();
        switch(opt)//based on opt it will jumps into the case
        {
            case 1: printf("Enter the Person Name: ");//here user will give the name
                    scanf("%[^\n]",name);
			              for(int i=0;i<(addressBook->contactCount);i++)
			              {
				             int res = strcmp(addressBook->contacts[i].name,name);
				             if(res == 0)
				             {
					             flag =1;
					             arr[count] = i;
					             count++;
				             }
		                }
                    if(flag==0)
			              {
				             printf("Contact Not Found!\n");
			              }
			              else
			              {   
				             if(count>=2)
				             {
					             for(int j=0;j<count;j++)
					             {
                            real = arr[j];
						                printf("\n\n------------------------------------------\n");
						                printf("%s\n%s\n%s\n",addressBook->contacts[real].name,addressBook->contacts[real].phone,addressBook->contacts[real].email);
						                printf("\n-------------------------------------------\n");
					             }
					             printf("\n*There are some Duplicate Contacts\n*Please Re-type with Phone or Email\n");
				             }
				             else
                     {	
                          real = arr[0];
			    	              Contact temp = addressBook->contacts[real];
			    	              addressBook->contacts[real] = addressBook->contacts[addressBook->contactCount-1];
			    	              addressBook->contacts[addressBook->contactCount-1] = temp;
				                  (addressBook->contactCount--);
                     }
                    }
                    break;
            case 2: printf("Enter the Person Mobile Number: ");//here user will give the Mobile number
                    scanf("%[^\n]",number);
                    for(i=0;i<addressBook->contactCount;i++)
                    {
                        if(strcmp(addressBook->contacts[i].phone,number)==0)
                        {

                            flag++;
                            Contact temp= addressBook->contacts[i];//after we are swaping the contacts
                            addressBook->contacts[i]=addressBook->contacts[addressBook->contactCount-1];
                            addressBook->contacts[addressBook->contactCount-1]=temp;
                            addressBook->contactCount--;
                            printf("Deleted Successfully.\n");
                            break;
                        }
                    }
                    if(flag==0)
                    {
                        printf("Invalid Phone Number!\n");
                    }
                    break;        
            case 3: printf("Enter the Person Email Id: ");//here user will give the Mobile number
                    scanf("%[^\n]",mail);
                    for(i=0;i<addressBook->contactCount;i++)
                    {
                        if(strcmp(addressBook->contacts[i].email,mail)==0)
                        {
                            flag++;
                            Contact temp= addressBook->contacts[i];//after we are swaping the contacts
                            addressBook->contacts[i]=addressBook->contacts[addressBook->contactCount-1];
                            addressBook->contacts[addressBook->contactCount-1]=temp;
                            addressBook->contactCount--;
                            printf("Deleted Successfully.\n");
                            break;
                        }
                    }
                    if(flag==0)
                    {
                        printf("Invalid Email Id!\n");
                    } 
                    break;
            case 4: u=0;
                    break;     
          default : printf("Invalid Input!\n");
                    break;                         
        }
      }
}
