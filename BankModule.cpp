#include <unistd.h>
#include "BankModule.h"
#include "Logger.h"

//---------- INITIALIZING callback ----------------
BankModule::displayMessage BankModule::_displayMessage = NULL;
BankModule::readX BankModule::_readX = NULL;
BankModule::saveData BankModule::_saveData = NULL;
BankModule::sendAPDUContact BankModule::_sendAPDUContact = NULL;
BankModule::sendAPDUContactless BankModule::_sendAPDUContactless = NULL;
BankModule::writeX BankModule::_writeX = NULL;
BankModule::Beep BankModule::_beep = NULL;
int BankModule::SAM_SLOT = 0;
//-------------------------------------------------
bool showingMessage;

int BankModule::initialize() {
    int error = 0;
    for (vector<BankingCardModule*>::iterator it = bankingCards.begin(); it != bankingCards.end(); ++it) {
        try {
            if ((*it)->isActive()) {
                DEBUG("=============INIT " << (*it)->getBankName() << "==============", "");
                if (!(*it)->isInitialized()) {
                    (*it)->loadLibrary();
                    (*it)->setWriteXCallBack(writeXWrap);
                    (*it)->setReadXCallBack(readXWrap);
                    (*it)->setSendApduToContactCardCallBack(sendAPDUContactWrap);
                    (*it)->setSendApduToContactLessCardCallBack(sendAPDUContactlessWrap);
                    (*it)->setShowMessageCallBack(displayMessageWrap);
                    (*it)->setLogCallBack(log);
                    (*it)->setBeepCallBack(beep);
                    (*it)->setSaveTransactionDataCallBack(saveDataWrap);
                    char ver[20] = "\0";
                    (*it)->getVersion(ver);
                    //                    string v(ver);
                    DEBUG("successfuly load " << (*it)->getBankName() << ":" << (*it)->getLibVersion() << " lib API v" << ver << " id " << (*it)->getBankId(), "");

                    char res[1024] = {"\0"};
                    if (!(*it)->init(res)) {
                        throw BankingModuleException(res);
                    }
                }
            }
        } catch (exception &ex) {
            error++;
            //FATAL("error when init " << (*it)->getBankName(), ex.what());
            (*it)->setErrorMessage(ex.what());
        }
        if ((*it)->isActive()) {
            DEBUG("===========================", "");
        }
    }

    return error;
}

int BankModule::autoDetectBankCardType(PiccParam piccParam, int *bankId, string *bankName, char* cardInfo) {
    DEBUG("Detect Card", "");
    for (vector<BankingCardModule*>::iterator it = bankingCards.begin(); it != bankingCards.end(); ++it) {
        if ((*it)->isInitialized()) {
            if ((*it)->isMyCard(piccParam, cardInfo) == 0) {
                bankID = (*it)->getBankId();
                *bankId = this->bankID;
                *bankName = (*it)->getBankName();
                DEBUG("Detected card : " << getBankName(), "");
                return 0;
            }
        }
    }
    return -1;
}

int BankModule::isMyCard(PiccParam piccParam, int bankId, char* cardInfo) {
    for (vector<BankingCardModule*>::iterator it = bankingCards.begin(); it != bankingCards.end(); ++it) {
        if ((*it)->getBankId() == bankId && (*it)->isInitialized()) {
            if ((*it)->isActive()) {
                return (*it)->isMyCard(piccParam, cardInfo);
            } else {
                //module is not activated
                return 4;
            }
        }
    }
    return -1;
}

//int BankModule::debit(PiccParam piccParam, double value, const char* localTime, char* pan, char* balanceBefore, char* balanceAfter) {
//    for (vector<BankingCardModule*>::iterator it = bankingCards.begin(); it != bankingCards.end(); ++it) {
//        if ((*it)->getBankId() == bankID && (*it)->isInitialized()) {
//            return (*it)->debit(piccParam, value, localTime, pan, balanceBefore, balanceAfter);
//        }
//    }
//    return -1;
//}

int BankModule::debit(PiccParam piccParam, int bankId, double value, const char *localTime, char *pan, double *balanceBefore, double *balanceAfter) {
    showingMessage = false;
    for (vector<BankingCardModule*>::iterator it = bankingCards.begin(); it != bankingCards.end(); ++it) {
        if ((*it)->getBankId() == bankId && (*it)->isInitialized()) {
            DEBUG("debit to bank id : " << bankId, "");
            if ((*it)->isActive()) {
                bankID = bankId;
                int ret_v = (*it)->debit(piccParam, value, localTime, pan, balanceBefore, balanceAfter);
                if (ret_v == 1 && showingMessage) {
                    //error with message
                    return 6;
                } else {
                    return ret_v;
                }
            } else {
                //module is not activated
                return 4;
            }
        } else {
            if (!(*it)->isInitialized()) {
                DEBUG("bank id : " << (*it)->getBankId() << ",name : " << (*it)->getBankName() << " is not initialized", "");
            }
        }
    }
    return -1;
}

