#ifndef BANKINGCARDMODULE_H
#define	BANKINGCARDMODULE_H

#include <functional>
#include "Definition.h"
using namespace std;


typedef int(*__writeX)(const char* buf, int n);
typedef int(*__readX)(char* buf, int *len);
typedef int(*__sendApduToContactLessCard)(const char* apdu, int apduLen, char* response, int *responseLen);
typedef int(*__sendApduToContactCard)(const char* apdu, int apduLen, char* response, int *responseLen);
typedef int(*__saveTransactionData)(const char* dataRaw, const char* txnStatus);
typedef int(*__showMessage)(const char* value, int timeout);
typedef int(*__logger)(const char* msg, const char* cause, int level);
typedef int(*__beep)(int n, int duration, int delay);

class BankingCardModule {
private:

    /**
     *=========================================================================
     * Callback functions below are interface functions that must be implemented
     * inside banking library  
     *=========================================================================
     */
    int(*fn_getVersion)(char*);
    void(*fn_setConfig)(BankConfigParam);
    int(*fn_init)(char*);
    // Check balance function will be called once needed by core 
    int(*fn_checkBalace)(PiccParam*, double*, char*);
    // Debit function of banking lib,this function will be called once transaction call
    int(*fn_debit)(PiccParam*, double, const char*, char*, double*, double*);
    int(*fn_isMycard)(PiccParam*, char*);
    int(*fn_setCardUserData)(PiccParam*, const char*, const char*);
    int(*fn_getCardUserData)(PiccParam*, const char*, char*);
    int(*fn_getLibVersion)(char*);
    int(*fn_setWriteXCallBack)(__writeX);
    int(*fn_setReadXCallBack)(__readX);
    int(*fn_setSendApduToContactCardCallBack)(__sendApduToContactCard);
    int(*fn_setSendApduToContactLessCardCallBack)(__sendApduToContactLessCard);
    int(*fn_setShowMessageCallBack)(__showMessage);
    int(*fn_setLogCallBack)(__logger);
    int(*fn_setSaveTransactionDataCallBack)(__saveTransactionData);
    int(*fn_setBeepCallBack)(__beep);

    int bankId;
    string bankName;
    string libraryName;
    bool initialized;
    string errorMessage;
    string config;
    bool active;
    int sam_slot;



public:
    void getVersion(char* version);
    string getVersion();
    string getLibVersion();
    int loadLibrary();
    int unLoadLibrary();
    int init(char* result);
    bool isInitialized() const;
    int checkBalance(PiccParam piccParam, double *balance, char *pan);
    int debit(PiccParam piccParam, double value, const char *localTime, char *pan, double *balanceBefore, double *balanceAfter);
    int isMyCard(PiccParam piccParam, char* cardInfo);
    int setCardUserData(PiccParam piccParam, const char* key, const char* value);
    int getCardUserData(PiccParam piccParam, const char* key, char* value);
    void setWriteXCallBack(__writeX funcPtr);
    void setReadXCallBack(__readX funcPtr);
    void setSendApduToContactLessCardCallBack(__sendApduToContactLessCard funcPtr);
    void setSendApduToContactCardCallBack(__sendApduToContactCard funcPtr);
    void setSaveTransactionDataCallBack(__saveTransactionData funcPtr);
    void setShowMessageCallBack(__showMessage funcPtr);
    void setLogCallBack(__logger funcPtr);
    void setBeepCallBack(__beep funcPtr);

    void setBankConfigParam(const string &param);

    int getBankId() const {
        return bankId;
    }

    void setBankId(int bankId) {
        this->bankId = bankId;
    }

    string getBankName() const {
        return bankName;
    }

    void setBankName(string bankName) {
        this->bankName = bankName;
    }

    string getLibraryName() const {
        return libraryName;
    }

    void setLibraryName(string libraryName) {
        this->libraryName = libraryName;
    }

    string getErrorMessage() const {
        return errorMessage;
    }

    void setErrorMessage(string errorMessage) {
        this->errorMessage = errorMessage;
    }

    bool isActive() const {
        return active;
    }

    void setActive(bool active) {
        this->active = active;
    }

    int getSam_slot() const {
        return sam_slot;
    }

    void setSam_slot(int sam_slot) {
        this->sam_slot = sam_slot;
    }

};
#endif	/* BANKINGCARDMODULE_H */