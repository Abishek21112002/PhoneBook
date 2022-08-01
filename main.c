#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define emptCheck if(emptyCheck()){ \
                        printf("The Phonebook is empty right now.\n"); \
                        break; \
                    }
#define checkIfCreated if(addContact(getContact())) printf("Contact has been addded succesfully\n");

typedef struct contact{
    char firstName[20];
    char lastName[20];
    char phoneNumber[15];
    char address[50];
    char email[50];

    struct contact * nextNodeUsingName;
    struct contact * nextNodeUsingPhoneNumber;

    struct contact * prevNodeUsingName;
    struct contact * prevNodeUsingPhoneNumber;

}node;

typedef node * nodePointer;

nodePointer hashTableUsingNames[26];
nodePointer hashTableUsingPhoneNumbers[99];

char tmp;
char tmpName[30];
char tmpPhoneNumber[15];
char tmpContact[80];
char tmpStr[30];

char * getName(void){

    printf("Enter the full name to search:");
    scanf("\n%[^\n]",tmpName);

    return tmpName;

}
char * getPhoneNumber(void){

    printf("Enter the phone number to search:");
    scanf("\n%[^\n]",tmpPhoneNumber);

    return tmpPhoneNumber;

}
char * getContact(void){

    printf("Enter the Contact Details in this format (FirstName,LastName,Phone,Address,Email):");
    scanf("\n%[^\n]",tmpContact);

    return tmpContact;

}
char getLetter(void){

    printf("Enter the letter:");
    scanf("\n%c",&tmp);
    tmp = toupper(tmp);

    return tmp;

}
char * getString(void){

    printf("Enter the full name or the phone number of the contact:");
    scanf("\n%[^\n]",tmpStr);

    return tmpStr;

}

int getHashKeyForName(char * name){

    return name[0]-'A';

}

int getHashKeyForPhoneNumber(char * phone){

    int key=0;
    int num;

    for(int i=8;i<=9;++i){

        num=phone[i]-'0';

        key=key*10+num;

    }

    return key;

}

void displayMenu(void){

    printf("***************************MENU*************************\n");
    printf("1. Display Contacts\n");
    printf("2. Search Contacts By Name\n");
    printf("3. Search Contacts By Phone\n");
    printf("4. Add New Contact\n");
    printf("5. Modify Existing Contact\n");
    printf("6. Remove Existing Contact\n");
    printf("7. Total Number Of Contacts\n");
    printf("8. Truncate PhoneBook\n");
    printf("9. Load Contacts From A File\n");
    printf("10. Exit\n");
    printf("11. Display Contacts Stored Based On Phone Numbers\n");
    printf("12. View Contacts With Their First Letters\n");
    printf("13. Create A Backup Of Contacts\n");
    printf("********************************************************\n");

}

void initialize(void){

    for(int i=0;i<99;++i){

        hashTableUsingPhoneNumbers[i] = NULL;

    }

    for(int i=0;i<26;++i){

        hashTableUsingNames[i] = NULL;

    }

}

bool emptyCheck(void){

    for(int i=0;i<26;++i){

        if(hashTableUsingNames[i]){

            return false;

        }

    }

    return true;

}

void displayNodeInHashTableName(nodePointer start){

    if(start == NULL){

        return;

    }
    nodePointer ptr = start;
    printf("-------------------------------------------------%c------------------------------------------------\n",start->firstName[0]);

    while(ptr){

        printf("%s %s\n",ptr->firstName,ptr->lastName);
        ptr = ptr->nextNodeUsingName;

    }

    printf("\n");


}

void displayContactsInHashTableName(void){

    if(emptyCheck()){

        printf("The Phonebook is empty right now.\n");
        return;

    }

    for(int i=0;i<26;++i){

        displayNodeInHashTableName(hashTableUsingNames[i]);

    }

}

void displayContactNamesUsingStartingLetters(char letter){

    letter = toupper(letter);


    nodePointer ptr = hashTableUsingNames[letter-'A'];

    if(ptr == NULL){

        printf("No Contacts Starting with Letter %c\n", letter);
        return;

    }

    printf("------------------------------------------------------------------------------------------------------------\n");

    while(ptr != NULL){

        printf("%s %s\n",ptr->firstName,ptr->lastName);
        ptr=ptr->nextNodeUsingName;

    }

    printf("------------------------------------------------------------------------------------------------------------\n");


}

