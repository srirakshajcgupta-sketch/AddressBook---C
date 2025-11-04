#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<ctype.h>
#include "contact.h"
#include "file.h"

void listContacts(AddressBook *addressBook) 
{
    if (addressBook->contactCount >= MAX_CONTACTS)
    { 
        printf("Address book is full!\n"); 
        return; 
    }

    int contactCount = 0;
    // Sort contacts based on the chosen criteria
    printf("SI.        Name         Phone.no          email-id\n");
    printf("------------------------------------------------------\n");

    for(int i=0;i<addressBook->contactCount;i++)
    {
         printf("%d    ",i+1);
         printf("%s        %s         %s\n",addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
         contactCount++;
    }

     printf("------------------------------------------------------\n");
    // Display total contact count
    printf("Total contacts: %d\n", addressBook->contactCount);
   
}

void initialize(AddressBook *addressBook)
{
    addressBook->contactCount = 0;

    FILE *file = fopen("contact.txt", "r");
    if (file == NULL)
    {
        printf("No existing contacts found. Starting fresh.\n");
        return;
    }

    // Read each contact from the file using a for loop
    for (int i = 0; i < MAX_CONTACTS; i++)
    {
        if (fscanf(file, "%[^,],%[^,],%[^\n]\n",addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email) == 3)
        {
            addressBook->contactCount++;
        }
        else
        {
            break; 
        }
    }

    fclose(file);
}


void saveAndExit(AddressBook *addressBook) 
{
    exit(EXIT_SUCCESS); // Exit the program
}


void createContact(AddressBook *addressBook)
{
	/* Define the logic to create a Contacts */
    
    //To add name of contact
    int attempts = 0;
    while (attempts < 3) 
    {
        int valid = 1;
        printf("Enter the name: ");
        scanf(" %[^\n]", addressBook->contacts[addressBook->contactCount].name);

        for (int i = 0; addressBook->contacts[addressBook->contactCount].name[i] != '\0'; i++) 
        {
            if (!isalpha(addressBook->contacts[addressBook->contactCount].name[i]) && addressBook->contacts[addressBook->contactCount].name[i] != ' ') 
            {
                valid = 0;
                break;
            }
        }

        if (valid) 
        {
            break;
        }
        attempts++;
        printf("Invalid name! Only alphabets allowed. Attempts left: %d\n", 3 - attempts);
    }
    if (attempts == 3) 
    {
        printf("Max attempts reached! Contact not added.\n");
        return;
    }
    
    //To add phone number
    while (attempts < 3) 
    {
        int valid = 1;
        printf("Enter phone number (10 digits): ");
        scanf(" %s", addressBook->contacts[addressBook->contactCount].phone);

  
        if (strlen(addressBook->contacts[addressBook->contactCount].phone) != 10)
        {
            valid = 0;
        }

        
        for (int i = 0; i < strlen(addressBook->contacts[addressBook->contactCount].phone); i++)
        {
            if (!isdigit(addressBook->contacts[addressBook->contactCount].phone[i]))
            {
                valid = 0;
            }
        }

        for (int i = 0; i < addressBook->contactCount; i++)
        {
            if (strcmp(addressBook->contacts[i].phone, addressBook->contacts[addressBook->contactCount].phone) == 0) 
            {
                valid = 0;
            }
        }
        if (valid)
        {
           break;
        }
        attempts++;
        printf("Invalid or duplicate phone number! Attempts left: %d\n", 3 - attempts);
    }
    if (attempts == 3)
     {
        printf("Max attempts reached! Contact not added.\n");
        return;
    }
    
    //To add email id
   attempts = 0;
    while (attempts < 3) 
    {
        int valid = 1;
        printf("Enter email: ");
        scanf(" %s", addressBook->contacts[addressBook->contactCount].email);

        char *email = addressBook->contacts[addressBook->contactCount].email;
        int len = strlen(email);
        if (!(email[0] >= 'a' && email[0] <= 'z'))
        {
            valid = 0;
        }
         
         char *at = strchr(email, '@');
         if (!at) 
        {
            valid = 0;
        }

    if (len < 5 || strcmp(email + len - 4, ".com") != 0)
    {
        valid = 0;
    }

    for (int i = 0; i < addressBook->contactCount; i++) 
    {
        if (strcmp(addressBook->contacts[i].email, email) == 0) 
        { 
            valid = 0;
        }
    }

        for (int i = 0; i < len; i++)
        {
            if (isupper(email[i]))
            {
                valid = 0;
            }
        }
        
        if (valid)
        {
            break;
        }
        attempts++;
        printf("Invalid or duplicate email! Attempts left: %d\n", 3 - attempts);
    }
    if (attempts == 3)
    {
        printf("Max attempts reached! Contact not added.\n");
        return;
    }
    addressBook->contactCount++;
    printf("Contact added successfully!\n");
 
} 

int searchContact(AddressBook *addressBook) 
{
    /* Define the logic for search */
    int option;
    char query[100];
    int index= -1;
    int i;

    do 
    {
        printf("\n-----Search option:-----\n");
        printf("1. Search by name\n");
        printf("2. Search by phone number\n");
        printf("3. Search by email id\n");
        printf("4. Exit\n");
        printf("Enter your option: ");
        scanf("%d", &option);


        switch (option)
         {
            case 1:
            // Search by name 
            {
                int valid = 1;
                printf("Enter name to search: ");
                scanf(" %[^\n]", query);

                for (int i = 0; query[i] != '\0'; i++) 
                {
                    if (!isalpha(query[i]) && query[i] != ' ') 
                    {
                        valid = 0;
                        break;
                    }
                }

                if (!valid) 
                {
                    printf("Invalid name! Only alphabets allowed.\n");
                    break;
                }

            int matchCount = 0;
            for (int i = 0; i < addressBook->contactCount; i++)
            {
                if (strcasecmp(addressBook->contacts[i].name, query) == 0)
                {
                    printf("%d. %s | %s | %s\n", i + 1,addressBook->contacts[i].name,addressBook->contacts[i].phone, addressBook->contacts[i].email);
                    matchCount++;
                }
            }

            if (matchCount == 0) 
            {
                printf("No contact found with name %s\n", query);
                return -1;
            }

            
            int choice;
            printf("Multiple contacts found.\n Enter the serial number to select: ");
            scanf("%d", &choice);

            if (choice < 1 || choice > addressBook->contactCount) 
            {
                printf("Invalid selection!\n");
                return -1;
            }

            return choice - 1;  
        }
                
            case 2: 
            // Search by phone
            { 
                int valid = 1;
                printf("Enter phone number to search: ");
                scanf("%s", query);

                if (strlen(query) != 10)
                {
                    valid = 0;
                }
                for (int i = 0; i < strlen(query); i++)
                 {
                    if (!isdigit(query[i]))
                    {
                        valid = 0;
                    }
                }

                if (!valid) 
                {
                    printf("Invalid phone! Must be 10 digits.\n");
                    break;
                }

                for (int i = 0; i < addressBook->contactCount; i++) 
                {
                    if (strcmp(addressBook->contacts[i].phone, query) == 0) 
                    {
                        printf("Found:\n %s | %s | %s\n",addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
                        return i;
                    }
                }
                    printf("No contact found with phone %s\n", query);
                    break;
            }

            case 3: 
            // Search by email
            { 
                int valid = 1;
                printf("Enter email to search: ");
                scanf("%s", query);

                int len = strlen(query);

                if (!(query[0] >= 'a' && query[0] <= 'z'))
                {
                    valid = 0;
                }

                char *at = strchr(query, '@');
                if (!at) 
                {
                    valid = 0;
                }

                if (len < 5 || strcmp(query + len - 4, ".com") != 0) 
                {
                    valid = 0;
                }

                if (!valid) 
                {
                    printf("Invalid email format!\n");
                    break;
                }

                for (int i = 0; i < addressBook->contactCount; i++)
                 {
                    if (strcmp(addressBook->contacts[i].email, query) == 0) 
                    {
                        printf("Found:\n %s | %s | %s\n",addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
                        return i;
                    }
                }
                
                    printf("No contact found with email %s\n", query);
                    break;
            }

            case 4:
                printf("Exiting search...\n");
                break;

            default:
                printf("Invalid option! Try again.\n");
        }

    } while (option != 4);
    return -1;
}


void editContact(AddressBook *addressBook)
{
	/* Define the logic for Editcontact */
    if (addressBook->contactCount == 0)
    {
        printf("No contacts to edit!\n");
        return;
    }

    int option;
    int index;
    char temp[100];
    int valid;   

    do
    {
        printf("\n-----Edit Menu-----\n");
        printf("1. Edit Name\n");
        printf("2. Edit Phone\n");
        printf("3. Edit Email\n");
        printf("4. Exit\n");
        printf("Enter your option: ");
        scanf("%d", &option);

        switch(option)
        {
            case 1: // Edit Name
                index = searchContact(addressBook);
                if (index == -1)
                {
                    printf("Contact not found.\n");
                    break;
                }

                printf("Enter new name: ");
                scanf(" %[^\n]", temp);

                valid = 1; 
                for (int i = 0; temp[i] != '\0'; i++)
                {
                    if (!isalpha(temp[i]) && temp[i] != ' ')
                    {
                        valid = 0;
                    }
                }

                if (valid)
                {
                    strcpy(addressBook->contacts[index].name, temp);
                    printf("Name updated successfully!\n");
                }
                else
                {
                    printf("Invalid name!\n");
                }
                printf("Updated Contact:\n%s | %s | %s\n",addressBook->contacts[index].name,addressBook->contacts[index].phone,addressBook->contacts[index].email);
                break;

            case 2: // Edit Phone
                index = searchContact(addressBook);
                if (index == -1)
                {
                    printf("Contact not found.\n");
                    break;
                }

                printf("Enter new phone (10 digits): ");
                scanf("%s", temp);

                valid = 1; 
                if (strlen(temp) != 10)
                {
                    valid = 0;
                }

                for (int i = 0; i < strlen(temp); i++)
                {
                    if (!isdigit(temp[i]))
                    {
                        valid = 0;
                    }
                }

                for (int i = 0; i < addressBook->contactCount; i++)
                {
                    if (i != index && strcmp(addressBook->contacts[i].phone, temp) == 0)
                    {
                        valid = 0;
                    }
                }

                if (valid)
                {
                    strcpy(addressBook->contacts[index].phone, temp);
                    printf("Phone number updated successfully!\n");
                }
                else
                {
                    printf("Invalid or duplicate phone!\n");
                }
                printf("Updated Contact:\n%s | %s | %s\n",addressBook->contacts[index].name,addressBook->contacts[index].phone,addressBook->contacts[index].email);
                break;

            case 3: // Edit Email
                index = searchContact(addressBook);
                if (index == -1)
                {
                    printf("Contact not found.\n");
                    break;
                }

                printf("Enter new email: ");
                scanf("%s", temp);

                valid = 1; 
                int len = strlen(temp);
                char *at = strchr(temp, '@');

                if (!(temp[0] >= 'a' && temp[0] <= 'z'))
                {
                    valid = 0;
                }
                if (!at)
                {
                    valid = 0;
                }
                if (len < 5 || strcmp(temp + len - 4, ".com") != 0)
                {
                    valid = 0;
                }

                for (int i = 0; i < addressBook->contactCount; i++)
                {
                    if (i != index && strcmp(addressBook->contacts[i].email, temp) == 0)
                    {
                        valid = 0;
                    }
                }

                if (valid)
                {
                    strcpy(addressBook->contacts[index].email, temp);
                    printf("Email updated successfully!\n");
                }
                else
                {
                    printf("Invalid or duplicate email!\n");
                }
                printf("Updated Contact:\n%s | %s | %s\n",addressBook->contacts[index].name,addressBook->contacts[index].phone,addressBook->contacts[index].email);
                break;

            case 4:
                printf("Exiting edit menu...\n");
                break;

            default:
                printf("Invalid option! Try again.\n");
        }

    } while(option != 4);
}

void deleteContact(AddressBook *addressBook)
{
    if (addressBook->contactCount == 0) 
    {
        printf("No contacts to delete!\n");
        return;
    }

    printf("\n----- Delete Contact -----\n");

   
    int index = searchContact(addressBook);

    if (index == -1) 
    {
        printf("Contact not found. Delete cancelled.\n");
        return;
    }

    
    printf("Deleting contact: %s | %s | %s\n",addressBook->contacts[index].name,addressBook->contacts[index].phone,addressBook->contacts[index].email);

    char confirm;
    printf("Are you sure? (y/n): ");
    scanf(" %c", &confirm);

    if (confirm == 'y' || confirm == 'Y')
    {
        for (int i = index; i < addressBook->contactCount - 1; i++) 
        {
            addressBook->contacts[i] = addressBook->contacts[i + 1];
        }
    }

    addressBook->contactCount--;

    printf("Contact deleted successfully!\n");
}

