#include "BankingCardModule.h"
#include "BankModule.h"
#include <cstddef>
#include <dlfcn.h>
#include <stdexcept>
#include <string.h>
#include "tiny-json.h"
#include "Logger.h"

#define CONFIG_BUFFER 2048

void *lib_handle;
char *error;
string _error = "";
BankConfigParam bankConfigParam;

int BankingCardModule::loadLibrary() {
    DEBUG("Load Dynamic Lib : " << libraryName.c_str(), "");
    //====== FOR DEBUG ============
    lib_handle = dlopen(libraryName.c_str(), RTLD_NOW);
    //    lib_handle = dlopen(libraryName.c_str(), RTLD_NOW | RTLD_GLOBAL);
    //otherwise use code bellow
    //    lib_handle = dlopen(libName, RTLD_LAZY);
    if (!lib_handle) {
        if ((error = dlerror()) != NULL) {
            throw runtime_error(error);
        }
    }
    *(void **) (&fn_getVersion) = dlsym(lib_handle, "getVersion");
    if ((error = dlerror()) != NULL) {
        throw runtime_error(error);
    }
    /**
     * Determine BankingCardModule API Version
     */
    char ver[20];
    //------- GET API VERSION -----------
    getVersion(ver);
    DEBUG("found API VERSION = " << ver, "");
    //-----------------------------------
    if (strcmp(ver, "1") == 0) {
        *(void **) (&fn_setConfig) = dlsym(lib_handle, "setConfig");
        if ((error = dlerror()) != NULL) {
            throw runtime_error(error);
        }
        *(void **) (&fn_init) = dlsym(lib_handle, "init");
        if ((error = dlerror()) != NULL) {
            throw runtime_error(error);
        }
        *(void **) (&fn_setWriteXCallBack) = dlsym(lib_handle, "setWriteXCallBack");
        if ((error = dlerror()) != NULL) {
            throw runtime_error(error);
        }
        *(void **) (&fn_setReadXCallBack) = dlsym(lib_handle, "setReadXCallBack");
        if ((error = dlerror()) != NULL) {
            throw runtime_error(error);
        }
        *(void **) (&fn_setSaveTransactionDataCallBack) = dlsym(lib_handle, "setSaveTransactionDataCallBack");
        if ((error = dlerror()) != NULL) {
            throw runtime_error(error);
        }
        *(void **) (&fn_setShowMessageCallBack) = dlsym(lib_handle, "setShowMessageCallBack");
        if ((error = dlerror()) != NULL) {
            throw runtime_error(error);
        }
        *(void **) (&fn_setLogCallBack) = dlsym(lib_handle, "setLogCallBack");
        if ((error = dlerror()) != NULL) {
            throw runtime_error(error);
        }
        *(void **) (&fn_setSendApduToContactLessCardCallBack) = dlsym(lib_handle, "setSendApduToContactLessCardCallBack");
        if ((error = dlerror()) != NULL) {
            throw runtime_error(error);
        }
        *(void **) (&fn_setSendApduToContactCardCallBack) = dlsym(lib_handle, "setSendApduToContactCardCallBack");
        if ((error = dlerror()) != NULL) {
            throw runtime_error(error);
        }
        *(void **) (&fn_setBeepCallBack) = dlsym(lib_handle, "setBeepCallBack");
        if ((error = dlerror()) != NULL) {
            throw runtime_error(error);
        }
        *(void **) (&fn_checkBalace) = dlsym(lib_handle, "checkBalance");
        if ((error = dlerror()) != NULL) {
            throw runtime_error(error);
        }
        *(void **) (&fn_debit) = dlsym(lib_handle, "debit");
        if ((error = dlerror()) != NULL) {
            throw runtime_error(error);
        }
        *(void **) (&fn_isMycard) = dlsym(lib_handle, "isMyCard");
        if ((error = dlerror()) != NULL) {
            throw runtime_error(error);
        }

    } else if (strcmp(ver, "2") == 0) {
        *(void **) (&fn_setConfig) = dlsym(lib_handle, "setConfig");
        if ((error = dlerror()) != NULL) {
            throw runtime_error(error);
        }
        *(void **) (&fn_init) = dlsym(lib_handle, "init");
        if ((error = dlerror()) != NULL) {
            throw runtime_error(error);
        }
        *(void **) (&fn_setWriteXCallBack) = dlsym(lib_handle, "setWriteXCallBack");
        if ((error = dlerror()) != NULL) {
            throw runtime_error(error);
        }
        *(void **) (&fn_setReadXCallBack) = dlsym(lib_handle, "setReadXCallBack");
        if ((error = dlerror()) != NULL) {
            throw runtime_error(error);
        }
        *(void **) (&fn_setSaveTransactionDataCallBack) = dlsym(lib_handle, "setSaveTransactionDataCallBack");
        if ((error = dlerror()) != NULL) {
            throw runtime_error(error);
        }
        *(void **) (&fn_setShowMessageCallBack) = dlsym(lib_handle, "setShowMessageCallBack");
        if ((error = dlerror()) != NULL) {
            throw runtime_error(error);
        }
        *(void **) (&fn_setLogCallBack) = dlsym(lib_handle, "setLogCallBack");
        if ((error = dlerror()) != NULL) {
            throw runtime_error(error);
        }
        *(void **) (&fn_setSendApduToContactLessCardCallBack) = dlsym(lib_handle, "setSendApduToContactLessCardCallBack");
        if ((error = dlerror()) != NULL) {
            throw runtime_error(error);
        }
        *(void **) (&fn_setSendApduToContactCardCallBack) = dlsym(lib_handle, "setSendApduToContactCardCallBack");
        if ((error = dlerror()) != NULL) {
            throw runtime_error(error);
        }
        *(void **) (&fn_setBeepCallBack) = dlsym(lib_handle, "setBeepCallBack");
        if ((error = dlerror()) != NULL) {
            throw runtime_error(error);
        }
        *(void **) (&fn_checkBalace) = dlsym(lib_handle, "checkBalance");
        if ((error = dlerror()) != NULL) {
            throw runtime_error(error);
        }
        *(void **) (&fn_debit) = dlsym(lib_handle, "debit");
        if ((error = dlerror()) != NULL) {
            throw runtime_error(error);
        }
        *(void **) (&fn_isMycard) = dlsym(lib_handle, "isMyCard");
        if ((error = dlerror()) != NULL) {
            throw runtime_error(error);
        }
        *(void **) (&fn_setCardUserData) = dlsym(lib_handle, "setCardUserData");
        if ((error = dlerror()) != NULL) {
            throw runtime_error(error);
        }
        *(void **) (&fn_getCardUserData) = dlsym(lib_handle, "getCardUserData");
        if ((error = dlerror()) != NULL) {
            throw runtime_error(error);
        }
    } else if (strcmp(ver, "3") == 0) {
        *(void **) (&fn_setConfig) = dlsym(lib_handle, "setConfig");
        if ((error = dlerror()) != NULL) {
            throw runtime_error(error);
        }
        *(void **) (&fn_init) = dlsym(lib_handle, "init");
        if ((error = dlerror()) != NULL) {
            throw runtime_error(error);
        }
        *(void **) (&fn_setWriteXCallBack) = dlsym(lib_handle, "setWriteXCallBack");
        if ((error = dlerror()) != NULL) {
            throw runtime_error(error);
        }
        *(void **) (&fn_setReadXCallBack) = dlsym(lib_handle, "setReadXCallBack");
        if ((error = dlerror()) != NULL) {
            throw runtime_error(error);
        }
        *(void **) (&fn_setSaveTransactionDataCallBack) = dlsym(lib_handle, "setSaveTransactionDataCallBack");
        if ((error = dlerror()) != NULL) {
            throw runtime_error(error);
        }
        *(void **) (&fn_setShowMessageCallBack) = dlsym(lib_handle, "setShowMessageCallBack");
        if ((error = dlerror()) != NULL) {
            throw runtime_error(error);
        }
        *(void **) (&fn_setLogCallBack) = dlsym(lib_handle, "setLogCallBack");
        if ((error = dlerror()) != NULL) {
            throw runtime_error(error);
        }
        *(void **) (&fn_setSendApduToContactLessCardCallBack) = dlsym(lib_handle, "setSendApduToContactLessCardCallBack");
        if ((error = dlerror()) != NULL) {
            throw runtime_error(error);
        }
        *(void **) (&fn_setSendApduToContactCardCallBack) = dlsym(lib_handle, "setSendApduToContactCardCallBack");
        if ((error = dlerror()) != NULL) {
            throw runtime_error(error);
        }
        *(void **) (&fn_setBeepCallBack) = dlsym(lib_handle, "setBeepCallBack");
        if ((error = dlerror()) != NULL) {
            throw runtime_error(error);
        }
        *(void **) (&fn_checkBalace) = dlsym(lib_handle, "checkBalance");
        if ((error = dlerror()) != NULL) {
            throw runtime_error(error);
        }
        *(void **) (&fn_debit) = dlsym(lib_handle, "debit");
        if ((error = dlerror()) != NULL) {
            throw runtime_error(error);
        }
        *(void **) (&fn_isMycard) = dlsym(lib_handle, "isMyCard");
        if ((error = dlerror()) != NULL) {
            throw runtime_error(error);
        }
        *(void **) (&fn_setCardUserData) = dlsym(lib_handle, "setCardUserData");
        if ((error = dlerror()) != NULL) {
            throw runtime_error(error);
        }
        *(void **) (&fn_getCardUserData) = dlsym(lib_handle, "getCardUserData");
        if ((error = dlerror()) != NULL) {
            throw runtime_error(error);
        }
        *(void **) (&fn_getLibVersion) = dlsym(lib_handle, "getLibVersion");
        if ((error = dlerror()) != NULL) {
            throw runtime_error(error);
        }
    }
    return 0;
}