nodePointer findContactUsingName(char * name,bool search){


    nodePointer ptr = hashTableUsingNames[getHashKeyForName(name)];

    if(ptr != NULL){

        char tmpName[40];

        do{
            strcpy(tmpName,"");
            strcat(tmpName,ptr->firstName);
            strcat(tmpName," ");
            strcat(tmpName,ptr->lastName);

            if(!strcmp(tmpName,name)){

                return ptr;

            }

            ptr = ptr->nextNodeUsingName;

        }while(ptr!=NULL);

    }

    if(search){

        printf("There is no such Contact.\n");
        printf("Here are some contacts that start with the letter %c\n",name[0]);
        displayContactNamesUsingStartingLetters(name[0]);

    }

    return NULL;

}

nodePointer findContactUsingPhoneNumber(char * phoneNumber,bool search){

    nodePointer ptr = hashTableUsingPhoneNumbers[getHashKeyForPhoneNumber(phoneNumber)];

    if(ptr != NULL){

        char tmpPhoneNumber[15];

        do{

            strcpy(tmpPhoneNumber,ptr->phoneNumber);

            if(!strcmp(tmpPhoneNumber,phoneNumber)){

                return ptr;

            }

            ptr=ptr->nextNodeUsingPhoneNumber;

        }while(ptr!=NULL);


    }

    if(search){

        printf("There is no such Contact.\n");

    }

    return NULL;


}

bool contactCheck(char * contact){

    int i = 0;

    for(int j=0;contact[j];++j){

        if(contact[j] == ','){

            i++;

        }

    }

    return i == 4;

}
bool duplicateContactCheck(char * contact){

    char name[30];

    char phoneNumber[15];

    char * duplicate = strdup(contact);

    char * piece = strtok(duplicate,",");
    strcpy(name,piece);
    strcat(name," ");
    piece = strtok(NULL,",");
    strcat(name,piece);


    piece = strtok(NULL,",");
    strcpy(phoneNumber,piece);

    if(findContactUsingPhoneNumber(phoneNumber,false)){

        printf("There is already a contact with that phone number.Please Try to distinguish them.\n");
        return true;

    }

    if(findContactUsingName(name,false)){

        printf("There is already a contact with that name.Please Try to Distinguish them.\n");
        return true;

    }

    return false;
}
bool phoneNumberCheck(char * phoneNumber){

    int i;

    for(i=0;phoneNumber[i];++i){

        if(isalpha(phoneNumber[i])){

            return false;

        }

    }

    return i == 10;

}

nodePointer newContact(char * contact){


    nodePointer newNode = (nodePointer) malloc(sizeof(node));
    newNode->nextNodeUsingName = newNode->nextNodeUsingPhoneNumber = newNode->prevNodeUsingName = newNode->prevNodeUsingPhoneNumber = NULL;

    char * piece;
    char * duplicate = strdup(contact);


    piece = strtok(duplicate,",");
    strcpy(newNode->firstName,piece);
    piece = strtok(NULL,",");
    strcpy(newNode->lastName,piece);
    piece= strtok(NULL,",");

    if(!phoneNumberCheck(piece)){


        free(newNode);
        return NULL;


    }

    strcpy(newNode->phoneNumber,piece);
    piece= strtok(NULL,",");
    strcpy(newNode->address,piece);
    piece= strtok(NULL,",");
    strcpy(newNode->email,piece);
    return newNode;


}

nodePointer addNode(nodePointer start,nodePointer new){

    if(start==NULL){

        start=new;

    }

    else{

        nodePointer ptr = start;

        while(ptr->nextNodeUsingPhoneNumber != NULL){

            ptr = ptr->nextNodeUsingPhoneNumber;

        }

        ptr->nextNodeUsingPhoneNumber = new;
        new->prevNodeUsingPhoneNumber = ptr;


    }

    return start;

}
bool isGreater(nodePointer a,nodePointer b){

    if(!strcmp(a->firstName,b->firstName)){

        if(strcmp(a->lastName,b->lastName)>0){

            return true;

        }

        else{

            return false;

        }

    }

    else{

        if(strcmp(a->firstName,b->firstName)>0){

            return true;

        }

        else{

            return false;

        }

    }

}
nodePointer sortedAdd(nodePointer start,nodePointer new){

    nodePointer sortedStart = start;

    if(sortedStart == NULL || isGreater(sortedStart,new)){

        if(sortedStart){

            sortedStart->prevNodeUsingName = new;

        }

        new->nextNodeUsingName = sortedStart;
        sortedStart = new;

    }

    else{

        nodePointer ptr = sortedStart;

        while(ptr->nextNodeUsingName != NULL && !isGreater(ptr,new)){

            ptr = ptr->nextNodeUsingName;

        }

        if(ptr->nextNodeUsingName){

            new->nextNodeUsingName = ptr;
            ptr->prevNodeUsingName->nextNodeUsingName = new;
            new->prevNodeUsingName = ptr->prevNodeUsingName;
            ptr->prevNodeUsingName = new;

        }

        else{

            if(isGreater(ptr,new)){

                new->nextNodeUsingName = ptr;
                ptr->prevNodeUsingName->nextNodeUsingName = new;
                new->prevNodeUsingName = ptr->prevNodeUsingName;
                ptr->prevNodeUsingName = new;

            }

            else{

                ptr->nextNodeUsingName = new;
                new->prevNodeUsingName = ptr;

            }

        }

    }

    return sortedStart;

}

