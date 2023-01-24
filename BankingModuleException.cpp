/* 
 * File:   BankingModuleException.cpp
 * Author: firman
 * 
 * Created on November 12, 2015, 1:36 PM
 */

#include "BankingModuleException.h"

BankingModuleException::BankingModuleException(const char* msg) {
    this->msg = msg;
}

BankingModuleException::BankingModuleException(const BankingModuleException& orig) {
}

BankingModuleException::~BankingModuleException() {
}

const char* BankingModuleException::what() const throw() {
    return this->msg.c_str();
}