int BankingCardModule::unLoadLibrary() {
    dlclose(lib_handle);
}

void BankingCardModule::getVersion(char* version) {
    if (fn_getVersion != NULL) {
        fn_getVersion(version);
    } else {
        char _NULL[4] = {'N', 'U', 'L', 'L'};
        memcpy(version, _NULL, 4);
    }
}

string BankingCardModule::getVersion() {
    char version[10] = "\0";
    if (fn_getVersion != NULL) {
        fn_getVersion(version);
    } else {
        char _NULL[4] = {'N', 'U', 'L', 'L'};
        memcpy(version, _NULL, 4);
    }
    return version;
}

string BankingCardModule::getLibVersion() {
    char version[10] = "\0";
    if (fn_getLibVersion != NULL) {
        fn_getLibVersion(version);
    } else {
        char _NULL[4] = {'N', 'U', 'L', 'L'};
        memcpy(version, _NULL, 4);
    }
    return version;
}

int BankingCardModule::init(char* result) {
    DEBUG("setting config " << getBankName(), "");
    if (fn_setConfig != NULL) {
        char *cstr = new char[config.length() + 1];
        strcpy(cstr,config.c_str());
        json_t mem[CONFIG_BUFFER];            
        json_t const* json = json_create( cstr, mem, sizeof mem / sizeof *mem );
                
        bankConfigParam.bankConfigProperties = new BankConfigProperty[CONFIG_BUFFER];
        int c = 0;
        json_t const* child;
        for( child = json_getChild( json ); child != 0; child = json_getSibling( child ) ) {
            char const* name = json_getName( child );
            char const* value = json_getValue( child );
            memcpy(bankConfigParam.bankConfigProperties[c].key, name, strlen(name));
            memcpy(bankConfigParam.bankConfigProperties[c].value, value, strlen(value));
            if (strcmp("samslot", bankConfigParam.bankConfigProperties[c].key) == 0) {
                sam_slot = atoi(bankConfigParam.bankConfigProperties[c].value);
            }
            c++;
        }
        bankConfigParam.size = c;
        BankModule::SAM_SLOT = sam_slot;
        fn_setConfig(bankConfigParam);
        DEBUG("slot sam : " << BankModule::SAM_SLOT, "");
        DEBUG("initializing " << getBankName(), "");
        if (fn_init(result) == 0) {
            initialized = true;
        } else {
            initialized = false;
        }
        DEBUG("init result " << bankName << " : " << initialized, "");
        return initialized;
    } else {
        return -1;
    }
}