bool addContact(char * contact){

    if(!contactCheck(contact)){

        printf("Contact Invalid\n");
        printf("!!!!Contact has not been added.\n");
        return false;

    }

    if(duplicateContactCheck(contact)){

        printf("!!!!Contact has not been added.\n");
        return false;

    }
    nodePointer new = newContact(contact);


    if(new == NULL){

        printf("Invalid Phone Number.Please enter a valid one.\n");
        printf("!!!!Contact has not been added.\n");
        return false;

    }

    hashTableUsingNames[getHashKeyForName(new->firstName)] = sortedAdd(hashTableUsingNames[getHashKeyForName(new->firstName)],new);
    hashTableUsingPhoneNumbers[getHashKeyForPhoneNumber(new->phoneNumber)] = addNode(hashTableUsingPhoneNumbers[getHashKeyForPhoneNumber(new->phoneNumber)],new);

    return true;


}


void freeContact(nodePointer ptr){

    if(ptr->nextNodeUsingName && ptr->prevNodeUsingName){

        ptr->prevNodeUsingName->nextNodeUsingName = ptr->nextNodeUsingName;
        ptr->nextNodeUsingName->prevNodeUsingName = ptr->prevNodeUsingName;

    }

    else if(ptr->nextNodeUsingName){

        hashTableUsingNames[getHashKeyForName(ptr->firstName)] = ptr->nextNodeUsingName;

    }

    else if(ptr->prevNodeUsingName){

        ptr->prevNodeUsingName->nextNodeUsingName = NULL;

    }

    else if(!ptr->nextNodeUsingName && !ptr->prevNodeUsingName){

        hashTableUsingNames[getHashKeyForName(ptr->firstName)] = ptr->nextNodeUsingName;

    }

    if(ptr->nextNodeUsingPhoneNumber && ptr->prevNodeUsingPhoneNumber){

        ptr->prevNodeUsingPhoneNumber->nextNodeUsingPhoneNumber = ptr->nextNodeUsingPhoneNumber;
        ptr->nextNodeUsingPhoneNumber->prevNodeUsingPhoneNumber = ptr->prevNodeUsingPhoneNumber;

    }

    else if(ptr->nextNodeUsingPhoneNumber){

        hashTableUsingPhoneNumbers[getHashKeyForPhoneNumber(ptr->phoneNumber)] = ptr->nextNodeUsingPhoneNumber;

    }

    else if(ptr->prevNodeUsingPhoneNumber){

        ptr->prevNodeUsingPhoneNumber->nextNodeUsingPhoneNumber = NULL;

    }

    else if(!ptr->prevNodeUsingPhoneNumber && !ptr->nextNodeUsingPhoneNumber){

        hashTableUsingPhoneNumbers[getHashKeyForPhoneNumber(ptr->phoneNumber)] = NULL;

    }

    free(ptr);
}
void removeContact(char * tmpStr){

    nodePointer ptr;

    if(phoneNumberCheck(tmpStr)){

        ptr = findContactUsingPhoneNumber(tmpStr,false);

        if(!ptr){

            printf("There is no such contact with that phone number.\n");
            return;

        }

    }

    else{

        ptr = findContactUsingName(tmpStr,false);

        if(!ptr){

            printf("There is no such contact with that name.\n");
            printf("Here are some contacts that start with the letter G.\n");
            displayContactNamesUsingStartingLetters(tmpStr[0]);
            return;

        }

    }

    freeContact(ptr);


}

