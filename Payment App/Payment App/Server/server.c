#include "server.h"
#include <stdio.h>

static int sequence_num = 0;
int index = -1;
ST_accountsDB_t accounts_DB[255] = {
	{1000.0, "8989374615436851"},
	{2000.0, "5807007076043875"},
	{3000.0, "12345678901234567"},
	{4000.0, "78945678901234567"},
	{5000.0, "12345678945634567"},
	{6000.0, "12312378901234567"}
};


//This function will take card data and validate these data.
//It checks if the PAN exists or not in the server's database and set the index of account if exist
EN_serverError_t isValidAccount(ST_cardData_t* cardData)
{
	for (int i = 0; i < (sizeof(accounts_DB) / sizeof(ST_accountsDB_t)); i++)
	{
		unsigned char* panNum = cardData->primaryAccountNumber;
		int valid_A = 0;
		for (int j = 0; j < 20; j++)
		{
			if (panNum[j] == accounts_DB[i].primaryAccountNumber[j])
			{
				valid_A = 1;
			}
			else
			{
				valid_A = 0;
				break;

			}
		}
		if (valid_A == 1)
		{
			index = i;
			return OK_S;
		}
	}
	return ACCOUNT_NOT_FOUND;
}


//This function will take terminal dataand validate these data.
//It checks if the transaction's amount is available or not.
//If the transaction amount is greater than the balance in the database will return LOW_BALANCE, else will return OK
EN_serverError_t isAmountAvailable(ST_terminalData_t* termData)
{
	if (termData->transAmount > accounts_DB[index].balance)
	{
		return LOW_BALANCE;
	}
	else
	{
		return OK_S;
	}
}


EN_serverError_t saveTransaction(ST_transaction_t* transData)
{
	sequence_num++;
	FILE* file;
	file = fopen("./TransactionsDB.txt", "a");
	if (!file) {
		return SAVING_FAILED;
	}


	fprintf(file, "====================================================================\n");
	fprintf(file, "Card Holder Name: %s\n", transData->cardHolderData.cardHolderName);
	fprintf(file, "PAN: %s\n", transData->cardHolderData.primaryAccountNumber);
	fprintf(file, "Expiry Date: %s\n", transData->cardHolderData.cardExpirationDate);
	fprintf(file, "Max Amount: %f\n", transData->terminalData.maxTransAmount);
	fprintf(file, "Amount: %f\n", transData->terminalData.transAmount);
	fprintf(file, "Transaction State: ");
	if (transData->transState == APPROVED) {
		fprintf(file, "APPROVED\n");
	}
	else if (transData->transState == DECLINED_INSUFFECIENT_FUND) {
		fprintf(file, "DECLINED_INSUFFECIENT_FUND\n");
	}
	else if (transData->transState == DECLINED_STOLEN_CARD) {
		fprintf(file, "DECLINED_STOLEN_CARD\n");
	}
	fprintf(file, "Transaction Sequence Number: %d\n", sequence_num);
	fclose(file);
	return OK_S;
}

EN_transStat_t recieveTransactionData(ST_transaction_t* transData)
{
	if (isValidAccount(&transData->cardHolderData) == ACCOUNT_NOT_FOUND) {
		transData->transState = DECLINED_STOLEN_CARD;
		saveTransaction(transData);
		return DECLINED_STOLEN_CARD;
	}
	if (isAmountAvailable(&transData->terminalData) == LOW_BALANCE) {
		transData->transState = DECLINED_INSUFFECIENT_FUND;
		saveTransaction(transData);
		return DECLINED_INSUFFECIENT_FUND;
	}
	transData->transState = APPROVED;
	accounts_DB[index].balance -= transData->terminalData.transAmount;  //for updating balance
	if (saveTransaction(transData) == SAVING_FAILED) {
		return INTERNAL_SERVER_ERROR;
	}
	return APPROVED;
}


void print_balance(int i)
{
	printf("\ncurrent balance is: %f\n", accounts_DB[i].balance);
}