bool BankingCardModule::isInitialized() const {
    return initialized;
}

int BankingCardModule::checkBalance(PiccParam piccParam, double *balance, char* pan) {
    BankModule::SAM_SLOT = sam_slot;
    DEBUG("slot sam : " << BankModule::SAM_SLOT, "");
    if (fn_checkBalace != NULL) {
        return fn_checkBalace(&piccParam, balance, pan);
    } else {
        return -1;
    }

}

int BankingCardModule::debit(PiccParam piccParam, double value, const char* localTime, char* pan, double *balanceBefore, double *balanceAfter) {
    BankModule::SAM_SLOT = sam_slot;
    DEBUG("slot sam : " << BankModule::SAM_SLOT, "");
    if (fn_debit != NULL) {
        return fn_debit(&piccParam, value, localTime, pan, balanceBefore, balanceAfter);
    } else {
        return -1;
    }
}

int BankingCardModule::isMyCard(PiccParam piccParam, char* cardInfo) {
    BankModule::SAM_SLOT = sam_slot;
    DEBUG("slot sam : " << BankModule::SAM_SLOT, "");
    if (fn_isMycard != NULL) {
        return fn_isMycard(&piccParam, cardInfo);
    } else {
        return -1;
    }
}

int BankingCardModule::setCardUserData(PiccParam piccParam, const char* key, const char* value) {
    BankModule::SAM_SLOT = sam_slot;
    DEBUG("slot sam : " << BankModule::SAM_SLOT, "");
    if (fn_setCardUserData != NULL) {
        return fn_setCardUserData(&piccParam, key, value);
    } else {
        return -1;
    }
}