//int BankModule::checkBalance(PiccParam piccParam, char* result, char* pan) {
//    for (vector<BankingCardModule*>::iterator it = bankingCards.begin(); it != bankingCards.end(); ++it) {
//        if ((*it)->getBankId() == bankID && (*it)->isInitialized()) {
//            return (*it)->checkBalance(piccParam, result, pan);
//        }
//    }
//    return -1;
//}

int BankModule::checkBalance(PiccParam piccParam, int bankId, double *result, char* pan) {
    for (vector<BankingCardModule*>::iterator it = bankingCards.begin(); it != bankingCards.end(); ++it) {
        if ((*it)->getBankId() == bankId && (*it)->isInitialized()) {
            if ((*it)->isActive()) {
                bankID = bankId;
                return (*it)->checkBalance(piccParam, result, pan);
            } else {
                //module is not activated
                return 4;
            }
        }
    }
    return -1;
}

int BankModule::setCardUserData(PiccParam piccParam, int bankId, const char* key, const char* userData) {
    for (vector<BankingCardModule*>::iterator it = bankingCards.begin(); it != bankingCards.end(); ++it) {
        if ((*it)->getBankId() == bankId && (*it)->isInitialized()) {
            if ((*it)->isActive()) {
                bankID = bankId;
                return (*it)->setCardUserData(piccParam, key, userData);
            } else {
                //module is not activated
                return 4;
            }
        }
    }
    return -1;
}

int BankModule::getCardUserData(PiccParam piccParam, int bankId, const char* key, char* userData) {
    for (vector<BankingCardModule*>::iterator it = bankingCards.begin(); it != bankingCards.end(); ++it) {
        if ((*it)->getBankId() == bankId && (*it)->isInitialized()) {
            if ((*it)->isActive()) {
                bankID = bankId;
                return (*it)->getCardUserData(piccParam, key, userData);
            } else {
                //module is not activated
                return 4;
            }
        }
    }
    return -1;
}

string BankModule::getBankName() {
    for (vector<BankingCardModule*>::iterator it = bankingCards.begin(); it != bankingCards.end(); ++it) {
        if ((*it)->getBankId() == bankID) {
            return (*it)->getBankName();
        }
    }
    return "not_found";
}

int BankModule::displayMessageWrap(const char* value, int timeout) {
    DEBUG(value << "[to:" << timeout << "]", "");
    showingMessage = true;
    return BankModule::_displayMessage(value, timeout);
}

int BankModule::readXWrap(char* buf, int *len) {
    return BankModule::_readX(buf, len);
}

int BankModule::saveDataWrap(const char* dataRaw, const char* txnStatus) {
    return BankModule::_saveData(dataRaw, txnStatus);
}

int BankModule::sendAPDUContactWrap(const char* apdu, int apduLen, char* response, int *responseLen) {
    DEBUG("slot sam : " << SAM_SLOT, "");
    return BankModule::_sendAPDUContact(apdu, apduLen, response, responseLen, SAM_SLOT);
}

int BankModule::sendAPDUContactlessWrap(const char* apdu, int apduLen, char* response, int *responseLen) {
    return BankModule::_sendAPDUContactless(apdu, apduLen, response, responseLen);
}

int BankModule::writeXWrap(const char* buf, int n) {
    return BankModule::_writeX(buf, n);
}

int BankModule::log(const char* msg, const char* cause, int level) {
    switch (level) {
        case 0:
            INFO(msg, cause);
            break;
        case 1:
            DEBUG(msg, cause);
            break;
        case 2:
            FATAL(msg, cause);
            break;
        case 3:
            WARN(msg, cause);
            break;
        case 4:
            TRACE(msg, cause);
            break;
        default:
            DEBUG(msg, cause);
            break;
    }
}

int BankModule::beep(int n, int duration, int delay) {
    _beep(n, duration, delay);
}

BankingCardModule* BankModule::getBankingCardModule(int bankId) const {
    auto it = find_if(this->bankingCards.begin(), this->bankingCards.end(), [ = ](BankingCardModule * p){
        return p->getBankId() == bankId;
    });
    if (it != this->bankingCards.end()) {
        return (*it);
    } else {
        return NULL;
    }
}

int BankModule::registerBankingCard(BankingCardModule* bankingCard) {
    auto it = find_if(this->bankingCards.begin(), this->bankingCards.end(), [ = ](BankingCardModule * p){
        return p->getBankId() == bankingCard->getBankId();
    });
    if (it == this->bankingCards.end()) {
        this->bankingCards.push_back(bankingCard);
        return 0;
    } else {
        return -1;
    }
}

int BankModule::unRegisterBankingCard(BankingCardModule* bankingCard) {
    auto it = find_if(this->bankingCards.begin(), this->bankingCards.end(), [ = ](BankingCardModule * p){
        return p->getBankId() == bankingCard->getBankId();
    });
    if (it != this->bankingCards.end()) {
        (*it)->unLoadLibrary();
        this->bankingCards.erase(it);
        return 0;
    } else {
        return -1;
    }
}






