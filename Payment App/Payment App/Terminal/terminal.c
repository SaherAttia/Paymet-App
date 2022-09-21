#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "terminal.h"


//function to get the current dete in the format DD/MM/YYYY, e.g 25/06/2022
//and put it in transaction date
EN_terminalError_t getTransactionDate(ST_terminalData_t* termData)
{
	SYSTEMTIME t;
	GetLocalTime(&t);

	termData->transactionDate[0] = '0' + (t.wDay / 10);  //to get the second digit of the day
	termData->transactionDate[1] = '0' + (t.wDay % 10);  //to get the first digit of the day
	termData->transactionDate[2] = '/';
	termData->transactionDate[3] = '0' + (t.wMonth / 10); //to get the second digit of the month
	termData->transactionDate[4] = '0' + (t.wMonth % 10); //to get the first digit of the month
	termData->transactionDate[5] = '/';
	termData->transactionDate[6] = '0' + (t.wYear / 1000); //to get the last digit of the year
	termData->transactionDate[7] = '0' + ((t.wYear % 1000) / 100); //to get the third digit of the year
	termData->transactionDate[8] = '0' + ((t.wYear % 100) / 10); //to get the second digit of the year
	termData->transactionDate[9] = '0' + (t.wYear % 10); //to get the first digit of the year
	termData->transactionDate[10] = '\0';

	return OK_T;
}


//This function compares the card expiry date with the transaction date.
//If the card expiration date is before the transaction date will return EXPIRED_CARD, else return OK.
//transaction date in the format DD/MM/YYYY, e.g 25/06/2022
//expiry date in the format "MM/YY", e.g "05/25".
EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData)
{
	int expiary_month = ((cardData.cardExpirationDate[0] - '0') * 10) + (cardData.cardExpirationDate[1] - '0');
	int expiary_year = ((cardData.cardExpirationDate[3] - '0') * 10) + (cardData.cardExpirationDate[4] - '0');
	int transaction_month = ((termData.transactionDate[3] - '0') * 10) + (termData.transactionDate[4] - '0');
	int transaction_year = ((termData.transactionDate[8] - '0') * 10) + (termData.transactionDate[9] - '0');


	if (transaction_year > expiary_year)
	{
		return EXPIRED_CARD;
	}
	else if (transaction_year < expiary_year)
	{
		return OK_T;
	}
	else
	{
		if (transaction_month < expiary_month)
		{
			return OK_T;
		}
		else
		{
			return EXPIRED_CARD;
		}
	}
}


//This function asks for the transaction amountand saves it into terminal data.
//If the transaction amount is less than or equal to 0 will return INVALID_AMOUNT, else return OK.
EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData)
{
	float amount;
	printf("Enter transaction amount: ");
	scanf("%f", &amount);

	if (amount <= 0)
	{
		return INVALID_AMOUNT;
	}
	else
	{
		termData->transAmount = amount;
		return OK_T;
	}
}


//This function compares the transaction amount with the terminal max amount.
//If the transaction amount is larger than the terminal max amount will return EXCEED_MAX_AMOUNT, else return OK.
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData)
{
	if (termData->transAmount > termData->maxTransAmount)
	{
		return EXCEED_MAX_AMOUNT;
	}
	else
	{
		return OK_T;
	}
}


//This function sets the maximum allowed amount into terminal data.
//If transaction max amount less than or equal to 0 will return INVALID_MAX_AMOUNT error, else return OK.
EN_terminalError_t setMaxAmount(ST_terminalData_t* termData)
{
	float max_amount;
	printf("Enter maximum allowed amount: ");
	scanf("%f", &max_amount);
	if (max_amount <= 0)
	{
		return INVALID_MAX_AMOUNT;
	}
	else
	{
		termData->maxTransAmount = max_amount;
		return OK_T;
	}
}