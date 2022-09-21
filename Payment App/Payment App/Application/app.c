#include "app.h"


void appStart()
{
	ST_cardData_t cardData;
	ST_terminalData_t termData;
	ST_transaction_t transData;


	while (getCardHolderName(&cardData) == WRONG_NAME) {
		printf("\n<WRONG NAME>\n");
	}

	while (getCardExpiryDate(&cardData) == WRONG_EXP_DATE) {
		printf("\n<WRONG EXPIARY DATE>\n");
	}

	while (getCardPAN(&cardData) == WRONG_PAN) {
		printf("\n<WRONG PAN>\n");
	}

	while (setMaxAmount(&termData) == INVALID_MAX_AMOUNT) {
		printf("\n<INVALID AMOUNT>\n");
	}

	while (getTransactionDate(&termData) == WRONG_DATE) {
		printf("\n<WRONG DATE>\n");

	}

	if (isCardExpired(cardData, termData) == EXPIRED_CARD) {
		printf("\n<EXPIRED CARD>\n");
		return;
	}

	while (getTransactionAmount(&termData) == INVALID_AMOUNT) {
		printf("\n<INVALID AMOUNT>\n");
	}

	if (isBelowMaxAmount(&termData) == EXCEED_MAX_AMOUNT) {
		printf("\n<EXCEED MAX AMOUNT>\n");
		return;
	}

	transData.cardHolderData = cardData;
	transData.terminalData = termData;
	EN_transStat_t transStat = recieveTransactionData(&transData);
	if (transStat == DECLINED_STOLEN_CARD) {
		printf("\n<DECLINED STOLEN CARD>\n");
	}
	else if (transStat == DECLINED_INSUFFECIENT_FUND) {
		printf("\n<DECLINED INSUFFECIENT FUND>\n");
	}
	else if (transStat == APPROVED) {
		printf("\n<APPROVED>\n");
	}

}