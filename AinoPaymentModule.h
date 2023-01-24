/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   AinoPaymentModule.h
 * Author: prodev-ahz
 *
 * Created on August 29, 2018, 5:24 PM
 */

#ifndef AINOPAYMENTMODULE_H
#define AINOPAYMENTMODULE_H

#ifdef __cplusplus
extern "C" {
#endif

    int InitializeModule(char *config);
    int DetectCard(char *card_info);
    int CheckBalance(double *balance, char* pan);
    int Debit(double value, const char *localTime, char *pan, double *balanceBefore, double *balanceAfter,char *transaction_data);
    int setCardUserData(const char* key, const char* userData);
    int getCardUserData(const char* key, const char* userData);

#ifdef __cplusplus
}
#endif

#endif /* AINOPAYMENTMODULE_H */