int BankingCardModule::getCardUserData(PiccParam piccParam, const char* key, char* value) {
    BankModule::SAM_SLOT = sam_slot;
    DEBUG("slot sam : " << BankModule::SAM_SLOT, "");
    if (fn_getCardUserData != NULL) {
        return fn_getCardUserData(&piccParam, key, value);
    } else {
        return -1;
    }
}

void BankingCardModule::setWriteXCallBack(__writeX funcPtr) {
    fn_setWriteXCallBack(funcPtr);
}

void BankingCardModule::setReadXCallBack(__readX funcPtr) {
    fn_setReadXCallBack(funcPtr);
}

void BankingCardModule::setSendApduToContactCardCallBack(__sendApduToContactCard funcPtr) {
    fn_setSendApduToContactCardCallBack(funcPtr);
}

void BankingCardModule::setSendApduToContactLessCardCallBack(__sendApduToContactLessCard funcPtr) {
    fn_setSendApduToContactLessCardCallBack(funcPtr);
}

void BankingCardModule::setSaveTransactionDataCallBack(__saveTransactionData funcPtr) {
    fn_setSaveTransactionDataCallBack(funcPtr);
}

void BankingCardModule::setShowMessageCallBack(__showMessage funcPtr) {
    fn_setShowMessageCallBack(funcPtr);
}

void BankingCardModule::setLogCallBack(__logger funcPtr) {
    fn_setLogCallBack(funcPtr);
}

void BankingCardModule::setBeepCallBack(__beep funcPtr) {
    fn_setBeepCallBack(funcPtr);
}

void BankingCardModule::setBankConfigParam(const string &param) {
    config = param;
}
