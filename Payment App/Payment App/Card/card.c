#include <stdio.h>
#include "card.h"
#include <stdlib.h>
#include <string.h>


//function to read a string of unknown length and return it
char* Read_String()
{
    char* s = calloc(1, sizeof(char));
    char t;
    int len;
    while (scanf_s("%c", &t) == 1)
    {
        if (t == '\n')
            break;
        len = strlen(s);
        s = realloc(s, len + 1);
        *(s + len) = t;
        *(s + len + 1) = '\0';
    }
    return s;
}


//This function will ask for the cardholder's name and store it into card data.
//Card holder name is 24 characters string max and 20 min.
//If the cardholder name is NULL, less than 20 characters or more than 24 will return WRONG_NAME error, else return OK.
EN_cardError_t getCardHolderName(ST_cardData_t* cardData)
{
	printf("Enter card holder name[20-24 characters]: ");
    char* temp = Read_String();

    if (strlen(temp) > 24 || strlen(temp) < 20)
    {
        return WRONG_NAME;
    }
    else
    {
        for (int i = 0; i < 25; i++)
        {
            cardData->cardHolderName[i] = temp[i];
        }
        return OK;
    }
}



//This function will ask for the card expiry dateand store it in card data.
//Card expiry date is 5 characters string in the format "MM/YY", e.g "05/25".
//If the card expiry date is NULL, less or more than 5 characters, 
//or has the wrong format will return WRONG_EXP_DATE error, else return OK.
EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData)
{
    printf("Enter card expiary date[MM/YY]: ");
    char* temp = Read_String();
    int month = (temp[0] - '0') * 10 + (temp[1] - '0');
    int year = (temp[3] - '0') * 10 + (temp[4] - '0');

    if (strlen(temp) == 5)
    {
        if (temp[2] != '/' || month > 12 || month < 1 || year < 0)
        {
            return WRONG_EXP_DATE;
        }
        else
        {
            for (int i = 0; i < 6; i++)
            {
                cardData->cardExpirationDate[i] = temp[i];
            }
            return OK;
        }
    }
    else
    {
        return WRONG_EXP_DATE;
    }
}


//This function will ask for the card's Primary Account Number and store it in card data.
//PAN is 20 characters alphanumeric only string 19 character max, and 16 character min.
//If the PAN is NULL, less than 16 or more than 19 characters, will return WRONG_PAN error, else return OK.
EN_cardError_t getCardPAN(ST_cardData_t* cardData)
{
    
    fseek(stdin, 0, SEEK_END);
        char PAN[100];
        int i;
        for (i = 0; i < 100; i++)PAN[i] = '\0';
        printf("Enter Card PAN: ");
        i = 0;
        char temp;
        fseek(stdin, 0, SEEK_END);
        while ((temp = getchar()) != '\n') {
            PAN[i] = temp;
            i++;
        }
        if (!PAN)return WRONG_PAN;
        if (PAN[19] != '\0' || PAN[14] == '\0')return WRONG_PAN;
        for (int i = 0; i < 20; i++) {
            cardData->primaryAccountNumber[i] = PAN[i];
        }
    return OK;
}