nodePointer copyContact(nodePointer ptr,char * tmp,int p){

    nodePointer copy = (nodePointer)malloc(sizeof(node));
    copy->nextNodeUsingName = copy->nextNodeUsingPhoneNumber = copy->prevNodeUsingName = copy->prevNodeUsingPhoneNumber = NULL;

    switch (p)
    {
        case 1:
            strcpy(copy->firstName,tmp);
            strcpy(copy->lastName,ptr->lastName);
            strcpy(copy->phoneNumber,ptr->phoneNumber);
            break;

        case 2:
            strcpy(copy->firstName,ptr->firstName);
            strcpy(copy->lastName,tmp);
            strcpy(copy->phoneNumber,ptr->phoneNumber);
            break;
        case 3:
            strcpy(copy->firstName,ptr->firstName);
            strcpy(copy->lastName,ptr->lastName);
            strcpy(copy->phoneNumber,tmp);
            break;
    }

    strcpy(copy->address,ptr->address);
    strcpy(copy->email,ptr->email);

    return copy;
}
void modifyContact(char * tmpStr){

    nodePointer ptr;

    if(phoneNumberCheck(tmpStr)){

        ptr = findContactUsingPhoneNumber(tmpStr,false);

        if(!ptr){

            printf("There is no such contact with that phone number.\n");
            return;

        }

    }

    else{

        ptr = findContactUsingName(tmpStr,false);

        if(!ptr){

            printf("There is no such contact with that name.\n");
            printf("Here are some contacts that start with the letter G.\n");
            displayContactNamesUsingStartingLetters(tmpStr[0]);
            return;

        }

    }

    int choice;
    char tmp1[40],tmp2[40];
    nodePointer new;

    printf("Enter the field which you wish to modify\n1. First Name\n2. Last Name\n3. Phone\n4. Address\n5. Email\n6. Exit\nEnter your choice:");
    scanf("%d",&choice);

    switch (choice)
    {
        case 1:
            printf("Enter the new value for the first name:");
            scanf("\n%[^\n]",tmp1);

            strcpy(tmp2,tmp1);
            strcat(tmp2," ");
            strcat(tmp2,ptr->lastName);

            if(findContactUsingName(tmp2,false)){

                printf("There seems to be a contact with the same name.\n");
                return;

            }

            new = copyContact(ptr,tmp1,1);
            removeContact(ptr->phoneNumber);
            hashTableUsingNames[getHashKeyForName(tmp2)] = sortedAdd(hashTableUsingNames[getHashKeyForName(tmp2)],new);
            hashTableUsingPhoneNumbers[getHashKeyForPhoneNumber(new->phoneNumber)] = addNode(hashTableUsingPhoneNumbers[getHashKeyForPhoneNumber(new->phoneNumber)],new);
            return;
        case 2:
            printf("Enter the new value for the last name:");
            scanf("\n%[^\n]",tmp1);

            strcpy(tmp2,ptr->firstName);
            strcat(tmp2," ");
            strcat(tmp2,tmp1);

            if(findContactUsingName(tmp2,false)){

                printf("There seems to be a contact with the same name.\n");
                return;

            }

            new = copyContact(ptr,tmp1,2);
            removeContact(ptr->phoneNumber);
            hashTableUsingNames[getHashKeyForName(tmp2)] = sortedAdd(hashTableUsingNames[getHashKeyForName(tmp2)],new);
            hashTableUsingPhoneNumbers[getHashKeyForPhoneNumber(new->phoneNumber)] = addNode(hashTableUsingPhoneNumbers[getHashKeyForPhoneNumber(new->phoneNumber)],new);
            return;
        case 3:
            printf("Enter the new value for the phone number:");
            scanf("\n%[^\n]",tmp1);

            if(findContactUsingPhoneNumber(tmp1,false)){

                printf("There seems to be a contact with the same phone number.\n");
                return;

            }
            new = copyContact(ptr,tmp1,3);
            strcpy(tmp2,ptr->firstName);
            strcat(tmp2," ");
            strcat(tmp2,ptr->lastName);
            removeContact(tmp2);
            hashTableUsingNames[getHashKeyForName(new->firstName)] = sortedAdd(hashTableUsingNames[getHashKeyForName(new->firstName)],new);
            hashTableUsingPhoneNumbers[getHashKeyForPhoneNumber(tmp1)] = addNode(hashTableUsingPhoneNumbers[getHashKeyForPhoneNumber(tmp1)],new);
            return;
        case 4:
            printf("Enter the new value for the address:");
            scanf("\n%[^\n]",tmp1);

            strcpy(ptr->address,tmp1);
            return;

        case 5:
            printf("Enter the new value for the email:");
            scanf("\n%[^\n]",tmp1);

            stpcpy(ptr->email,tmp1);
            return;
        case 6:
            return;
        default:
            printf("Invalid Choice\n");
            return;
    }

}

