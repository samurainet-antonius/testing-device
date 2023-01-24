/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <cstdlib>
#include <iostream>
#include <string>
#include <algorithm>
#include <string.h>
#include "AinoPaymentModule.h"
#include "BankModule.h"
#include "BankingCardModule.h"
#include "reader-wrapper.h"
#include "Definition.h"
#include "tiny-json.h"
#include "json-util.h"
#include "Logger.h"

#define CONFIG_BUFFER 2048

using namespace std;


BankingCardModule *myBankingCardModule;
BankModule myBankModule;

Logger myLogger;

PiccParam myPicc;

int bank_id = 0;
char trx_data[1024] = {0};
    
int displayMessage(const char* value, int timeout){
    printf("Message : %s\n",value);
    return 0;
}

int beep(int n, int duration, int delay) {
    printf("BEEP\n");
    return 0;
}

int saveData(const char* dataRaw, const char* txnStatus) {
    memset(trx_data,0,sizeof(trx_data));
    sprintf(trx_data,"%s\0",dataRaw);
    return 0;
}

int InitializeModule(char *config){
    int baudrate = 0;         
    int return_value = 0;
    
    char comm_type[16] = {0};
    char path[64] = {0};
    char buffer[CONFIG_BUFFER] = {0};
    
    json_t mem[CONFIG_BUFFER];    
    json_t const* json = json_create( config, mem, sizeof mem / sizeof *mem );
        
    json_t const* child;
        
    myLogger.setWorkingDirectory("/aino/app/");
    myLogger.initialize();
    
    for( child = json_getChild( json ); child != 0; child = json_getSibling( child ) ) {
        
        myBankingCardModule = new BankingCardModule;
        
        jsonType_t propertyType = json_getType( child );
        char const* name = json_getName( child );
        char const* value;        
        
        if (propertyType == JSON_OBJ) {
            if ( name ) printf("%s\n", name );
        
            myBankingCardModule->setActive(true);
            myBankingCardModule->setBankName(string (name));
            myBankingCardModule->setBankId(bank_id);

            string libraryname = "\0";
            libraryname.append("lib");
            libraryname.append(name);
            libraryname.append(".so");

            printf("%s\n", libraryname.c_str() );
            myBankingCardModule->setLibraryName(libraryname);

            memset(buffer,0,sizeof(buffer));
            json2text(child,buffer);
            printf("%s\n", buffer);
            myBankingCardModule->setBankConfigParam(string (buffer));

            myBankModule.registerBankingCard(myBankingCardModule);
            bank_id++;
        } else {
            if (strcmp(name,"path") == 0) {
                strcpy(path,json_getValue( child ));
                printf("PATH : %s\n",path);
            } else if (strcmp(name,"comm_type") == 0) {
                strcpy(comm_type,json_getValue( child ));
                printf("COMM TYPE : %s\n",comm_type);
            } else if (strcmp(name,"baudrate") == 0) {
                baudrate = json_getInteger( child );
                printf("BAUDRATE : %u\n",baudrate);
            }
        }
    }    
    
    return_value = initialize(comm_type[0],path,baudrate);
    if (return_value != 0) return return_value;
    
    myBankModule.setSendAPDUContactCallback(sendAPDUContact);
    myBankModule.setSendAPDUContactlessCallback(sendAPDUContactLess);
    myBankModule.setReadXCallback(readX);
    myBankModule.setWriteXCallback(writeX);
    myBankModule.setDisplayMessageCallback(displayMessage);
    myBankModule.setBeepCallback(beep);
    myBankModule.setSaveDataCallback(saveData);
    
    myBankModule.initialize();
    
    return 0;
}

int DetectCard(char *card_info){
    int return_value = 0;
    string *bank_string = new string;
    char bank_char[255] = {0};
    char info_char[255] = {0};
    char ATS_char[32] = {0};
    int ATS_len = 0;
    
    return_value = getSmartCard(&myPicc);
    if (return_value != 0) return return_value;
    
    switch(myPicc.SAK) {
        case CARD_TYPE::MIFARE_4K_CARD :
        case CARD_TYPE::MIFARE_CARD :
        case CARD_TYPE::MIFARE_CARD_X :
            return_value = myBankModule.autoDetectBankCardType(myPicc,&bank_id,bank_string,info_char);
            if (return_value != 0) return return_value;
            strcpy(bank_char,bank_string->c_str());
            memcpy(card_info,bank_char,strlen(bank_char));
            memcpy(card_info+strlen(bank_char),",",1);
            memcpy(card_info+strlen(bank_char)+1,info_char,strlen(info_char));
            break;
        case CARD_TYPE::ISO14443A_CARD :
        case CARD_TYPE::JCOP_CARD:
        case CARD_TYPE::JCOP_CARD_NXP:
            return_value = activateCard(ATS_char,&ATS_len);
            if (return_value != 0) return return_value;
            return_value = myBankModule.autoDetectBankCardType(myPicc,&bank_id,bank_string,info_char);
            if (return_value != 0) return return_value;
            strcpy(bank_char,bank_string->c_str());
            memcpy(card_info,bank_char,strlen(bank_char));
            memcpy(card_info+strlen(bank_char),",",1);
            memcpy(card_info+strlen(bank_char)+1,info_char,strlen(info_char));
            break;            
    }    
    return 0;    
};

int CheckBalance(double *balance, char* pan){
    int return_value = 0;
    return_value = myBankModule.checkBalance(myPicc,bank_id,balance,pan);
    if (return_value != 0) return return_value;
    return 0;
}

int Debit(double value, const char *localTime, char *pan, double *balanceBefore, double *balanceAfter,char *transaction_data){
    int return_value = 0;
    return_value = myBankModule.debit(myPicc,bank_id,value,localTime,pan,balanceBefore,balanceAfter);
    if (return_value == 0) {
        strcpy(transaction_data,trx_data);
        return 0;
    }
    return return_value;
}
