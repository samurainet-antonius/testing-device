/* 
 * File:   Definition.h
 * Author: firman
 *
 * Created on February 29, 2016, 3:46 PM
 */

#ifndef DEFINITION_H
#define	DEFINITION_H
#include <stdint.h>

struct BankConfigProperty {
#ifdef	__cplusplus
    char key[100] = "\0";
    char value[200] = "\0";
#else    
    char key[100];
    char value[200];
#endif

};

struct BankConfigParam {
    int size;
    struct BankConfigProperty *bankConfigProperties;
};

typedef struct picc {
    uint8_t ATQA[2];
    uint8_t SAK;
    uint8_t CSN[10];
    uint8_t CSNlen;
} PiccParam;

enum CARD_TYPE {
    MIFARE_CARD = 0x08,
    MIFARE_CARD_X = 0x88,
    MIFARE_4K_CARD = 0x18,
    ISO14443A_CARD = 0x20,
    JCOP_CARD = 0x28,
    JCOP_CARD_NXP = 0x38
};


#endif	/* DEFINITION_H */

