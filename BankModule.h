#ifndef BANKINGMODULE_H
#define	BANKINGMODULE_H

#include "BankType.h"
#include <string>
#include <stddef.h>
#include <functional>
#include "BankingCardModule.h"
#include "BankingModuleException.h"
#include <vector>
#include <set>
#include <algorithm>
#include <stdint.h>

#define LIB_BANK_VERSION "1.1.1"

using namespace std;

class BankModule {
private:

    typedef function<int(const char*, int) > writeX;
    typedef function<int(char*, int*) > readX;
    typedef function<int(const char*, int, char*, int*) > sendAPDUContactless;
    typedef function<int(const char*, int, char*, int*, int) > sendAPDUContact;
    typedef function<int(const char*, const char*) > saveData;
    typedef function<int(const char*, int) > displayMessage;
    typedef function<void(int, int, int) > Beep;

    vector<BankingCardModule*> bankingCards;
    static writeX _writeX;
    static readX _readX;
    static displayMessage _displayMessage;
    static saveData _saveData;
    static sendAPDUContactless _sendAPDUContactless;
    static sendAPDUContact _sendAPDUContact;
    static Beep _beep;

    int bankID;
    const string version = LIB_BANK_VERSION;

    //---------- WRAPPER FUNCTION FOR C Dynamic Lib ---------------
    static int writeXWrap(const char* buf, int n);
    static int readXWrap(char* buf, int *len);
    static int sendAPDUContactlessWrap(const char* apdu, int apduLen, char* response, int *responseLen);
    static int sendAPDUContactWrap(const char* apdu, int apduLen, char* response, int *responseLen);
    static int saveDataWrap(const char* dataraw, const char* txnStatus);
    static int displayMessageWrap(const char* value, int timeout);
    static int log(const char* msg, const char* cause, int level);
    static int beep(int n, int duration, int delay);
    //-------------------------------------------------------------

    void resetBankIdValue() {
        this->bankID = 0;
    }
public:

    BankModule() {
        unique(bankingCards.begin(), bankingCards.end());
    }
    int initialize();
    int autoDetectBankCardType(PiccParam piccParam, int *bankId, string *bankName, char* cardInfo);
    int isMyCard(PiccParam piccParam, int bankId, char* cardInfo);
    int debit(PiccParam piccParam, int bankId, double value, const char* localTime, char *pan, double *balanceBefore, double *balanceAfter);
    int checkBalance(PiccParam piccParam, int bankId, double *result, char* pan);
    int setCardUserData(PiccParam piccParam, int bankId, const char* key, const char* userData);
    int getCardUserData(PiccParam piccParam, int bankId, const char* key, char* userData);
    string getBankName();
    void setBankName(int bankId);

    static int SAM_SLOT;

    int getBankID() const {
        return bankID;
    }

    void setDisplayMessageCallback(displayMessage fPtr) {
        BankModule::_displayMessage = fPtr;
    }

    void setBeepCallback(Beep fPtr) {
        BankModule::_beep = fPtr;
    }

    void setReadXCallback(readX fPtr) {
        BankModule::_readX = fPtr;
    }

    void setSaveDataCallback(saveData fPtr) {
        BankModule::_saveData = fPtr;
    }

    void setSendAPDUContactCallback(sendAPDUContact fPtr) {
        BankModule::_sendAPDUContact = fPtr;
    }

    void setSendAPDUContactlessCallback(sendAPDUContactless fPtr) {
        BankModule::_sendAPDUContactless = fPtr;
    }

    void setWriteXCallback(writeX fPtr) {
        BankModule::_writeX = fPtr;
    }

    const string getVersion() const {
        return version;
    }

    vector<BankingCardModule*> getBankingCards() const {
        return bankingCards;
    }

    BankingCardModule* getBankingCardModule(int bankId) const;

    int registerBankingCard(BankingCardModule *bankingCard);

    int unRegisterBankingCard(BankingCardModule *bankingCard);


};
#endif	/* BANKINGMODULE_H */