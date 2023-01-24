#ifndef _READER_WRAPPER_H_
#define _READER_WRAPPER_H_

#ifdef	__cplusplus
extern "C" {
#endif

#include "Definition.h"

#define VERSION "2.0.0"

    int initialize(char type, const char *path, int baud);
    int deInitialize();

    int sendAPDUContactLess(const char* apdu, int apduLen, char* response, int *responseLen);
    int sendAPDUContact(const char* apdu, int apduLen, char* response, int *responseLen, int slotSam);

    int sendX(const char* cmd, int cmdLen, char *response, int *responseLen);
    int writeX(const char* cmd, int cmdLen);
    int readX(char *response, int *responseLen);

    int activateCard(char *ret, int *retLen);
    int deactivateCard();
    int getSmartCard(PiccParam *ret);
    void getVersion(char *version);
    
    // Mifare function
    int MifareLoadKey(const char *key);
    int MifareAuth(char keyType,char blockNo,const char *CSN);
    int MifareRead(char blockNo,char *data);
    int MifareWrite(char blockNo,const char *data);
#endif


#ifdef	__cplusplus
}
#endif