void displayTotalContactsInPhoneBook(void){


    int total;

    for(int i=0;i<26;++i){

        nodePointer ptr = hashTableUsingNames[i];

        while(ptr!=NULL){

            total++;
            ptr=ptr->nextNodeUsingName;

        }

    }

    printf("You have %d contacts in your PhoneBook.\n",total);



}

void truncatePhoneBook(void){

    nodePointer ptr;

    for(int i=0;i<26;++i){

        if(hashTableUsingNames[i]){

            hashTableUsingNames[i] = NULL;

        }

    }

    for(int i=0;i<99;++i){

        if(hashTableUsingPhoneNumbers[i]){

            ptr = hashTableUsingPhoneNumbers[i];
            while(ptr){

                hashTableUsingPhoneNumbers[i] = ptr->nextNodeUsingPhoneNumber;
                free(ptr);
                ptr = hashTableUsingPhoneNumbers[i];

            }

        }

    }


}

void loadContactsFromAFile(void){

    FILE * fp = fopen("/home/shek/CLionProjects/PhoneBook/Contacts.txt","r");

    if(fp == NULL){

        printf("Error in opening the file.");

    }

    else{

        char contact[200];

        while(fscanf(fp,"\n%[^\n]",contact)!=EOF){

            fgetc(fp);
            addContact(contact);
        }
        printf("Load Successful\n");
    }

    fclose(fp);

}

void displayNodeInHashTablePhoneNumber(nodePointer start){

    if(start == NULL){
        return;
    }

    nodePointer ptr = start;
    printf("-------------------------------------------------%d-------------------------------------------------------------\n",getHashKeyForPhoneNumber(ptr->phoneNumber));


    while(ptr){

        printf("%s %s\n",ptr->firstName,ptr->lastName);
        ptr =ptr->nextNodeUsingPhoneNumber;

    }

    printf("\n");

}

void displayContactsInHashTablePhoneNumber(void){

    if(emptyCheck()){

        printf("The Phonebook is empty right now.\n");
        return;

    }

    for(int i=0;i<99;++i){

        displayNodeInHashTablePhoneNumber(hashTableUsingPhoneNumbers[i]);

    }

}
void backupContactsToAFile(void){

    FILE * fp=fopen("/home/shek/CLionProjects/PhoneBook/Contacts.txt","a");
    for(int i=0;i<26;++i){

        if(hashTableUsingNames[i]){

            nodePointer ptr = hashTableUsingNames[i];
            while(ptr != NULL){


                fprintf(fp,"%s,%s,%s,%s,%s\n",ptr->firstName,ptr->lastName,ptr->phoneNumber,ptr->address,ptr->email);
                ptr = ptr->nextNodeUsingName;


            }

        }

    }

    printf("Backup Successful\n");

    fclose(fp);
}

void display(nodePointer ptr){


    if(ptr != NULL){

        printf("------------------------------------------------------------------------------------------------------------------\n");
        printf("First Name: %s\nLast Name: %s\nPhone: %s\nAddress: %s\nEmail: %s\n", ptr->firstName, ptr->lastName,
               ptr->phoneNumber, ptr->address, ptr->email);
        printf("------------------------------------------------------------------------------------------------------------------\n");
        return;

    }


}

bool execute(int ch){


    switch(ch){

        case 1:
            displayContactsInHashTableName();
            return true;

        case 2:

            emptCheck

            display(findContactUsingName(getName(),true));
            return true;

        case 3:

            emptCheck

            display(findContactUsingPhoneNumber(getPhoneNumber(),true));
            return true;

        case 4:

            checkIfCreated

            return true;

        case 5:

            emptCheck

            modifyContact(getString());
            return true;

        case 6:

            emptCheck

            removeContact(getString());
            return true;

        case 7:
            displayTotalContactsInPhoneBook();
            return true;

        case 8:

            emptCheck

            truncatePhoneBook();
            return true;

        case 9:
            loadContactsFromAFile();
            return true;

        case 10:
            return false;

        case 11:

            emptCheck

            displayContactsInHashTablePhoneNumber();
            return true;

        case 12:

            emptCheck

            displayContactNamesUsingStartingLetters(getLetter());
            return true;

        case 13:

            emptCheck

            backupContactsToAFile();
            return true;

        default:
            printf("Invalid Choice\n");
            return true;

    }


}

void run(void){

    initialize();

    loadContactsFromAFile();

    int choice;

    do{

        displayMenu();

        printf("Enter the choice:");

        scanf("%d",&choice);

    }while(execute(choice));


}

int main(void){

    run();

    return 0;

}