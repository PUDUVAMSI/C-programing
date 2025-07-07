/*                                                 :::  DESCRIPTION  :::
NAME : PUDU VAMSI
DATE : 12/03/2025
BATCH NO : 24038
DESCRIPTION : An Address Book Project is a simple yet practical application designed to store, manage, and retrieve contact details such as names, phone numbers, email addresses.
*/
#include "contact.h"

int main()
{
	AddressBook addressBook;
	int i=1;
	initialize(&addressBook);//we are calling function
	printf("\t\t\t\t\t\t\t----:::WELCOME TO ADDRESSBOOK:::----\n");
	while(i)
	{

	  printf("\nAddress Book Menu:\n");//here we are giving some options to user
	  printf("1. Create contact\n");
	  printf("2. Search contact\n");
	  printf("3. Edit contact\n");
	  printf("4. Delete contact\n");
      printf("5. List all contacts\n");
	  printf("6. Exit\n");
	  printf("Enter your choice: ");
	  int opt;
	  scanf("%d",&opt);
	  switch(opt)//here we are using the switch case
	  {//based on the opt will jumpes into that condition
		   case 1: createContact(&addressBook);//we are calling function
		           break;
		   case 2: searchContact(&addressBook);//we are calling function
		           break;		   
	       case 3: editContact(&addressBook);//we are calling function
	               break;
	       case 4: deleteContact(&addressBook);//we are calling function
		           break;
		   case 5: listContacts(&addressBook);//we are calling function	
		           break;
	       case 6: saveAndExit(&addressBook);//we are calling function
		           i=0;
		           break;
		  default: printf("Invalid Option!\nPlease Enter Valid Option-->\n");
		           break;
		         		   
	  }		
	}	   			      	   
}


